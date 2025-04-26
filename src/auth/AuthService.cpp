//
// Created by 32002425 on 26-04-2025.

// AuthService.cpp
#include "AuthService.h"
#include "auth.pb.h"      // nanopb-generated

using namespace secure_mpc::auth;

AuthService::AuthService(NetworkSession& net,
                         const uint8_t priv[32],
                         const std::vector<uint8_t>& cpub,
                         const std::vector<uint8_t>& spub)
 : net_(net), clientPub_(cpub), serverPub_(spub) {
    std::memcpy(clientPriv_, priv, 32);
}

void AuthService::startClient(const std::string& serialID) {
  serialID_ = serialID;
  // 1) Hash serialID, sign, send AuthRequest
  AuthRequest req;
  auto hash = CryptoManager::sha256(std::vector<uint8_t>(serialID.begin(), serialID.end()));
  auto sig  = CryptoManager::sign(clientPriv_, hash);
  req.serial_hash = pb_bytes_array_t_from_vector(hash);
  req.signature   = pb_bytes_array_t_from_vector(sig);
  std::vector<uint8_t> out(req.ByteSize()); req.SerializeToArray(out.data(), out.size());
  net_.send(out);
  // wait for AuthChallenge
  net_.start([this](auto d){ onChallenge(d); });
}

void AuthService::onChallenge(const std::vector<uint8_t>& d) {
  AuthChallenge ch; ch.ParseFromArray(d.data(), d.size());
  // 4) verify server sig
  auto h = CryptoManager::sha256(std::vector<uint8_t>(ch.nonce.begin(), ch.nonce.end()));
  if (!CryptoManager::verify(serverPub_, h, ch.signature)) throw std::runtime_error("Bad server sig");
  // sign nonce and send AuthResponse
  auto sig = CryptoManager::sign(clientPriv_, h);
  AuthResponse resp;
  resp.nonce_hash = pb_bytes_array_t_from_vector(h);
  resp.signature  = pb_bytes_array_t_from_vector(sig);
  std::vector<uint8_t> out(resp.ByteSize()); resp.SerializeToArray(out.data(), out.size());
  net_.send(out);
  // session now authenticated
  std::cout << "Client authenticated." << std::endl;
}

void AuthService::startServer() {
  // 2) wait AuthRequest
  net_.start([this](auto d){ onAuthRequest(d); });
}

void AuthService::onAuthRequest(const std::vector<uint8_t>& d) {
  AuthRequest req; req.ParseFromArray(d.data(), d.size());
  // verify client sig on serial hash
  if (!CryptoManager::verify(clientPub_,
         std::vector<uint8_t>(req.serial_hash->bytes, req.serial_hash->bytes + req.serial_hash->size),
         std::vector<uint8_t>(req.signature->bytes, req.signature->bytes + req.signature->size)))
    throw std::runtime_error("Invalid client sig");
  // 3) send nonce
  std::vector<uint8_t> nonce(32);
  random_bytes(nonce.data(), nonce.size());
  auto sig = CryptoManager::sign(serverPriv_, nonce_h = CryptoManager::sha256(nonce));
  AuthChallenge ch;
  ch.nonce     = pb_bytes_array_t_from_vector(nonce);
  ch.signature = pb_bytes_array_t_from_vector(sig);
  std::vector<uint8_t> out(ch.ByteSize()); ch.SerializeToArray(out.data(), out.size());
  net_.send(out);
  // wait AuthResponse
  net_.start([this](auto d){ onResponse(d); });
}

void AuthService::onResponse(const std::vector<uint8_t>& d) {
  AuthResponse resp; resp.ParseFromArray(d.data(), d.size());
  // verify client response
  if (!CryptoManager::verify(clientPub_,
        std::vector<uint8_t>(resp.nonce_hash->bytes, resp.nonce_hash->bytes + resp.nonce_hash->size),
        std::vector<uint8_t>(resp.signature->bytes, resp.signature->bytes + resp.signature->size)))
    throw std::runtime_error("Auth failure");
  std::cout << "Server: Client verified, serial ID = " << serialID_ << std::endl;
}
