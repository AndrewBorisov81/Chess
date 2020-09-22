#pragma once

#include "cocos2d.h"

#include <vector>

#include "LogicData.h"

class PromptLogicHelper : public cocos2d::Node
{
public:
  PromptLogicHelper();
  ~PromptLogicHelper();

  virtual bool init();

  static PromptLogicHelper* createPromptLogicHelper();

  void getPossibleMoves(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& cellsPossibleMovesPiece);

  void getPossibleMovesDirection(int iRow, int iColumn, Direction direction, std::vector<cocos2d::Size>& resultPossibleMovesPiece);

  void getPossibleMovesPawnKing(int iRow, int iColumn, std::vector<cocos2d::Size>& resultPossibleMovesPiece, cocos2d::Size* piece_moves, int size);

  // CallBack
  void callBackIsMoveValide(const std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> isMoveValide);

private:
  std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> m_isMoveValideCallback{ nullptr };
};