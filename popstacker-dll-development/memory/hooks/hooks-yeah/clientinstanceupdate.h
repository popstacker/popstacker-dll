#pragma once
#include "../funchook.h"

class ClientInstanceUpdateHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(ClientInstance*, bool);
	static inline func_t oFunc = nullptr;

	static __int64 ClientInstance_UpdateCallback(ClientInstance* _this, bool a2) {
		if (_this)
			Game::clientInstance = _this;

		if (oFunc)
			return oFunc(_this, a2);

		return 0;
	}

public:
	ClientInstanceUpdateHook() {
		OriginFunc = reinterpret_cast<void**>(&oFunc);
		func = reinterpret_cast<void*>(&ClientInstance_UpdateCallback);
	}
};