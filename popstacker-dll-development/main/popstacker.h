#pragma once
#include <Windows.h>

class popstacker {
private:
	static bool initialized;
public:
	static bool isinitialized();
	static void initialize();
	static void shutdown();
};