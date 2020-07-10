#pragma once

#include "cocos2d.h"

#include "Logic.h"

class GameLayer;

class FiguresMoveLogic : public Logic
{
public:
  FiguresMoveLogic();
  FiguresMoveLogic(GameLayer* gameLayer);
  ~FiguresMoveLogic();

  virtual bool init();

  bool isMoveValid(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);
};
