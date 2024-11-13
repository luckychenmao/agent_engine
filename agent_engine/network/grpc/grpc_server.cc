#include "network/grpc_server.h"

#include "util/string_util.h"

using namespace grpc;
using namespace util;

namespace network {
LOG_SETUP(network, GrpcServer);

GrpcServer::GrpcServer() {}
GrpcServer::~GrpcServer() {}

bool GrpcServer::Init(const ServerDescription &desc) {
    desc_ = desc;
    if (desc_.io_thread_num <= 0) {
        LOG(WARN, "grpc io thread num is 0");
        return false;
    }
    std::string spec = StringUtil::FormatString("0.0.0.0:%lu", desc_.port);
    builder_.AddListeningPort(spec, grpc::InsecureServerCredentials(), &listen_port_);
    builder_.SetMaxReceiveMessageSize(MaxMessageSize);
    builder_.SetMaxSendMessageSize(MaxMessageSize);
    for (size_t i = 0; i < desc_.io_thread_num; i++) {
        completion_queues_.emplace_back(builder_.AddCompletionQueue().release());
    }
    return true;
}

bool GrpcServer::RegisterService(grpc::Service *service) {
    if (!service) {
        LOG(ERROR, "register service is null");
        return false;
    }
    builder_.RegisterService(service);
    return true;
}
bool GrpcServer::Start() {
    server_ = builder_.BuildAndStart();
    if (!server_) {
        LOG(ERROR, "start grpc server failed, port[%lu], thread_num [%lu]", desc_.port, desc_.io_thread_num);
        return false;
    } else {
        for (auto &cq : completion_queues_) {
            work_threads_.push_back(
                util::Thread::CreateThread(std::bind(&GrpcServer::WorkLoop, this, cq), "grpc_server_io"));
        }
    }
    LOG(INFO, "start grpc server succ, listen port[%d]", listen_port_);
    return true;
}

void GrpcServer::Wait() {
    for (auto &thread : work_threads_) {
        thread->Join();
    }
}

void GrpcServer::Stop() {}

void GrpcServer::WorkLoop(const ServerCompletionQueuePtr &cq) {
    void *tag;
    bool ok;
    while (true) {
        if (!cq->Next(&tag, &ok)) {
            break;
        }
        if (!ok) { // Completion queue has been shut down
            break;
        }
        // // LOG(INFO, "next %s", ok ? "true" : "false" );
        // if (tag) {
        //     auto closure = static_cast<GigStreamClosureBase *>(tag);
        //     closure->run(ok);
        // } else {
        //     // shutdown alarm
        //     autil::ScopedReadWriteLock lock(cqsPtr->enqueueLock, 'w');
        //     cqsPtr->stopped = true;
        //     cqsPtr->cq->Shutdown();
        //     LOG(INFO, "cq: %p shutdown", cqsPtr->cq.get());
        // }
        LOG(DEBUG, "receive data, tag[%p]", tag);
    }
}

} // namespace network
