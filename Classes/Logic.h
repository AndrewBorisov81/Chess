#pragma once

#include "cocos2d.h"

#include <deque>

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
  Figure* figureBefore;
  Figure* figureAfter;
};

struct IntendedMove
{
  Figure* figure;
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

class Logic : public cocos2d::Node
{
public:
  Logic();
  Logic(GameLayer* gameLayer);
  ~Logic();
  virtual bool init();

  void moveFigure(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);

  bool castlingAllowed(Side iSide, int iColor);

  Figure* getFigureAtPosition(int i, int j);

  void logMove(std::string &to_record);

  bool undoIsPossible();

  void undoLastMove();

  std::string getLastMove();

  void deleteLastMove(void);

  int getCurrentTurn();

  bool isPathFree(Position startingPos, Position finishingPos, int iDirection);

  bool isReachable(int iRow, int iColumn, int iColor);

  bool Logic::isSquareOccupied(int iRow, int iColumn);

  Figure* getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move);

  UnderAttack isUnderAttack(int iRow, int iColumn, int iColor, IntendedMove* pintended_move);

  bool wouldKingBeInCheck(Figure* figure, Position present, Position future, EnPassant* S_enPassant);

  int getOpponentColor();

  void changeTurns(void);

  Position findKing(int iColor);

  bool canBeBlocked(Position startingPos, Position finishingPos, int iDirection);

  bool isCheckMate();

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
  /*std::vector<char> white_captured;
  std::vector<char> black_captured;*/
  std::vector<Figure*> white_captured;
  std::vector<Figure*> black_captured;

  void parseMove(std::string move, Position* pFrom, Position* pTo, char* chPromoted = nullptr);
  std::string parseMoveCellIntToString(const Position& pFrom);
  void parseMoveStringToCell(std::string move, Position* pFrom, Position* pTo);

  void updateFigures(const std::vector<std::vector<Figure*>>& figures);

protected:
  GameLayer* m_gameLayer{ nullptr };
  int  m_currentTurn{ 0 };
  std::vector<std::vector<Figure*>> m_figures;

  // Undo is possible?
  struct Undo
  {
    bool bCanUndo;
    bool bCapturedLastMove;

    bool bCastlingKingSideAllowed;
    bool bCastlingQueenSideAllowed;

    EnPassant en_passant;
    Castling  castling;
    Promotion promotion;
  } m_undo;

  // Castling requirements
  bool m_bCastlingKingSideAllowed[2];
  bool m_bCastlingQueenSideAllowed[2];

  //Holds the current turn
  //int  m_currentTurn{ static_cast<int>(Player::WHITE_PLAYER) };
  bool m_bGameFinished{ false };
};