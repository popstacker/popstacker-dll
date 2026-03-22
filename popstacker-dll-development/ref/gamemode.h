#pragma once
#include "../misc/math.h"
#include "../memory/memoryutil.h"
#include <functional>

class Actor;
class Player;
class InputMode;
class ItemStack;
class Block;

class GameMode {
public:
	CLASS_MEMBER(Player*, player, 0x8);
	CLASS_MEMBER(Vec3<int>, lastBreakPos, 0x10);
	CLASS_MEMBER(int32_t, lastBreakFace, 0x1C);
	CLASS_MEMBER(float, lastDestroyProgress, 0x20);
	CLASS_MEMBER(float, destroyProgress, 0x24);
public:
	virtual ~GameMode();
	virtual bool startDestroyBlock(Vec3<int> const& pos, uint8_t face, bool& hasDestroyedBlock);
	virtual bool destroyBlock(Vec3<int> const& pos, uint8_t face);
	virtual bool continueDestroyBlock(Vec3<int> const& pos, uint8_t face, Vec3<float> const& playerPos, bool& hasDestroyedBlock);
	virtual void stopDestroyBlock(Vec3<int> const& pos);
	virtual void startBuildBlock(Vec3<int> const& pos, uint8_t face);
	virtual bool buildBlock(Vec3<int> const& pos, uint8_t face, bool);
	virtual void continueBuildBlock(Vec3<int> const& pos, uint8_t face);
	virtual void stopBuildBlock();
	virtual void tick();
	virtual float getPickRange(InputMode const& currentInputMode, bool isVR);
	virtual bool useItem(ItemStack& item);
	virtual __int64 useItemOn(ItemStack& item, Vec3<int> const& at, uint8_t face, Vec3<float> const& hit, Block const* targetBlock);
	virtual bool interact(Actor* entity, Vec3<float> const& location);
	virtual bool attack(Actor* entity);
	virtual void releaseUsingItem();
	virtual void setTrialMode(bool isEnabled);
	virtual bool isInTrialMode();
	virtual void registerUpsellScreenCallback(std::function<void(bool)> callback);
};
