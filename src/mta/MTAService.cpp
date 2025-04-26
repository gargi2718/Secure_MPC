//
// Created by 32002425 on 26-04-2025.
//

// MTAService.cpp
#include "MTAService.h"
#include "mta.pb.h"

using namespace secure_mpc::mta;

std::vector<int64_t> MTAService::requestAdditiveShares(int64_t x) {
    int bitLen = 64; // assume 64-bit shares
    // 1) send your x-share
    // 2) run CoT rounds to get mci for each bit of remote y
    auto mc = runCoTRounds(x, bitLen);
    // 3) compute U = -Σ2^i * U_i  and V = Σ2^i * mc_i
    int64_t U = 0, V = 0;
    for (int i = 0; i < bitLen; i++) {
        int64_t Ui = /* stored during CoT rounds */;
        U -= (Ui << i);
        V += (mc[i] << i);
    }
    return {U, V};  // additive shares
}

std::vector<int64_t> MTAService::runCoTRounds(int64_t x, int bitLen) {
    std::vector<int64_t> mc(bitLen), Uis(bitLen);
    for (int i = 0; i < bitLen; i++) {
        // 1) Alice (sender) picks random Ui
        int64_t Ui = random_scalar();
        Uis[i] = Ui;
        // 2) prepare m0=Ui, m1=Ui+x
        // 3) perform one COT with choice bit = yi (from server)
        //    → exchange OTInit, OTReply, OTSend, OTFinish messages over net_
        //    → derive mci
        // (Implementation omitted for brevity; see COT A.3.1–A.3.3 :contentReference[oaicite:10]{index=10})
        mc[i] = /* received mc */;
    }
    return mc;
}
