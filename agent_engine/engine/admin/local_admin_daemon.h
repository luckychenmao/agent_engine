#pragma once

#include <map>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <vector>

#include "util/log.h"

namespace engine {
namespace admin {

struct ProcessLauncherParam {
    std::string binPath;
    std::vector<std::string> args;
    std::map<std::string, std::string> envs;
    std::string workDir;
};

class LocalAdminDaemon {
public:
    LocalAdminDaemon();
    ~LocalAdminDaemon();
    LocalAdminDaemon(const LocalAdminDaemon &) = delete;
    LocalAdminDaemon &operator=(const LocalAdminDaemon &) = delete;

public:
    bool Init(const std::string &configPath, const std::string &workDir, const std::string &binaryPath);
    bool DaemonRun();

private:
    std::vector<ProcessLauncherParam> _adminProcessParam;
    std::vector<pid_t> _pidVec;
    int32_t _maxStartCount = -1;
    int32_t _startCount = 0;

private:
    LOG_DECLARE();
};

} // namespace admin
} // namespace engine
