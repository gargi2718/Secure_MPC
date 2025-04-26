//
// Created by 32002425 on 26-04-2025.
//
#include <iostream>
#include <boost/asio.hpp>
#include "NetworkSession.h"
#include "CryptoManager.h"
#include "AuthService.h"
#include "MTAService.h"

int main() {
    boost::asio::io_context io;
    NetworkSession net(io);
    net.accept(9000);

    uint8_t serverPriv[32] = {/* ... */};
    std::vector<uint8_t> clientPub = {/* ... */};
    auto serverPub = CryptoManager::derivePublicKey(serverPriv);

    AuthService auth(net, /*clientPriv*/nullptr, clientPub, serverPub);
    auth.startServer();

    // after auth:
    MTAService mta;
    mta.handleAdditiveShares(/* y = your multiplicative share */);

    io.run();
    return 0;
}
