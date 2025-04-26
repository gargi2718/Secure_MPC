//
// Created by 32002425 on 26-04-2025.
//

#ifndef MTASERVICE_H
#define MTASERVICE_H


// MTAService.h
#pragma once
#include "CryptoManager.h"
#include "NetworkSession.h"
#include <vector>
#include <cstdint>

class MTAService {
public:
    // Both parties run this after auth: client holds x, server holds y
    // Client calls requestAdditiveShares(x); server calls handleAdditiveShares(y);
    std::vector<int64_t> requestAdditiveShares(int64_t x);
    void handleAdditiveShares(int64_t y);

private:
    // per-bit CoT
    std::vector<int64_t> runCoTRounds(int64_t x, int bitLen);
    NetworkSession* net_;
};



#endif //MTASERVICE_H
