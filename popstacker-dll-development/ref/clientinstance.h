#pragma once
#include "../memory/memoryutil.h"
#include "guidata.h"
#include "minecraft.h"
#include "minecraftgame.h"
#include "actor.h"

class ClientInstance {
public:
	CLASS_MEMBER(MinecraftGame*, mcGame, 0xC8);
	CLASS_MEMBER(Minecraft*, minecraft, 0xD0);
	CLASS_MEMBER(GuiData*, guiData, 0x558);	
public:
	LocalPlayer* getLocalPlayer() {
		return MemoryUtil::CallVFunc<29, LocalPlayer*>(this);
	}

	void grabMouse() {
		MemoryUtil::CallVFunc<332, void>(this);
	}

	void releasebMouse() {
		MemoryUtil::CallVFunc<333, void>(this);
	}
};
