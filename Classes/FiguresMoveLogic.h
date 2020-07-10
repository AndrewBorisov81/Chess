#pragma once

#include "cocos2d.h"

#include "Logic.h"

class GameLayer;

class FiguresMoveLogic : public Logic
{
  FiguresMoveLogic();
  ~FiguresMoveLogic();

  bool isMoveValid(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);

private:
  GameLayer* m_gameLayer{nullptr};
};
