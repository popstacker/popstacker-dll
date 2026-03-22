#pragma once
#include "../memory/memoryutil.h"
#include "font.h"

class MinecraftGame {
public:
	CLASS_MEMBER(bool, mouseGrabbed, 0x1A8); // CE dissect structures
	CLASS_MEMBER(Font*, mcFont, 0xF70);	// CE dissect structures
};