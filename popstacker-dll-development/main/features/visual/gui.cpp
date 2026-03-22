#include "gui.h"
#include <random>
#include <cmath>
#include "../../events/eventmanager.h"

GUI::ClickWindow::ClickWindow(std::string windowName, Vec2<float> startPos, FeatureCategory c) {
    this->name = windowName;
    this->category = c;
    this->pos = startPos;
    this->extended = true;

    for (auto& feature : EventManager::get().getFeatures()) {
        if (feature->getCategory() == c && feature != EventManager::get().getFeature<GUI>()) {
            this->featureList.push_back(feature);
        }
    }

    std::sort(this->featureList.begin(), this->featureList.end(), [](FeatureBase* lhs, FeatureBase* rhs) {
        return lhs->getName() < rhs->getName();
        });
}

GUI::GUI() : FeatureBase("GUI", FeatureCategory::Visual, VK_INSERT) {
}

GUI::~GUI() {
    for (auto& window : windowList) {
        delete window;
    }
    windowList.clear();
}

void GUI::updateSelectedAnimRect(const Vec4<float>& rect, float& anim) {
    bool shouldUp = (mousePos.x >= rect.x && mousePos.x <= rect.z &&
        mousePos.y >= rect.y && mousePos.y <= rect.w);

    if (draggingWindowPtr != nullptr)
        shouldUp = false;

    if (shouldUp)
        anim = 1.f;
    else
        anim -= Direct2D::deltaTime * 2.f;

    if (anim > 1.f)
        anim = 1.f;
    if (anim < 0.f)
        anim = 0.f;
}

void GUI::initialize() {
    setEnabled(false);

    Vec2<float> startPos = Vec2<float>(175.f, 75.f);
    windowList.push_back(new ClickWindow("Movement", startPos, FeatureCategory::Movement));
    startPos.x += 210.f;
    windowList.push_back(new ClickWindow("Combat", startPos, FeatureCategory::Combat));
    startPos.x += 210.f;
    windowList.push_back(new ClickWindow("Render", startPos, FeatureCategory::Render));
    startPos.x += 210.f;
    windowList.push_back(new ClickWindow("World", startPos, FeatureCategory::World));
    startPos.x += 210.f;
    windowList.push_back(new ClickWindow("Visual", startPos, FeatureCategory::Visual));
    startPos.x += 210.f;
    windowList.push_back(new ClickWindow("Misc", startPos, FeatureCategory::Misc));
    startPos.x += 210.f;
    windowList.push_back(new ClickWindow("Client", startPos, FeatureCategory::Client));

    initialized = true;
}

void GUI::onEnable() {
    if (Game::getClientInstance()) {
        Game::getClientInstance()->releasebMouse();
    }
    openAnim = 0.0f;

    // Reset all interaction states
    isLeftClickDown = false;
    isRightClickDown = false;
    isHoldingLeftClick = false;
    isHoldingRightClick = false;
    draggingWindowPtr = nullptr;
    capturingKeybindFeaturePtr = nullptr;
}

void GUI::onDisable() {
    if (Game::getClientInstance()) {
        Game::getClientInstance()->grabMouse();
    }
    openAnim = 0.0f;

    // Reset all interaction states
    isLeftClickDown = false;
    isRightClickDown = false;
    isHoldingLeftClick = false;
    isHoldingRightClick = false;
    draggingWindowPtr = nullptr;
    capturingKeybindFeaturePtr = nullptr;
}

void GUI::onKeyPress(uint64_t key, bool isDown) {
    if (!isEnabled()) return;
    if (!isDown) return;

    if (key < 192) {  // Regular keys only
        if (capturingKeybindFeaturePtr != nullptr) {
            if (key != VK_ESCAPE) {
                capturingKeybindFeaturePtr->setKeybind(key);
            }
            capturingKeybindFeaturePtr = nullptr;
            return;
        }
    }
}

void GUI::onMouseInput(MouseButton button, bool isDown, const Vec2<float>& pos, char mouseButton) {
    if (!isEnabled()) return;

    mousePos = pos;

    switch (button) {
    case MouseButton::Left:
        isLeftClickDown = isDown;
        isHoldingLeftClick = isDown;
        break;
    case MouseButton::Right:
        isRightClickDown = isDown;
        isHoldingRightClick = isDown;
        break;
    default:
        break;
    }

    if (draggingWindowPtr != nullptr) {
        if (!isHoldingLeftClick)
            draggingWindowPtr = nullptr;
    }

    if (capturingKeybindFeaturePtr != nullptr) {
        if (isRightClickDown) {
            capturingKeybindFeaturePtr->setKeybind(0);
            capturingKeybindFeaturePtr = nullptr;
            isRightClickDown = false;
        }
    }
}

