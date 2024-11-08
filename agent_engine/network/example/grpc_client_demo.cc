#include <iostream>

#include "network/grpc_connection.h"
#include "network/grpc_connection_pool.h"
#include "proto/agent_service.grpc.pb.h"
#include "util/log.h"
#include "util/scope.h"

static const std::string FileLogConfig = R"EOF(
alog.rootLogger=INFO, defaultAppender
alog.max_msg_len=65535
alog.logger.swift.WorkerRunner=INFO, WorkerRunnerAppender
alog.appender.defaultAppender=FileAppender
alog.appender.defaultAppender.fileName=grpc_client_demo.log
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
    network::GrpcConnectionPool pool;
    auto succ = pool.Init();
    if (!succ) {
        LOG(ERROR, "grpc connect pool init failed.");
        return -1;
    }
    auto connection = pool.GetConnection("0.0.0.0:33419");
    if (!connection) {
        LOG(ERROR, "get grpc connect failed.");
        return -1;
    }
    proto::ChatRequest request;
    connection->Post(request);
    while (true) {
        usleep(1000000000);
    }
    return 0;
}
