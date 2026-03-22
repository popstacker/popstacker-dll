#pragma once
#include "../FuncHook.h"
#include "../../../main/commandmanager/commandmanager.h"

class SendChatMessageHook : public FuncHook {
private:
	using func_t = bool(__thiscall*)(__int64, const std::string&);
	static inline func_t oFunc = nullptr;

	static bool SendChatMessageCallback(__int64 _this, const std::string& message) {
		if (!oFunc)
			return false;

		if (!message.empty()) {
			char firstChar = message[0];
			if (firstChar == CommandManager::prefix) {
				CommandManager::execute(message);
				return false;
			}
		}

		return oFunc(_this, message);
	}

public:
	SendChatMessageHook() {
		OriginFunc = reinterpret_cast<void**>(&oFunc);
		func = reinterpret_cast<void*>(&SendChatMessageCallback);
	}
};