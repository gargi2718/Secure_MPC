//
// Created by 32002425 on 26-04-2025.
//

#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H
// CryptoManager.h
#pragma once
#include <vector>
#include <cstdint>

class CryptoManager {
public:
    // Derive public key from 32-byte privkey
    static std::vector<uint8_t> derivePublicKey(const uint8_t priv[32]);

    // SHA-256 hash
    static std::vector<uint8_t> sha256(const std::vector<uint8_t>& data);

    // ECDSA sign (RFC6979 deterministic)
    static std::vector<uint8_t> sign(const uint8_t priv[32], const std::vector<uint8_t>& hash);

    // ECDSA verify
    static bool verify(const std::vector<uint8_t>& pub,
                       const std::vector<uint8_t>& hash,
                       const std::vector<uint8_t>& sig);
};

#endif //CRYPTOMANAGER_H
