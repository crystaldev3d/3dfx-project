#pragma once
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <windowsx.h>
#include <glide.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

#pragma warning(push)
    #pragma warning(disable: 4996)
    //#define CGLM_FORCE_DEPTH_ZERO_TO_ONE
    //#define CGLM_FORCE_RIGHT_HANDED
    #include <cglm/cglm.h>
    #include <cglm/cam.h>
#pragma warning(pop)

#define FXDEBUG 1