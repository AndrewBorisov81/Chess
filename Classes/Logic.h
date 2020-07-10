#pragma once

#include "cocos2d.h"

class GameLayer;
class Figure;

enum class FigureColor
{
  WHITE_FIGURE = 0,
  BLACK_FIGURE = 1
};

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
  //Position  pos;
  char chBefore;
  char chAfter;
};

struct IntendedMove
{
  Figure* figure;
  Position from;
  Position to;
};

/*struct IntendedMove
{
  char chPiece;
  TypeFigure typeFigure;
  bool isWhite;
  Figure* figure;
  Position from;
  Position to;
}*/

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

class Logic : public cocos2d::Node
{
public:
  Logic();
  Logic(GameLayer* gameLayer);
  ~Logic();
  virtual bool init();

  bool castlingAllowed(Side iSide, int iColor);

  Figure* getFigureAtPosition(int i, int j);

  std::string getLastMove();

  int getCurrentTurn();

  bool isPathFree(Position startingPos, Position finishingPos, int iDirection);

  bool Logic::isSquareOccupied(int iRow, int iColumn);

  Figure* getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move);

  UnderAttack isUnderAttack(int iRow, int iColumn, int iColor, IntendedMove* pintended_move);

  //bool wouldKingBeInCheck(char chPiece, Position present, Position future, EnPassant* S_enPassant);
  bool wouldKingBeInCheck(Figure* figure, Position present, Position future, EnPassant* S_enPassant);

  Position findKing(int iColor);

  bool isKingInCheck(int iColor, IntendedMove* intended_move = nullptr);

  bool playerKingInCheck(IntendedMove* intended_move = nullptr);

  // Save all the moves
  struct Round
  {
    std::string white_move;
    std::string black_move;
  };

  std::deque<Round> rounds;

  // Save the captured pieces
  std::vector<char> white_captured;
  std::vector<char> black_captured;

  void parseMove(std::string move, Position* pFrom, Position* pTo, char* chPromoted = nullptr);

private:
  GameLayer* m_gameLayer{ nullptr };

  // Castling requirements
  bool m_bCastlingKingSideAllowed[2];
  bool m_bCastlingQueenSideAllowed[2];

  //Holds the current turn
  int  m_currentTurn;
};