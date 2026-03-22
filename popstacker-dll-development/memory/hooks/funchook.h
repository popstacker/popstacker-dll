#pragma once
#include <string>
#include <memory>

#include "../../main/events/eventmanager.h"
#include "../../memory/memoryutil.h"
#include "../../lib/minhook/MinHook.h"
#include "../../ref/game.h"

class FuncHook {
public:
	const char* name;
	uintptr_t address;
	void* OriginFunc;
	void* func;
	bool enable = false;

	virtual bool enableHook();
	virtual void onHookRequest();
};
