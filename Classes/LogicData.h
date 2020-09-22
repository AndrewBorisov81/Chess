#pragma once

enum class Player
{
  WHITE_PLAYER = 0,
  BLACK_PLAYER = 1
};

enum class Direction
{
  HORIZONTAL = 0,
  VERTICAL,
  DIAGONAL,
  L_SHAPE
};

enum Side
{
  QUEEN_SIDE = 2,
  KING_SIDE = 3
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
  int typeBefore;
  int typeAfter;
};

struct IntendedMove
{
  int iPiece;
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

struct Undo
{
  bool bCanUndo;
  bool bCapturedLastMove;

  bool bCastlingKingSideAllowed;
  bool bCastlingQueenSideAllowed;

  EnPassant en_passant;
  Castling  castling;
  Promotion promotion;
};