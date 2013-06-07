#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <Windows.h>

#define IsKeyDown(vk_code)         (GetAsyncKeyState(vk_code) & 0x8000)
#define NonAsyncKeyDown(vk_code) (GetKeyState(vk_code) & 0x8000)
#define VK_A 0x41
#define VK_D 0x44
#define VK_L 0x4C
#define VK_N 0x4E
#define VK_Q 0x51
#define VK_S 0x53
#define VK_W 0x57
#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75

#endif