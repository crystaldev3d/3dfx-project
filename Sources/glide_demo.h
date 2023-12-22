#pragma once
#include "super_header.h"

int Start(HWND hwin);
void End();
int Update();
void Input(int msg, int key);
void Mouse(int button, int state, int x, int y);

extern GrContext_t context;
extern bool done;
