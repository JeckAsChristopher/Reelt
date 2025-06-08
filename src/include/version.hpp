#ifndef REELT_VERSION_HPP
#define REELT_VERSION_HPP

#include <string>

#define REELT_VERSION     "v0.7"
#define REELT_BUILD_DATE  __DATE__
#define REELT_AUTHOR      "Jeck Christopher Anog"
#define REELT_HASH        "631eea"

std::string getReeltVersion();
bool verifyVersionIntegrity();
std::string getSecureVersion();

#endif
