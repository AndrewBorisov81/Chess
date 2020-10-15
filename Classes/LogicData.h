#pragma once

#include <variant>

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

struct Move
{
  int iPiece;
  Position from;
  Position to;

  EnPassant en_passant;
  Castling castling;
  Promotion promotion;
};

struct SimpleT { int fromx, fromy, tox, toy; };
struct CaptureT { int fromx, fromy, tox, toy; bool enpassant; };
struct CastleT { bool kingSide; };

class ChessMove
{
  std::variant<SimpleT, CaptureT, CastleT> m_details;

  /*int iPiece;
  Player turn;
  Position from;
  Position to;*/

  bool isSimple() const { return m_details.index() == 0; };
  bool isCapturing() const { return m_details.index() == 1; };
  bool isCastling() const { return m_details.index() == 2; };
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
