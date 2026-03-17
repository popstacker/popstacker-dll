#include "Example.h"
#include <Windows.h>

Example::Example()
    : FeatureBase("example", FeatureCategory::Client) {
}

void Example::onDLLTick() {
    OutputDebugStringA("example tick\n");
}