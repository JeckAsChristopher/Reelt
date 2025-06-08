// LICENSE BY MIT 2025

#include "include/version.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <openssl/sha.h>

static const std::string encoded_version = "djAuNw==";

std::string base64_decode(const std::string &in) {
    std::string out;
    std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;
    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

std::string getReeltVersion() {
    return base64_decode(encoded_version);
}

bool verifyVersionIntegrity() {
    std::string version = getReeltVersion();
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)version.c_str(), version.size(), hash);
    std::ostringstream oss;
    for (int i = 0; i < 3; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str() == REELT_HASH;
}

std::string getSecureVersion() {
    std::ostringstream oss;
    oss << getReeltVersion()
        << " build: " << REELT_BUILD_DATE
        << ", author: " << REELT_AUTHOR
        << ", hash: " << REELT_HASH;
    return oss.str();
}
