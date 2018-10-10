#pragma once

#include "spritesheet.h"

namespace Impacto {

class Renderer2D {
 public:
  static void Init();

  Renderer2D();
  ~Renderer2D();

  void Begin();
  void DrawSprite(Sprite const& sprite, glm::vec2 topLeft,
                  glm::vec4 tint = glm::vec4(1.0),
                  glm::vec2 scale = glm::vec2(1.0));
  void Finish();

 private:
  enum Renderer2DMode { R2D_None, R2D_Sprite };

  struct VertexBufferSprites {
    glm::vec2 Position;
    glm::vec2 UV;
    glm::vec4 Tint;
  };

  static int const VertexBufferSize = 32 * 1024;
  static int const IndexBufferCount =
      VertexBufferSize / (4 * sizeof(VertexBufferSprites)) * 6;

  void Flush();

  static inline void QuadSetUV(RectF const& spriteBounds, float designWidth,
                               float designHeight, glm::vec2* bottomLeft,
                               glm::vec2* topLeft, glm::vec2* topRight,
                               glm::vec2* bottomRight);
  static inline void QuadSetPosition(RectF const& transformedQuad,
                                     glm::vec2* bottomLeft, glm::vec2* topLeft,
                                     glm::vec2* topRight,
                                     glm::vec2* bottomRight);

  GLuint VBO;
  GLuint IBO;
  GLuint VAOSprites;

  bool Drawing = false;

  GLuint CurrentTexture = 0;
  Renderer2DMode CurrentMode = R2D_None;
  uint8_t VertexBuffer[VertexBufferSize];
  int VertexBufferFill = 0;
  uint16_t IndexBuffer[IndexBufferCount];
  int IndexBufferFill = 0;
};

}  // namespace Impacto