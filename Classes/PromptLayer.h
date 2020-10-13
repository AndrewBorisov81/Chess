#pragma once

#include "cocos2d.h"

#include <vector>
#include <array>
#include <functional>

#include "Grid.h"

class PromptElement;
class PromptLogicHelper;

struct EnPassant;
struct Castling;
struct Promotion;

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

  void showCirclePrompts(const std::vector<cocos2d::Size>& valideMoves);
  void hideCirclePrompts();
  void setPositionCircles(const std::vector<cocos2d::Size>& valideMoves);

  // Call backs
  void getValideMoves(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves);
  void callBackIsMoveValide(const std::function<bool(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> isMoveValide);

private:
  PromptElement* m_presentPromptRect{ nullptr };
  PromptElement* m_futurePromptRect{ nullptr };
  PromptElement* m_promptCircle{ nullptr };

  std::array<PromptElement*, 27> m_circles;

  PromptLogicHelper* m_promptLogicHelper{ nullptr };

  // Call backs
  std::function<bool(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> m_isMoveValideCallBack;
};
