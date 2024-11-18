#include "engine/admin/local_admin_daemon.h"

namespace engine {
namespace admin {
LOG_SETUP(engine.admin, LocalAdminDaemon);

LocalAdminDaemon::LocalAdminDaemon() {}
LocalAdminDaemon::~LocalAdminDaemon() {}

bool LocalAdminDaemon::Init(const std::string &configPath, const std::string &workDir, const std::string &binaryPath) {
    std::unique_ptr<AdminConfig> config(AdminConfig::loadConfig(configPath));
    if (NULL == config) {
        AUTIL_LOG(WARN, "init admin config failed, config path [%s]", configPath.c_str());
        return false;
    }
    _maxStartCount = config->getMaxRestartCountInLocal();
    auto adminCount = config->getAdminCount() > 0 ? config->getAdminCount() : 1;
    string adminBinPath = fslib::util::FileUtil::joinPath(binaryPath, "/usr/local/bin/admin_bin");
    swift::util::LocalProcessLauncher procLauncher;
    for (uint32_t i = 0; i < adminCount; i++) {
        ProcessLauncherParam param;
        string adminWorkDir = fslib::util::FileUtil::joinPath(workDir, "admin_" + StringUtil::toString(i));
        vector<string> args = {"-l",
                               StringUtil::formatString("%s/usr/local/etc/swift/swift_alog.conf", binaryPath.c_str()),
                               "-c",
                               config->getConfigPath(),
                               "-w",
                               adminWorkDir,
                               "-t",
                               "20",
                               "-q",
                               "50",
                               "-i",
                               "3",
                               "--recommendPort"};
        param.args = args;
        param.binPath = adminBinPath;
        param.workDir = adminWorkDir;
        _adminProcessParam.push_back(param);
        pid_t pid = procLauncher.start(adminBinPath, args, {}, adminWorkDir);
        if (pid < 0) {
            AUTIL_LOG(INFO, "start app[%s], workDir[%s] failed", config->getApplicationId().c_str(), workDir.c_str());
            return false;
        } else {
            AUTIL_LOG(INFO, "start admin[%d %d] success, workDir[%s]", i, pid, workDir.c_str());
            _pidVec.push_back(pid);
        }
    }
    if (_adminProcessParam.size() > 0 && _adminProcessParam.size() == _pidVec.size()) {
        AUTIL_LOG(INFO, "start app [%s], workDir[%s] success.", config->getApplicationId().c_str(), workDir.c_str());
        return true;
    }
    return false;
}

bool LocalAdminDaemon::DaemonRun() {
    swift::util::LocalProcessLauncher procLauncher;
    for (size_t i = 0; i < _pidVec.size(); ++i) {
        ProcessLauncherParam &param = _adminProcessParam[i];
        auto status = procLauncher.getProcStat(_pidVec[i], param.workDir);
        if (status.isExited) {
            if (_maxStartCount > 0 && _startCount >= _maxStartCount) {
                AUTIL_LOG(WARN,
                          "start process failed, start count [%d] is large than max start count [%d]",
                          _startCount,
                          _maxStartCount);
                return false;
            }
            AUTIL_LOG(WARN, "admin[%lu %d] disappear, restart workDir[%s]", i, _pidVec[i], param.workDir.c_str());
            pid_t pid = procLauncher.start(param.binPath, param.args, param.envs, param.workDir);
            _startCount++;
            if (pid < 0) {
                AUTIL_LOG(INFO, "start admin[%lu] fail, workDir[%s]", i, param.workDir.c_str());
                continue;
            } else {
                AUTIL_LOG(INFO, "start admin[%lu %d] success, workDir[%s]", i, pid, param.workDir.c_str());
                _pidVec[i] = pid;
            }
        }
    }
    return true;
}

} // namespace admin
} // namespace engine
