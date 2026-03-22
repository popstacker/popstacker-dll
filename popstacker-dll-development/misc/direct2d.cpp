#include "Direct2D.h"
#include <unordered_map>
#include <winrt/base.h>
#include <codecvt>
#include <locale>
#include <mutex>
#include "../lib/fonts/mojangles.h"
#include "../main/events/eventmanager.h"

float Direct2D::deltaTime = 0.016f;

static ID2D1Factory3* d2dFactory = nullptr;
static IDWriteFactory* d2dWriteFactory = nullptr;
static ID2D1Device2* d2dDevice = nullptr;
static ID2D1DeviceContext2* d2dDeviceContext = nullptr;
static ID2D1Bitmap1* sourceBitmap = nullptr;

static std::unordered_map<float, winrt::com_ptr<IDWriteTextFormat>> textFormatCache;
static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutCache;
static std::unordered_map<uint32_t, winrt::com_ptr<ID2D1SolidColorBrush>> colorBrushCache;
static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutTemporary;

static int currentD2DFontSize = 20;
static std::string currentD2DFont = "Mojangles";
static bool isFontItalic = false;
static bool compactMojanglesSpace = false;

static bool initD2D = false;
static winrt::com_ptr<IDWriteFactory5> d2dWriteFactory5;
static winrt::com_ptr<IDWriteInMemoryFontFileLoader> d2dInMemoryFontLoader;
static winrt::com_ptr<IDWriteFontCollection1> d2dEmbeddedFontCollection;
static bool d2dInMemoryLoaderRegistered = false;

struct EmbeddedFontData {
    const void* data;
    UINT32 size;
};

static const EmbeddedFontData gEmbeddedFonts[] = {
    { Mojangles, static_cast<UINT32>(sizeof(Mojangles)) }
};

template<typename T>
void SafeRelease(T*& ptr) {
    if (ptr != nullptr) {
        ptr->Release();
        ptr = nullptr;
    }
}

std::wstring to_wide(const std::string& str);
uint64_t getTextLayoutKey(const std::string& textStr, float textSize);
IDWriteTextFormat* getTextFormat(float textSize);
IDWriteTextLayout* getTextLayout(const std::string& textStr, float textSize, bool storeTextLayout = true);
ID2D1SolidColorBrush* getSolidColorBrush(const Color& color);
HRESULT ensureEmbeddedFontCollection();
std::wstring resolveSelectedFontFamily(const std::string& selectedFont);
bool embeddedCollectionHasFamily(const std::wstring& familyName);
void applyMojanglesSpaceCompaction(IDWriteTextLayout* textLayout, const std::wstring& text, float textSize);

void Direct2D::NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi) {
    if (!initD2D) {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &d2dFactory);
        DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(d2dWriteFactory), reinterpret_cast<IUnknown**>(&d2dWriteFactory));
        ensureEmbeddedFontCollection();

        IDXGIDevice* dxgiDevice;
        d3d11Device->QueryInterface<IDXGIDevice>(&dxgiDevice);
        d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
        dxgiDevice->Release();

        d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);

        IDXGISurface* dxgiBackBuffer = nullptr;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
        D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            fxdpi, fxdpi);
        d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &sourceBitmap);
        dxgiBackBuffer->Release();

        d2dDeviceContext->SetTarget(sourceBitmap);
        initD2D = true;
    }
    d2dDeviceContext->BeginDraw();
}

void Direct2D::EndFrame() {
    if (!initD2D) return;
    d2dDeviceContext->EndDraw();
    textLayoutTemporary.clear();
}

void Direct2D::Render() {
    EventManager::get().onDirect2DRender();

    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (!localPlayer) return;

    GuiData* gui = Game::getClientInstance()->getguiData();
    if (!gui) return;

    float y = 1.0f;
    float lineHeight = 0.0f;
    float padding = 1.0f;
    float rightMargin = 1.0f;
    float textSize = 1.0f;

    for (auto& feature : EventManager::get().getFeatures()) {
        if (!feature->isEnabled()) continue;

        std::string text = feature->getName();
        float textWidth = getTextWidth(text, textSize);
        float textHeight = getTextHeight(text, textSize);

        lineHeight = textHeight + padding;
        float x = gui->windowSizeReal.x - textWidth - rightMargin;

        drawText({ x, y }, text, { 255, 255, 255, 255 }, textSize);
        y += lineHeight;
    }
}

void Direct2D::Clean() {
    if (!initD2D) return;

    SafeRelease(d2dFactory);
    if (d2dWriteFactory && d2dInMemoryLoaderRegistered && d2dInMemoryFontLoader) {
        d2dWriteFactory->UnregisterFontFileLoader(d2dInMemoryFontLoader.get());
    }
    d2dEmbeddedFontCollection = nullptr;
    d2dWriteFactory5 = nullptr;
    d2dInMemoryFontLoader = nullptr;
    d2dInMemoryLoaderRegistered = false;
    SafeRelease(d2dWriteFactory);
    SafeRelease(d2dDevice);
    SafeRelease(d2dDeviceContext);
    SafeRelease(sourceBitmap);

    textFormatCache.clear();
    textLayoutCache.clear();
    colorBrushCache.clear();
    textLayoutTemporary.clear();

    initD2D = false;
}

