#include "engine/util/local_process_launcher.h"

namespace engine {
namespace util {
LOG_SETUP(engine.util, LocalProcessLauncher);

#define STDOUT_FILE_NAME "stdout.out"
#define STDERR_FILE_NAME "stderr.out"
const std::string ProcessPidFile = "process.pid";
const int PATH_MAX = 4096;

LocalProcessLauncher::LocalProcessLauncher() {}
LocalProcessLauncher::~LocalProcessLauncher() {}

pid_t LocalProcessLauncher::Start(const std::string &path,
                                  const vector<string> &args,
                                  const KeyValueMap &envs,
                                  const std::string &workDir) {
    if (!CreateWorkDir(workDir)) {
        LOG(ERROR,
            "create workdir failed for process:[%s], "
            "workdir:[%s]",
            path.c_str(),
            workDir.c_str());
        return -1;
    }
    std::string pidFile = workDir + "/" + ProcessPidFile;
    if (::remove(pidFile.c_str()) != 0) {
        if (errno != ENOENT) {
            LOG(ERROR, "remove file %s failed, error no %s ", pidFile.c_str(), strerror(errno));
            return -1;
        }
    }

    std::vector<std::string> tmp_args = args;
    auto cmdline = CreateCmd(path, tmp_args);
    pid_t pid = fork();
    if (pid == 0) {
        setsid();
        (void)chdir(workDir.c_str());

        fclose(stdout);
        fclose(stderr);
        umask(0);

        std::string pidSuffixStr = "." + StringUtil::ToString(getpid());
        std::string stdoutFile = std::string(STDOUT_FILE_NAME) + pidSuffixStr;
        std::string stderrFile = d::string(STDERR_FILE_NAME) + pidSuffixStr;
        (void)freopen(stdoutFile.c_str(), "w+", stdout);
        (void)freopen(stderrFile.c_str(), "w+", stderr);

        if (!SetEnv(envs)) {
            fprintf(stderr, "set envs failed.");
            fflush(stderr);
            _exit(-1);
        }

        if (!CloseFds()) {
            fprintf(stderr, "close fds failed.");
            fflush(stderr);
            _exit(-1);
        }
        // get the proc's starttime as the proc signature
        std::string procSignature;
        pid_t curPid = getpid();
        if (!GenerateProcSignature(curPid, &procSignature)) {
            fprintf(stderr, "get proc[%d] signature failed.", curPid);
            fflush(stderr);
            _exit(-1);
        }
        std::string pidFile = workDir + "/" + ProcessPidFile;
        if (::remove(pidFile.c_str()) != 0) {
            if (errno != ENOENT) {
                fprintf(stderr, "remove file %s fail, %s", pidFile.c_str(), strerror(errno));
                fflush(stderr);
                _exit(-1);
            }
        }

        // persist pid signature info
        std::string tmpSignatureFilePath = pidFile + "__tmp__";
        ofstream out(tmpSignatureFilePath.c_str());
        if (!out || !out.write(procSignature.c_str(), procSignature.size()) ||
            rename(tmpSignatureFilePath.c_str(), pidFile.c_str()) != 0) {
            fprintf(stderr, "create signature file failed.");
            fflush(stderr);
            out.close();
            _exit(-1);
        }
        out.close();
        auto cmds = cmdline;
        execvp(path.c_str(), cmds.get());

        fprintf(stderr,
                "command(%s) is not execvp successful, "
                "error code: %d(%s)\n",
                path.c_str(),
                errno,
                strerror(errno));
        fflush(stdout);
        fflush(stderr);
        _exit(-1); // error occurred
    } else {
        // do nothing;
    }

    LOG(INFO, "start process. path:[%s], pid:[%d], workdir:[%s]", path.c_str(), pid, workDir.c_str());
    PrintArgsAndEnvs(path, tmp_args, envs);
    return pid;
}

bool LocalProcessLauncher::Stop(pid_t pid, int32_t sig) {
    assert(pid > 1);
    if (KillProcess(-pid, sig) != 0) {
        int ec = errno;
        LOG(ERROR,
            "kill process failed. pid:[%d], sig:[%d], "
            "errno:[%d], errmsg:[%s]",
            pid,
            sig,
            ec,
            strerror(ec));
        return false;
    }
    LOG(INFO, "stop process [%d] by sig [%d]", pid, sig);
    return true;
}

LocalProcessLauncher::ExitStatus
LocalProcessLauncher::ParseExitStatus(pid_t pid, int status, int32_t *exitCode, int32_t *exitSig) {
    *exitCode = 0;
    *exitSig = 0;
    if (WIFEXITED(status)) {
        *exitCode = WEXITSTATUS(status);
        LOG(INFO, "process exit normal. pid:[%d], exitcode:[%d]", pid, *exitCode);
        return NORMAL_EXITED;
    } else if (WIFSIGNALED(status)) {
        *exitSig = WTERMSIG(status);
        LOG(INFO, "process exit by signal. pid:[%d], signal:[%d]", pid, *exitSig);
        return SIG_EXITED;
    } else if (WIFSTOPPED(status)) {
        *exitSig = WSTOPSIG(status);
        LOG(INFO,
            "process was stopped by signal."
            " pid:[%d], signal:[%d]",
            pid,
            *exitSig);
        return SIG_STOPPED;
    }
    return NORMAL_EXITED;
}

ProcStat LocalProcessLauncher::GetProcStat(pid_t pid, const std::string &workDir) {
    ProcStat procStat;
    int status = 0;
    pid_t wpid = waitpid(pid, &status, WNOHANG);
    int err = errno;
    if (wpid == 0) {
        LOG(DEBUG, "pid[%d] is running:%d", pid, wpid);
        procStat.isExited = false;
        return procStat;
    }

    if (wpid > 0) {
        LOG(DEBUG, "process wait. pid:[%d], status:[%d]", pid, status);
        LocalProcessLauncher::ExitStatus exitStatus =
            ParseExitStatus(pid, status, &procStat.exitCode, &procStat.exitSig);
        if (exitStatus == NORMAL_EXITED || exitStatus == SIG_EXITED) {
            procStat.isExited = true;
            if (procStat.exitCode == 0 && procStat.exitSig == 0) {
                procStat.normalExited = true;
            } else {
                procStat.normalExited = false;
            }
        } else {
            procStat.isExited = false;
        }
    } else if (wpid < 0) {
        if (err == ECHILD) {
            // not my child or process does not exist
            if (LocalProcessLauncher::ProcessDirExist(pid) && IsMyProcess(pid, workDir)) {
                procStat.isExited = false;
            } else {
                LOG(INFO,
                    "process [%d] not exist with errno[%d], "
                    "it will be deal with NORMAL_EXITED",
                    pid,
                    errno);
                procStat.isExited = true;
                procStat.normalExited = true;
            }
        }
    }

    return procStat;
}

int32_t LocalProcessLauncher::KillProcess(pid_t pid, int32_t signal) { return kill(pid, signal); }

bool LocalProcessLauncher::IsMyProcess(pid_t pid, const std::string &workDir) {
    std::string procWorkDir = GetCwd(pid);
    if (not StringUtil::endsWith(procWorkDir, workDir)) { // work dir may soft link
        LOG(WARN, "process [%d] workdir not match, cur:[%s], expect:[%s]", pid, procWorkDir.c_str(), workDir.c_str());
        return false;
    }
    return true;
}

bool LocalProcessLauncher::HasPorcessPidFile(pid_t pid) {
    std::string procWorkDir = GetCwd(pid);
    std::string processPidFile = procWorkDir + "/" + ProcessPidFile;
    fslib::ErrorCode ec = FileSystem::isExist(processPidFile);
    if (ec != fslib::EC_TRUE) {
        LOG(INFO, "process[%s] is not exist ", processPidFile.c_str());
        return false;
    }
    return true;
}

bool LocalProcessLauncher::createWorkDir(const std::string &dir) {
    fslib::ErrorCode ec = FileSystem::isExist(dir);
    if (fslib::EC_TRUE == ec) {
        return true;
    } else if (fslib::EC_FALSE == ec) {
        ec = FileSystem::mkDir(dir, true);
        if (ec != fslib::EC_OK) {
            LOG(ERROR, "create dir failed. dir:[%s], error:[%s]", dir.c_str(), FileSystem::getErrorString(ec).c_str());
            return false;
        }
        return true;
    }
    return false;
}

bool LocalProcessLauncher::setEnv(const map<string, string> &envs) {
    for (KeyValueMap::const_iterator it = envs.begin(); it != envs.end(); it++) {
        bool ret = autil::EnvUtil::setEnv(it->first, it->second);
        fprintf(stderr, "set env key:%s, value:%s\n", it->first.c_str(), it->second.c_str());
        fflush(stderr);
        if (!ret) {
            fprintf(stderr, "set env error, key:%s, value:%s\n", it->first.c_str(), it->second.c_str());
            fflush(stderr);
            return false;
        }
    }
    return true;
}

void LocalProcessLauncher::printArgsAndEnvs(const std::string &path, const vector<string> &args, const KeyValueMap &envs) {
    stringstream ss;
    ss << "args:";
    for (vector<string>::const_iterator it = args.begin(); it != args.end(); it++) {
        ss << " " << *it;
    }
    ss << endl << "envs:";
    for (KeyValueMap::const_iterator it = envs.begin(); it != envs.end(); it++) {
        ss << " " << it->first << "=" << it->second;
    }
    ss << endl;
    LOG(INFO, "%s", ss.str().c_str());
}

bool LocalProcessLauncher::closeFds() {
    vector<int32_t> fds;
    if (!getAllFds(&fds)) {
        return false;
    }
    for (size_t i = 0; i < fds.size(); i++) {
        if (fds[i] > 2) {
            close(fds[i]);
        }
    }
    return true;
}

shared_ptr<char *> LocalProcessLauncher::CreateCmd(const std::string &path, const vector<string> &args) {
    shared_ptr<char *> cmdlinePtr(new char *[args.size() + 2], [](char **p) { delete[] p; });
    char **cmdline = cmdlinePtr.get();
    cmdline[0] = const_cast<char *>(path.c_str());

    size_t argNum = 1;
    for (vector<string>::const_iterator it = args.begin(); it != args.end(); it++) {
        if ((*it) != "") {
            cmdline[argNum++] = const_cast<char *>(it->c_str());
        }
    }

    cmdline[argNum] = NULL;
    return cmdlinePtr;
}

bool LocalProcessLauncher::generateProcSignature(pid_t pid, std::string *signature) {
    std::string strPid = StringUtil::toString(pid);
    std::string startTime;
    if (!LocalProcessLauncher::getStartTime(pid, &startTime)) {
        return false;
    }
    *signature = strPid + ":" + startTime;
    return true;
}

bool LocalProcessLauncher::getStartTime(pid_t pid, std::string *startTime) {
    std::string strPid = StringUtil::toString(pid);
    std::string statFilePath = "/proc/" + strPid + "/stat";
    ifstream in(statFilePath.c_str());
    if (!in) {
        return false;
    }
    std::string line;
    std::getline(in, line);
    in.close();
    vector<string> tmp;
    istringstream iss(line);
    std::copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string>>(tmp));

