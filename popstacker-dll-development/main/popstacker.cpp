#include "popstacker.h"
#include "events/eventmanager.h"

bool popstacker::initialized = false;

bool popstacker::isinitialized() {
    return initialized;
}

void popstacker::initialize() {
    initialized = true;
    EventManager::get().initialize();
}

void popstacker::shutdown() {
    initialized = false;
}