#pragma once
#include "../memory/memoryutil.h"
#include <vector>

class Actor;

class Level {
public:
	std::vector<Actor*> getRuntimeActorList() {
		std::vector<Actor*> listOut;
		MemoryUtil::CallVFunc<278, decltype(&listOut)>(this, &listOut);
		return listOut;
	}
};