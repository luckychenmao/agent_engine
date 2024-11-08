#include <iostream>

#include "network/grpc_server.h"
#include "util/log.h"
#include "util/scope.h"

static const std::string FileLogConfig = R"EOF(
alog.rootLogger=INFO, defaultAppender
alog.max_msg_len=65535
alog.logger.swift.WorkerRunner=INFO, WorkerRunnerAppender
alog.appender.defaultAppender=FileAppender
alog.appender.defaultAppender.fileName=grpc_server_demo.log
alog.appender.defaultAppender.layout=PatternLayout
alog.appender.defaultAppender.layout.LogPattern=[%%d] [%%l] [%%t,%%F -- %%f():%%n] [%%m]
alog.appender.defaultAppender.cache_limit=128
alog.appender.defaultAppender.compress=true
alog.appender.defaultAppender.max_file_size=1024
alog.appender.defaultAppender.log_keep_count=300
alog.appender.defaultAppender.async_flush=false
alog.appender.defaultAppender.flush_threshold=1024
alog.appender.defaultAppender.flush_interval=1000
alog.appender.WorkerRunnerAppender=ConsoleAppender
alog.appender.WorkerRunnerAppender.layout=PatternLayout
alog.appender.WorkerRunnerAppender.layout.LogPattern=[%%d] [%%l] [%%t,%%F -- %%f():%%n] [%%m]

)EOF";

int32_t main(int argc, char **argv) {
    util::ScopeGuard _([] {
        LOG_FLUSH();
        LOG_SHUTDOWN();
    });
    DECLARE_AND_SETUP_LOGGER(grpc_demo, grpc_demo);
    LOG_CONFIG_FROM_STRING(FileLogConfig.c_str());
    network::ServerDescription desc;
    desc.port = 33419;
    network::GrpcServer server;
    auto succ = server.Init(desc);
    if (!succ) {
        LOG(ERROR, "grpc server init failed.");
        return -1;
    }
    LOG(INFO, "grpc server init success.");
    server.Wait();
    return 0;
}