void GUI::renderParticles() {
    static std::vector<StarParticle> particles;
    static float spawnTimer = 0.f;
    static std::mt19937 rng(std::random_device{}());

    Vec2<float> screenSize = Game::clientInstance->guiData->windowSizeReal;
    float dt = Direct2D::deltaTime;

    std::uniform_real_distribution<float> spawnDist(0.05f, 0.4f);
    std::uniform_real_distribution<float> sizeDist(1.5f, 5.5f);
    std::uniform_real_distribution<float> speedDist(80.f, 320.f);
    std::uniform_real_distribution<float> startXDist(screenSize.x * 0.2f, screenSize.x * 1.2f);
    std::uniform_real_distribution<float> startYDist(-50.f, screenSize.y * 0.4f);
    std::uniform_real_distribution<float> angleDist(-0.15f, 0.15f);

    spawnTimer -= dt;
    if (spawnTimer <= 0.f && particles.size() < 180) {
        spawnTimer = spawnDist(rng);

        StarParticle p;
        p.pos.x = startXDist(rng);
        p.pos.y = startYDist(rng);
        p.size = sizeDist(rng);
        p.speed = speedDist(rng);
        p.angleOffset = angleDist(rng);
        p.alpha = 0.f;
        p.life = 0.f;
        p.maxLife = (screenSize.x + screenSize.y) / p.speed + 2.f;
        p.fadeIn = 0.3f;
        p.fadeOut = 0.4f;

        particles.push_back(p);
    }

    for (auto& p : particles) {
        p.life += dt;

        float moveDist = p.speed * dt;
        p.pos.x -= moveDist * (0.707f + p.angleOffset);
        p.pos.y += moveDist * (0.707f - p.angleOffset);

        if (p.life < p.fadeIn) {
            p.alpha = p.life / p.fadeIn;
        }
        else if (p.life > p.maxLife - p.fadeOut) {
            p.alpha = (p.maxLife - p.life) / p.fadeOut;
        }
        else {
            p.alpha = 1.f;
        }

        p.alpha = std::clamp(p.alpha, 0.f, 1.f);

        if (p.alpha > 0.01f) {
            uint8_t a = static_cast<uint8_t>(p.alpha * 200.f);
            Direct2D::fillCircle(p.pos, p.size, Color(255, 255, 255, a));
        }
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [&](const StarParticle& p) {
                return p.life >= p.maxLife ||
                    p.pos.x < -100.f ||
                    p.pos.y > screenSize.y + 100.f;
            }),
        particles.end()
    );
}

