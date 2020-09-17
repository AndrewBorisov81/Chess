#pragma once

#include "cocos2d.h"

#include <array>

#include "Grid.h"

class PromptElement;

class PromptLayer : public Grid
{
public:
  PromptLayer();
  PromptLayer(float cellSize, int rows, int columns);
  ~PromptLayer();

  virtual bool init();

  void showRectPrompts(bool isToGreenRect = true);
  void setPositionRects(const cocos2d::Size& from, const cocos2d::Size& to);
  void hideRectPrompts();

  void showCirclePrompts();
  void hideCirclePrompts();

private:
  PromptElement* m_presentPromptRect{ nullptr };
  PromptElement* m_futurePromptRect{ nullptr };
  PromptElement* m_promptCircle{ nullptr };

  std::array<PromptElement*, 27> m_circles;
};

