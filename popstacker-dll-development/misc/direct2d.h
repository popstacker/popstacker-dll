#pragma once
#include <d3d11.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <dxgi1_4.h>
#include <dwrite_3.h>
#include <future>
#include <string>
#include <winrt/base.h>
#include "../misc/math.h"
#include "../memory/memoryutil.h"
#include "../misc/uicolor.h"

namespace Direct2D {
	extern float deltaTime;

	void NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi);
	void EndFrame();
	void Render();
	void Clean();
	void Flush();

	Vec2<float> getWindowSize();
	void drawText(const Vec2<float>& textPos, const std::string& textStr, const Color& color, float textSize = 1.f, bool storeTextLayout = false);
	void drawRectangle(const Vec2<float>& pos, const Vec2<float>& size, const Color& color, float thickness = 1.0f);
	void fillRectangle(const Vec2<float>& pos, const Vec2<float>& size, const Color& color);
	void drawCircle(const Vec2<float>& center, float radius, const Color& color, float thickness = 1.0f);
	void fillCircle(const Vec2<float>& center, float radius, const Color& color);
	float getTextWidth(const std::string& text, float textSize = 1.0f);
	float getTextHeight(const std::string& text, float textSize = 1.0f);
	bool worldToScreen(const Vec3<float>& pos, Vec2<float>& out);
};