void GUI::onDirect2DRender() {
    if (!initialized || !isEnabled()) return;

    static Vec2<float> oldMousePos = mousePos;
    mouseDelta = mousePos - oldMousePos;
    oldMousePos = mousePos;

    Vec2<float> screenSize = Game::getClientInstance()->getguiData()->windowSizeReal;
    float deltaTime = Direct2D::deltaTime;

    float textSize = 1.f;
    float textPaddingX = 3.f * textSize;
    float textPaddingY = 1.f * textSize;
    float textHeight = Direct2D::getTextHeight("A", textSize) + 2.f;
    float windowWidth = 202.f + (textPaddingX * 2.f);

    std::string descriptionText = "NULL";

    openAnim += deltaTime * 2.f;
    if (openAnim > 1.f)
        openAnim = 1.f;

    // Background darkening
    Direct2D::fillRectangle(Vec2<float>(0.f, 0.f), screenSize, Color(0, 0, 0, (int)(110 * openAnim)));
    renderParticles();

    // Draw windows
    for (auto& window : windowList) {
        if (window == draggingWindowPtr) {
            window->pos = window->pos + mouseDelta;
        }

        Vec4<float> headerRect = Vec4<float>(
            window->pos.x,
            window->pos.y,
            window->pos.x + windowWidth,
            window->pos.y + textHeight + (textPaddingY * 2.f)
        );

        Vec2<float> headerTextPos = Vec2<float>(
            headerRect.x + (headerRect.z - headerRect.x - Direct2D::getTextWidth(window->name, textSize)) / 2.f,
            headerRect.y + (headerRect.w - headerRect.y - Direct2D::getTextHeight(window->name, textSize)) / 2.f
        );

        // Check header interaction
        if (mousePos.x >= headerRect.x && mousePos.x <= headerRect.z &&
            mousePos.y >= headerRect.y && mousePos.y <= headerRect.w) {
            if (isLeftClickDown) {
                draggingWindowPtr = window;
                isLeftClickDown = false;
            }
            else if (isRightClickDown) {
                window->extended = !window->extended;
                isRightClickDown = false;
            }
        }

        updateSelectedAnimRect(headerRect, window->selectedAnim);

        // Draw header
        Direct2D::fillRectangle(Vec2<float>(headerRect.x, headerRect.y),
            Vec2<float>(headerRect.z - headerRect.x, headerRect.w - headerRect.y),
            mainColor);
        Direct2D::drawText(headerTextPos, window->name, Color(255, 255, 255), textSize);
        Direct2D::fillRectangle(Vec2<float>(headerRect.x, headerRect.y),
            Vec2<float>(headerRect.z - headerRect.x, headerRect.w - headerRect.y),
            Color(255, 255, 255, (int)(45 * window->selectedAnim)));

        if (window->extended) {
            float moduleSpace = 2.f * textSize;
            float yOffset = headerRect.w + moduleSpace;
            float totalHeight = 0;

            // Calculate total height first
            for (auto& feature : window->featureList) {
                totalHeight += textHeight + (textPaddingY * 2.f) + moduleSpace;
            }

            // Draw window background
            float bgPaddingX = 2.f * textSize;
            Vec2<float> bgPos = Vec2<float>(headerRect.x + bgPaddingX, headerRect.w);
            Vec2<float> bgSize = Vec2<float>(windowWidth - (bgPaddingX * 2.f), totalHeight);
            Direct2D::fillRectangle(bgPos, bgSize, Color(0, 0, 0, 145));

            // Draw modules
            for (auto& feature : window->featureList) {
                float modPaddingX = bgPaddingX + (2.f * textSize);
                Vec4<float> moduleRect = Vec4<float>(
                    headerRect.x + modPaddingX,
                    yOffset,
                    headerRect.z - modPaddingX,
                    yOffset + textHeight + (textPaddingY * 2.f)
                );

                Vec2<float> moduleTextPos = Vec2<float>(
                    moduleRect.x + (moduleRect.z - moduleRect.x - Direct2D::getTextWidth(feature->getName(), textSize)) / 2.f,
                    moduleRect.y + (moduleRect.w - moduleRect.y - Direct2D::getTextHeight(feature->getName(), textSize)) / 2.f
                );

                // Check module interaction
                if (mousePos.x >= moduleRect.x && mousePos.x <= moduleRect.z &&
                    mousePos.y >= moduleRect.y && mousePos.y <= moduleRect.w) {
                    descriptionText = feature->getName();
                    if (isLeftClickDown) {
                        feature->toggle();
                        isLeftClickDown = false;
                    }
                }

                updateSelectedAnimRect(moduleRect, feature->selectedAnim);

                // Draw module background
                Color bgColor = feature->isEnabled() ? mainColor : Color(32, 32, 32, 200);
                Direct2D::fillRectangle(Vec2<float>(moduleRect.x, moduleRect.y),
                    Vec2<float>(moduleRect.z - moduleRect.x, moduleRect.w - moduleRect.y),
                    bgColor);
                Direct2D::drawText(moduleTextPos, feature->getName(),
                    feature->isEnabled() ? Color(255, 255, 255) : Color(175, 175, 175), textSize);
                Direct2D::fillRectangle(Vec2<float>(moduleRect.x, moduleRect.y),
                    Vec2<float>(moduleRect.z - moduleRect.x, moduleRect.w - moduleRect.y),
                    Color(255, 255, 255, (int)(45 * feature->selectedAnim)));

                yOffset += textHeight + (textPaddingY * 2.f) + moduleSpace;
            }
        }
    }

    // Draw description tooltip
    if (showDescription && descriptionText != "NULL" && draggingWindowPtr == nullptr) {
        float tooltipTextSize = 1.f;
        float textWidth = Direct2D::getTextWidth(descriptionText, tooltipTextSize);
        float textHeightDesc = Direct2D::getTextHeight(descriptionText, tooltipTextSize);
        float padding = 4.f;

        Vec2<float> tooltipPos = Vec2<float>(mousePos.x + 15.f, mousePos.y + 15.f);
        Vec2<float> tooltipSize = Vec2<float>(textWidth + padding * 2.f, textHeightDesc + padding);

        Direct2D::fillRectangle(tooltipPos, tooltipSize, Color(0, 0, 0, 180));
        Direct2D::drawText(Vec2<float>(tooltipPos.x + padding, tooltipPos.y + padding / 2.f),
            descriptionText, Color(255, 255, 255), tooltipTextSize);
    }

    isLeftClickDown = false;
    isRightClickDown = false;
}