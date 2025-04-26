//
// Created by 32002425 on 26-04-2025.
//
#include <iostream>
#include <boost/asio.hpp>
#include "NetworkSession.h"
#include "CryptoManager.h"
#include "AuthService.h"
#include "MTAService.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
    boost::asio::io_context io;
    NetworkSession net(io);
    net.connect({boost::asio::ip::address::from_string("127.0.0.1"), 9000});

    // load client priv/pub and server pub from config
    uint8_t clientPriv[32] = {/* ... */};
    auto clientPub = CryptoManager::derivePublicKey(clientPriv);
    std::vector<uint8_t> serverPub = {/* ... */};

    AuthService auth(net, clientPriv, clientPub, serverPub);
    auth.startClient("DEVICE_SERIAL_1234");

    // after auth:
    MTAService mta;
    auto shares = mta.requestAdditiveShares(/* x = your multiplicative share */);
    std::cout << "Client additive share U = " << shares[0]
              << ", V = " << shares[1] << std::endl;

    io.run();
    return 0;
}
