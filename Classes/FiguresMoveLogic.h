#pragma once

#include "cocos2d.h"

#include "GameLayer.h"

enum class Direction
{
  HORIZONTAL = 0,
  VERTICAL,
  DIAGONAL,
  L_SHAPE
};

struct Position
{
  int iRow;
  int iColumn;
};

struct EnPassant
{
  bool bApplied;
  Position PawnCaptured;
};

struct Castling
{
  bool bApplied;
  Position rook_before;
  Position rook_after;
};

struct Promotion
{
  bool bApplied;
  //Position  pos;
  char chBefore;
  char chAfter;
};

struct IntendedMove
{
  char chPiece;
  Position from;
  Position to;
};

struct Attacker
{
  Position  pos;
  Direction dir;
};

struct UnderAttack
{
  bool bUnderAttack;
  int iNumAttackers;
  Attacker attacker[9]; //maximum theorical number of attackers
};

class GameLayer;

class FiguresMoveLogic : public cocos2d::Node
{
  FiguresMoveLogic();
  FiguresMoveLogic(GameLayer* gameLayer);
  ~FiguresMoveLogic();

  bool isMoveValid(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);

private:
  GameLayer* m_gameLayer{nullptr};
};
