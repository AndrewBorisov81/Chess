#pragma once

#include "cocos2d.h"

#include <deque>
#include <stack>

class GameLayer;
class Piece;

enum class PieceColor
{
  WHITE_PIECE = 0,
  BLACK_PIECE = 1
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
  char chBefore;
  char chAfter;
  int typeBefore;
  int typeAfter;
  bool isWhite;
  Piece* pieceBefore;
  Piece* pieceAfter;
};

struct IntendedMove
{
  Piece* piece;
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

class Logic : public cocos2d::Node
{
public:
  Logic();
  Logic(GameLayer* gameLayer);
  ~Logic();
  virtual bool init();

  void movePiece(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);

  bool castlingAllowed(Side iSide, int iColor);

  Piece* getPieceAtPosition(int i, int j);

  int getPieceAtPositionI(int i, int j);

  void logMove(std::string &to_record);

  bool undoIsPossible();

  void undoLastMove();

  void initUndo(Undo& undo);

  std::string getLastMove();

  void deleteLastMove(void);

  int getCurrentTurn();

  bool isPathFree(Position startingPos, Position finishingPos, int iDirection);

  bool isReachable(int iRow, int iColumn, int iColor);

  bool Logic::isSquareOccupied(int iRow, int iColumn);

  Piece* getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move);

  UnderAttack isUnderAttack(int iRow, int iColumn, int iColor, IntendedMove* pintended_move);

  bool wouldKingBeInCheck(Piece* piece, Position present, Position future, EnPassant* S_enPassant);

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
  std::vector<Piece*> white_captured;
  std::vector<Piece*> black_captured;

  std::vector<int> iwhite_captured;
  std::vector<int> iblack_captured;

  void parseMove(std::string move, Position* pFrom, Position* pTo, char* chPromoted = nullptr);
  std::string parseMoveCellIntToString(const Position& pFrom);
  void parseMoveStringToCell(std::string move, Position* pFrom, Position* pTo);

  void updatePiece(const std::vector<std::vector<Piece*>>& pieces);

protected:
  GameLayer* m_gameLayer{ nullptr };
  int  m_currentTurn{ 0 };
  std::vector<std::vector<Piece*>> m_pieces;
  std::vector<std::vector<int>> m_board;

  // Undo is possible?
  /*struct Undo
  {
    bool bCanUndo;
    bool bCapturedLastMove;

    bool bCastlingKingSideAllowed;
    bool bCastlingQueenSideAllowed;

    EnPassant en_passant;
    Castling  castling;
    Promotion promotion;
  } m_undo;*/

  std::stack<Undo> m_undos;
  Undo m_currentUndo;

  // Castling requirements
  bool m_bCastlingKingSideAllowed[2];
  bool m_bCastlingQueenSideAllowed[2];

  //Holds the current turn
  //int  m_currentTurn{ static_cast<int>(Player::WHITE_PLAYER) };
  bool m_bGameFinished{ false };
};