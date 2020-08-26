#pragma once

#include "cocos2d.h"

#include "Logic.h"
#include "GameLayer.h"

class PieceMoveLogic : public Logic
{
public:
  PieceMoveLogic();
  PieceMoveLogic(GameLayer* gameLayer);
  ~PieceMoveLogic();

  virtual bool init();

  bool isMoveValid(Piece* piece, Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);
};
