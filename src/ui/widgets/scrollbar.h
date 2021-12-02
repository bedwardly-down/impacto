#pragma once

#include <functional>

#include "../widget.h"
#include "../../spritesheet.h"
#include "../../text.h"

namespace Impacto {
namespace UI {
namespace Widgets {

enum ScrollbarDirection { SBDIR_VERTICAL, SBDIR_HORIZONTAL };

class Scrollbar : public Widget {
 public:
  Scrollbar(int id, glm::vec2 pos, float min, float max, float* value,
            ScrollbarDirection dir, Sprite const& track, Sprite const& thumb,
            glm::vec2 thumbOffset = glm::vec2(0.0f, 0.0f));
  Scrollbar(int id, glm::vec2 pos, float min, float max, float* value,
            ScrollbarDirection dir, Sprite const& track, Sprite const& thumb,
            Sprite const& fill, glm::vec2 thumbOffset = glm::vec2(0.0f, 0.0f));
  void UpdateInput();
  virtual void Render();

  void Move(glm::vec2 relativePosition) override;
  void MoveTo(glm::vec2 pos) override;

  int Id;
  ScrollbarDirection Direction;
  Sprite TrackSprite;
  Sprite ThumbSprite;
  Sprite FillSprite;
  float Length;
  float MinValue;
  float MaxValue;
  float* Value;

 protected:
  float Step = 0.0f;
  float TrackProgress = 0.0f;
  glm::vec2 ThumbPosition;
  glm::vec2 ThumbSpriteOffset = glm::vec2(0.0f, 0.0f);
  RectF TrackBounds;
  RectF ThumbBounds;
  bool Scrolling = false;
  bool HasFill = false;
};

}  // namespace Widgets
}  // namespace UI
}  // namespace Impacto