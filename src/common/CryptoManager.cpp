//
// Created by 32002425 on 26-04-2025.
//
// CryptoManager.h
// CryptoManager.cpp
#include "CryptoManager.h"
extern "C" {
#include "trezor-crypto/sha2.h"
#include "trezor-crypto/secp256k1.h"
#include "trezor-crypto/ecdsa.h"
#include "trezor-crypto/rfc6979.h"
}

std::vector<uint8_t> CryptoManager::derivePublicKey(const uint8_t priv[32]) {
    uint8_t pub[65];
    size_t outlen = sizeof(pub);
    secp256k1_pubkey_t key;
    secp256k1_ec_pubkey_create(&key, priv);
    secp256k1_ec_pubkey_serialize(pub, &outlen, &key, SECP256K1_EC_UNCOMPRESSED);
    return {pub, pub + outlen};
}

std::vector<uint8_t> CryptoManager::sha256(const std::vector<uint8_t>& data) {
    uint8_t digest[32];
    sha256_Raw(data.data(), data.size(), digest);
    return {digest, digest + 32};
}

std::vector<uint8_t> CryptoManager::sign(const uint8_t priv[32], const std::vector<uint8_t>& hash) {
    uint8_t sig[64];
    // RFC6979 Deterministic k
    ecdsa_sign_digest_rfc6979(secp256k1, priv, hash.data(), sig);
    return {sig, sig + 64};
}

bool CryptoManager::verify(const std::vector<uint8_t>& pub,
                           const std::vector<uint8_t>& hash,
                           const std::vector<uint8_t>& sig) {
    secp256k1_pubkey_t key;
    secp256k1_ec_pubkey_parse(&key, pub.data(), pub.size());
    return ecdsa_verify_digest(secp256k1, pub.data(), hash.data(), sig.data());
}

