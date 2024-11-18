#pragma once

#include <map>
#include <memory>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <vector>

#include "util/log.h"

namespace engine {
namespace util {

struct ProcStat {
    bool is_exited = true;
    bool normal_exited = true;
    int32_t exit_code = 0;
    int32_t exit_sig = 0;

    void Reset() noexcept {
        is_exited = true;
        normal_exited = true;
        exit_code = 0;
        exit_sig = 0;
    }
};

class LocalProcessLauncher {
public:
    enum class ExitStatus {
        NotExited,
        NormalExited,
        SigExited,
        SigStopped
    };
    using KeyValueMap = std::map<std::string, std::string>;

public:
    LocalProcessLauncher();
    ~LocalProcessLauncher();
    LocalProcessLauncher(const LocalProcessLauncher &) = delete;
    LocalProcessLauncher &operator=(const LocalProcessLauncher &) = delete;

public:
    pid_t Start(const std::string &path,
                const std::vector<std::string> &args,
                const KeyValueMap &envs,
                const std::string &workDir);

    bool Stop(pid_t pid, int32_t sig);

    ProcStat GetProcStat(pid_t pid, const std::string &workDir);

    int32_t KillProcess(pid_t pid, int32_t signal);

    bool IsMyProcess(pid_t pid, const std::string &workDir);

    bool HasPorcessPidFile(pid_t pid);

private:
    bool CreateWorkDir(const std::string &dir);
    std::shared_ptr<char *> CreateCmd(const std::string &path, const std::vector<std::string> &args);
    bool SetEnv(const KeyValueMap &envs);

    LocalProcessLauncher::ExitStatus ParseExitStatus(pid_t pid, int status, int32_t *exitCode, int32_t *exitSig);

    void PrintArgsAndEnvs(const std::string &path, const std::vector<std::string> &args, const KeyValueMap &envs);

    bool GetAllFds(std::vector<int32_t> *fds);
    bool CloseFds();
    bool GenerateProcSignature(pid_t pid, std::string *signature);
    bool GetStartTime(pid_t pid, std::string *startTime);
    bool ProcessDirExist(pid_t pid);
    std::string GetCwd(pid_t pid);

private:
    LOG_DECLARE();
};

} // namespace util
} // namespace engine
