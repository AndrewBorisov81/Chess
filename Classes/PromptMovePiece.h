#pragma once

#include "cocos2d.h"

#include <vector>

#include "Grid.h"

enum class PromptElement
{
  GREEN_RECT,
  RED_RECT,
  GREEN_CIRCLE
};

class PromptMovePiece : public Grid
{
public:
  PromptMovePiece();
  PromptMovePiece(float cellSize, int rows, int columns);
  ~PromptMovePiece();

  virtual bool init();

  //void hideRectEnd(bool rect1 = true, bool rect2 = true, bool isMoveValide = true);

private:
  cocos2d::Sprite* m_greenRect1{ nullptr };
  cocos2d::Sprite* m_greenRect2{ nullptr };
  cocos2d::Sprite* m_redRect{ nullptr };

  std::vector<cocos2d::Sprite*> m_greenCircles;

};
