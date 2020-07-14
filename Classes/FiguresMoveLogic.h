#pragma once

#include "cocos2d.h"

#include "Logic.h"
#include "GameLayer.h"

class FiguresMoveLogic : public Logic
{
public:
  FiguresMoveLogic();
  FiguresMoveLogic(GameLayer* gameLayer);
  ~FiguresMoveLogic();

  virtual bool init();

  bool isMoveValid(Figure* figure, Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);
};