void Direct2D::Flush() {
    if (d2dDeviceContext)
        d2dDeviceContext->Flush();
}

Vec2<float> Direct2D::getWindowSize() {
    if (!sourceBitmap) return Vec2<float>(0, 0);
    D2D1_SIZE_U size = sourceBitmap->GetPixelSize();
    return Vec2<float>((float)size.width, (float)size.height);
}

void Direct2D::drawText(const Vec2<float>& textPos, const std::string& textStr, const Color& color, float textSize, bool storeTextLayout) {
    if (!d2dDeviceContext) return;

    IDWriteTextLayout* textLayout = getTextLayout(textStr, textSize, storeTextLayout);
    if (!textLayout) return;

    ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
    if (!colorBrush) return;

    D2D1_TEXT_ANTIALIAS_MODE prevMode = d2dDeviceContext->GetTextAntialiasMode();
    d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
    d2dDeviceContext->DrawTextLayout(D2D1::Point2F(textPos.x, textPos.y), textLayout, colorBrush);
    d2dDeviceContext->SetTextAntialiasMode(prevMode);
}

float Direct2D::getTextWidth(const std::string& text, float textSize) {
    if (!d2dDeviceContext) return 0.0f;

    IDWriteTextLayout* textLayout = getTextLayout(text, textSize, false);
    if (!textLayout) return 0.0f;

    DWRITE_TEXT_METRICS metrics{};
    textLayout->GetMetrics(&metrics);
    return metrics.widthIncludingTrailingWhitespace;
}

float Direct2D::getTextHeight(const std::string& text, float textSize) {
    if (!d2dDeviceContext) return 0.0f;

    IDWriteTextLayout* textLayout = getTextLayout(text, textSize, false);
    if (!textLayout) return 0.0f;

    DWRITE_TEXT_METRICS metrics{};
    textLayout->GetMetrics(&metrics);
    return metrics.height;
}

bool Direct2D::worldToScreen(const Vec3<float>& pos, Vec2<float>& out) {
    return false;
}

