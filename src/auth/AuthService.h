//
// Created by 32002425 on 26-04-2025.
//

#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H



// AuthService.h
#pragma once
#include "CryptoManager.h"
#include "NetworkSession.h"

class AuthService {
public:
    AuthService(NetworkSession& net,
                const uint8_t clientPriv[32],
                const std::vector<uint8_t>& clientPub,
                const std::vector<uint8_t>& serverPub);

    void startClient(const std::string& serialID);
    void startServer();

private:
    void onAuthRequest(const std::vector<uint8_t>& data);
    void onChallenge(const std::vector<uint8_t>& data);
    void onResponse(const std::vector<uint8_t>& data);

    NetworkSession& net_;
    uint8_t clientPriv_[32];
    std::vector<uint8_t> clientPub_, serverPub_;
    std::string serialID_;
};




#endif //AUTHSERVICE_H
