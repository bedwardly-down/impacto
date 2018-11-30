#pragma once

#include "spritesheet.h"
#include "text.h"

namespace Impacto {

namespace Renderer2D {

void Init();
void Shutdown();

void BeginFrame();
void EndFrame();

void DrawSprite(Sprite const& sprite, RectF const& dest,
                glm::vec4 tint = glm::vec4(1.0), float angle = 0.0f);
void DrawSprite(Sprite const& sprite, glm::vec2 topLeft,
                glm::vec4 tint = glm::vec4(1.0),
                glm::vec2 scale = glm::vec2(1.0), float angle = 0.0f);
void DrawRect(RectF const& dest, glm::vec4 color, float angle = 0.0f);

void DrawSprite3DRotated(Sprite const& sprite, RectF const& dest, float depth,
                         glm::vec2 vanishingPoint, bool stayInScreen,
                         glm::quat rot, glm::vec4 tint = glm::vec4(1.0f));
void DrawSprite3DRotated(Sprite const& sprite, glm::vec2 topLeft, float depth,
                         glm::vec2 vanishingPoint, bool stayInScreen,
                         glm::quat rot, glm::vec4 tint = glm::vec4(1.0f),
                         glm::vec2 scale = glm::vec2(1.0f));
void DrawRect3DRotated(RectF const& dest, float depth, glm::vec2 vanishingPoint,
                       bool stayInScreen, glm::quat rot, glm::vec4 color);

void DrawProcessedText(ProcessedTextGlyph* text, int length, Font* font,
                       float opacity = 1.0f, bool outlined = false,
                       bool smoothstepGlyphOpacity = true);

}  // namespace Renderer2D
}  // namespace Impacto