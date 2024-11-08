#include "network/server_config.h"

namespace network {

void SecureConfig::Jsonize(util::legacy::Jsonizable::JsonWrapper &json) {
    json.Jsonize("pem_root_certs", pem_root_certs, pem_root_certs);
    json.Jsonize("pem_private_key", pem_private_key, pem_private_key);
    json.Jsonize("pem_cert_chain", pem_cert_chain, pem_cert_chain);
}

bool SecureConfig::operator==(const SecureConfig &rhs) const {
    return pem_root_certs == rhs.pem_root_certs && pem_private_key == rhs.pem_private_key &&
           pem_cert_chain == rhs.pem_cert_chain;
}

} // namespace network
