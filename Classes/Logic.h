#pragma once

#include "cocos2d.h"

#include <deque>
#include <stack>
#include <array>

#include <functional>

#include <LogicData.h>

class GameLayer;
class Piece;

class Logic : public cocos2d::Node
{
public:
  Logic();

  Logic(GameLayer* gameLayer);

  ~Logic();

  virtual bool init();

  void movePiece(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);

  bool castlingAllowed(Side iSide, int iColor);

  int getPieceAtPositionI(int i, int j);

  void logMove(std::string &to_record);

  bool undoIsPossible();

  void undoLastMove();

  void initUndo(Undo& undo);

  std::string getLastMove();

  void deleteLastMove(void);

  int getCurrentTurn();

  int getOppositCurrentTurn();

  bool isPathFree(Position startingPos, Position finishingPos, int iDirection);

  bool isReachable(int iRow, int iColumn, int iColor);

  bool Logic::isSquareOccupied(int iRow, int iColumn);

  int getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move);

  UnderAttack isUnderAttack(int iRow, int iColumn, int iColor, IntendedMove* pintended_move);

  bool wouldKingBeInCheck(int iPiece, Position present, Position future, EnPassant* S_enPassant);

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
  std::vector<Piece*> white_captured;
  std::vector<Piece*> black_captured;

  std::vector<int> iwhite_captured;
  std::vector<int> iblack_captured;

  //void parseMove(std::string move, Position* pFrom, Position* pTo, char* chPromoted = nullptr);
  std::string parseMoveCellIntToString(const Position& pFrom);
  void parseMoveStringToCell(std::string move, Position* pFrom, Position* pTo);

  //void updatePiece(const std::vector<std::vector<Piece*>>& pieces);

  std::array<std::array<int, 8>, 8>& getBoardA();
  void loadBoard(const int piece_board[8][8]);

  //Callbacks
  void callBackAddPiece(const std::function<void(int type, bool isWhite, const cocos2d::Size& futureCell)> addPiece);
  void callBackDeletePiece(const std::function<void(const cocos2d::Size& presentCell)> deletePiece);
  void callBackMovePiece(const std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> movePiece);
  void callBackUndoLastMove(const std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> undoLastMove);

protected:
  GameLayer* m_gameLayer{ nullptr };
  
  std::vector<std::vector<Piece*>> m_pieces;
  
  std::array<std::array<int, 8>, 8> m_boardA;

  std::stack<Undo> m_undos;
  Undo m_currentUndo;

  // Castling requirements
  bool m_bCastlingKingSideAllowed[2];
  bool m_bCastlingQueenSideAllowed[2];

  //Holds the current turn
  int  m_currentTurn{ static_cast<int>(Player::WHITE_PLAYER) };
  bool m_bGameFinished{ false };

  //Callbacks
  std::function<void(const cocos2d::Size& presentCell)> m_deletePiece{ nullptr };
  std::function<void(int type, bool isWhite, const cocos2d::Size& futureCell)> m_addPiece{ nullptr };
  std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> m_movePiece{ nullptr };
  std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> m_undoLastMove{ nullptr };
};