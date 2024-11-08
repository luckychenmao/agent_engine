#pragma once

#include "util/legacy/jsonizable.h"

namespace network {

class SecureConfig : public util::legacy::Jsonizable {
public:
    SecureConfig() {}
    ~SecureConfig() {}
    void Jsonize(util::legacy::Jsonizable::JsonWrapper &json);
    bool operator==(const SecureConfig &rhs) const;

public:
    std::string pem_root_certs;
    std::string pem_private_key;
    std::string pem_cert_chain;
};

struct ServerDescription {
    size_t io_thread_num = 1;
    size_t port = 0;
    SecureConfig secure_config;
};

} // namespace network