std::wstring to_wide(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

uint64_t getTextLayoutKey(const std::string& textStr, float textSize) {
    std::hash<std::string> textHash;
    std::hash<float> textSizeHash;
    return textHash(textStr) ^ textSizeHash(textSize);
}

HRESULT ensureEmbeddedFontCollection() {
    if (d2dEmbeddedFontCollection) return S_OK;
    if (!d2dWriteFactory) return E_POINTER;

    HRESULT hr = d2dWriteFactory->QueryInterface(__uuidof(IDWriteFactory5), reinterpret_cast<void**>(d2dWriteFactory5.put_void()));
    if (FAILED(hr)) return hr;

    hr = d2dWriteFactory5->CreateInMemoryFontFileLoader(d2dInMemoryFontLoader.put());
    if (FAILED(hr)) return hr;

    hr = d2dWriteFactory->RegisterFontFileLoader(d2dInMemoryFontLoader.get());
    if (SUCCEEDED(hr)) {
        d2dInMemoryLoaderRegistered = true;
    }
    else if (hr != DWRITE_E_ALREADYREGISTERED) {
        return hr;
    }

    winrt::com_ptr<IDWriteFontSetBuilder1> fontSetBuilder;
    hr = d2dWriteFactory5->CreateFontSetBuilder(fontSetBuilder.put());
    if (FAILED(hr)) return hr;

    bool hasAtLeastOneFont = false;
    for (const EmbeddedFontData& font : gEmbeddedFonts) {
        winrt::com_ptr<IDWriteFontFile> fontFile;
        hr = d2dInMemoryFontLoader->CreateInMemoryFontFileReference(
            d2dWriteFactory,
            font.data,
            font.size,
            nullptr,
            fontFile.put()
        );
        if (FAILED(hr)) continue;

        hr = fontSetBuilder->AddFontFile(fontFile.get());
        if (SUCCEEDED(hr)) hasAtLeastOneFont = true;
    }

    if (!hasAtLeastOneFont) return E_FAIL;

    winrt::com_ptr<IDWriteFontSet> fontSet;
    hr = fontSetBuilder->CreateFontSet(fontSet.put());
    if (FAILED(hr)) return hr;

    return d2dWriteFactory5->CreateFontCollectionFromFontSet(fontSet.get(), d2dEmbeddedFontCollection.put());
}

std::wstring resolveSelectedFontFamily(const std::string& selectedFont) {
    if (selectedFont == "Mojangles" || selectedFont == "Mojang") return L"Mojangles";
    return L"Mojangles";
}

bool embeddedCollectionHasFamily(const std::wstring& familyName) {
    if (!d2dEmbeddedFontCollection) return false;
    UINT32 familyIndex = 0;
    BOOL familyExists = FALSE;
    d2dEmbeddedFontCollection->FindFamilyName(familyName.c_str(), &familyIndex, &familyExists);
    return familyExists == TRUE;
}

IDWriteTextFormat* getTextFormat(float textSize) {
    if (!d2dWriteFactory) return nullptr;
    if (textFormatCache[textSize].get() == nullptr) {
        ensureEmbeddedFontCollection();
        std::wstring fontNameWide = resolveSelectedFontFamily(currentD2DFont);
        IDWriteFontCollection* fontCollection = nullptr;
        if (embeddedCollectionHasFamily(fontNameWide)) {
            fontCollection = d2dEmbeddedFontCollection.get();
        }
        else if (embeddedCollectionHasFamily(L"Arial")) {
            fontNameWide = L"Arial";
            fontCollection = d2dEmbeddedFontCollection.get();
        }
        HRESULT hr = d2dWriteFactory->CreateTextFormat(
            fontNameWide.c_str(),
            fontCollection,
            DWRITE_FONT_WEIGHT_NORMAL,
            isFontItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            (float)currentD2DFontSize * textSize,
            L"en-us",
            textFormatCache[textSize].put()
        );
        if (FAILED(hr)) {
            d2dWriteFactory->CreateTextFormat(
                L"Segoe UI",
                nullptr,
                DWRITE_FONT_WEIGHT_NORMAL,
                isFontItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                (float)currentD2DFontSize * textSize,
                L"en-us",
                textFormatCache[textSize].put()
            );
        }
    }
    return textFormatCache[textSize].get();
}

IDWriteTextLayout* getTextLayout(const std::string& textStr, float textSize, bool storeTextLayout) {
    if (!d2dWriteFactory) return nullptr;
    IDWriteTextFormat* textFormat = getTextFormat(textSize);
    if (!textFormat) return nullptr;

    std::wstring wideText = to_wide(textStr);
    uint64_t textLayoutKey = getTextLayoutKey(textStr, textSize);

    auto& cache = storeTextLayout ? textLayoutCache : textLayoutTemporary;
    if (cache[textLayoutKey].get() == nullptr) {
        d2dWriteFactory->CreateTextLayout(
            wideText.c_str(),
            (UINT32)wideText.length(),
            textFormat,
            FLT_MAX,
            0.f,
            cache[textLayoutKey].put()
        );
        applyMojanglesSpaceCompaction(cache[textLayoutKey].get(), wideText, textSize);
    }
    return cache[textLayoutKey].get();
}

void applyMojanglesSpaceCompaction(IDWriteTextLayout* textLayout, const std::wstring& text, float textSize) {
    if (!textLayout) return;
    if (!compactMojanglesSpace) return;
    if (currentD2DFont != "Mojangles" && currentD2DFont != "Mojang") return;

    winrt::com_ptr<IDWriteTextLayout1> textLayout1;
    if (FAILED(textLayout->QueryInterface(__uuidof(IDWriteTextLayout1), reinterpret_cast<void**>(textLayout1.put_void()))))
        return;

    const float spaceTightenAmount = -((float)currentD2DFontSize * textSize * 0.12f);
    for (UINT32 i = 0; i < static_cast<UINT32>(text.size()); i++) {
        if (text[i] != L' ') continue;
        DWRITE_TEXT_RANGE range = { i, 1 };
        textLayout1->SetCharacterSpacing(0.f, spaceTightenAmount, 0.f, range);
    }
}

ID2D1SolidColorBrush* getSolidColorBrush(const Color& color) {
    uint32_t colorBrushKey = (uint32_t)(color.r << 24 | color.g << 16 | color.b << 8 | color.a);
    if (colorBrushCache[colorBrushKey].get() == nullptr) {
        d2dDeviceContext->CreateSolidColorBrush(color.toD2D1Color(), colorBrushCache[colorBrushKey].put());
    }
    return colorBrushCache[colorBrushKey].get();
}

void Direct2D::drawRectangle(const Vec2<float>& pos, const Vec2<float>& size, const Color& color, float thickness) {
    if (!d2dDeviceContext) return;
    ID2D1SolidColorBrush* brush = getSolidColorBrush(color);
    if (!brush) return;

    D2D1_RECT_F rect = D2D1::RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    d2dDeviceContext->DrawRectangle(rect, brush, thickness);
}

void Direct2D::fillRectangle(const Vec2<float>& pos, const Vec2<float>& size, const Color& color) {
    if (!d2dDeviceContext) return;
    ID2D1SolidColorBrush* brush = getSolidColorBrush(color);
    if (!brush) return;

    D2D1_RECT_F rect = D2D1::RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    d2dDeviceContext->FillRectangle(&rect, brush);
}

void Direct2D::drawCircle(const Vec2<float>& center, float radius, const Color& color, float thickness) {
    if (!d2dDeviceContext) return;
    ID2D1SolidColorBrush* brush = getSolidColorBrush(color);
    if (!brush) return;

    d2dDeviceContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), radius, radius), brush, thickness);
}

void Direct2D::fillCircle(const Vec2<float>& center, float radius, const Color& color) {
    if (!d2dDeviceContext) return;
    ID2D1SolidColorBrush* brush = getSolidColorBrush(color);
    if (!brush) return;

    d2dDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), radius, radius), brush);
}