    if (tmp.size() < 22) {
        return false;
    }
    *startTime = tmp.at(21);
    return true;
}

bool LocalProcessLauncher::processDirExist(pid_t pid) {
    stringstream ss;
    ss << "/proc/" << pid;
    fslib::ErrorCode ec = FileSystem::isExist(ss.str());
    if (ec != fslib::EC_TRUE) {
        LOG(INFO, "process[%d] is not exist in /proc dir", pid);
        return false;
    }
    return true;
}

bool LocalProcessLauncher::getAllFds(std::vector<int32_t> *fds) {
    int32_t pid = getpid();
    std::string dirName = string("/proc/") + StringUtil::toString(pid) + "/fd";
    vector<string> subFiles;
    fslib::ErrorCode ec = fs::FileSystem::listDir(dirName, subFiles);
    if (ec != fslib::EC_OK) {
        return false;
    }
    fds->clear();
    for (size_t i = 0; i < subFiles.size(); i++) {
        int32_t fd = -1;
        const std::string &fdStr = subFiles[i];
        if (!StringUtil::strToInt32(fdStr.c_str(), fd)) {
            LOG(WARN, "invalid fd str %s.", fdStr.c_str());
            continue;
        }
        fds->push_back(fd);
    }
    return true;
}

string LocalProcessLauncher::GetCwd(pid_t pid) {
    std::string strPid = StringUtil::ToString(pid);
    std::string cwdPath = string("/proc/") + strPid + "/cwd";
    char buf[PATH_MAX];
    ssize_t len = readlink(cwdPath.c_str(), buf, PATH_MAX);
    if (len > 0) {
        buf[len] = '\0';
        return buf;
    }
    return "";
}

} // namespace util
} // namespace engine
