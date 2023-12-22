#pragma once
#include "super_header.h"

#define LOG(text) std::cout << text << std::endl;
#define LOGn(text, n) std::cout << text << " " << n << std::endl;
#define LOGns(text, n, text2) std::cout << text << " " << n << " " << text2 << std::endl;

int CheckDeviceInfo();
int CheckForSupportedResolutions(bool withZBuffer = true);