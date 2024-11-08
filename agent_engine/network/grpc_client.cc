#include "network/grpc_client.h"

#include <functional>
#include <grpc++/alarm.h>
#include <grpc++/grpc++.h>

namespace network {
LOG_SETUP(network, GrpcClient);

GrpcClient::GrpcClient(size_t thread_num) : thread_num_(thread_num) {}

GrpcClient::~GrpcClient() {
    Stop();
    shutdown_alarms_.clear();
}

bool GrpcClient::Start() {
    for (size_t i = 0; i < thread_num_; ++i) {
        auto cq = std::make_shared<grpc::CompletionQueue>();
        auto status = std::make_shared<CompletionQueueStatus>(std::move(cq));
        completion_queues_.emplace_back(status);
        auto thread = util::Thread::CreateThread(std::bind(&GrpcClient::WorkLoop, this, i, status), "grpc_client_io");
        work_threads_.push_back(std::move(thread));
    }
    LOG(INFO, "grpc client [%p] started, threads [%lu]", this, thread_num_);
    return true;
}

void GrpcClient::Stop() {
    for (const auto &cqs : completion_queues_) {
        std::unique_lock _(cqs->enqueue_lock);
        cqs->stopped = true;
    }
    for (const auto &cqs : completion_queues_) {
        auto alarm = std::make_unique<grpc::Alarm>(cqs->cq.get(), gpr_now(GPR_CLOCK_MONOTONIC), nullptr);
        shutdown_alarms_.push_back(std::move(alarm));
    }
    completion_queues_.clear();
    work_threads_.clear();
    LOG(INFO, "grpc client [%p] stopped", this);
}

const CompletionQueueStatusPtr &GrpcClient::GetCompletionQueue(size_t id) {
    size_t index = id % completion_queues_.size();
    return completion_queues_[index];
}

void GrpcClient::WorkLoop(size_t idx, CompletionQueueStatusPtr status) {
    auto &cq = *status->cq;
    LOG(INFO, "client work loop started, worker [%p] idx [%lu] cq [%p]", this, idx, &cq);
    for (;;) {
        void *tag;
        bool ok = false;
        // LOG(INFO, "worker cq: %p", &cq);
        if (!cq.Next(&tag, &ok)) {
            break;
        }
        // LOG(INFO, "next %s", ok ? "true" : "false" );
        if (tag) {
            LOG(INFO, "vvv tag[%p]", tag);
        } else {
            // shutdown alarm
            std::unique_lock _(status->enqueue_lock);
            status->stopped = true;
            status->cq->Shutdown();
            LOG(INFO, "cq: %p shutdown", status->cq.get());
        }
    }
    LOG(INFO, "client work loop stopped, worker [%p] idx [%lu] cq [%p]", this, idx, &cq);
}

} // namespace network
