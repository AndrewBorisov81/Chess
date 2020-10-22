#pragma once

#include "cocos2d.h"

#include <functional>

#include "Piece.h"
#include "PieceMove.h"
#include "LogicData.h"

class PromptLogicHelper;

class AILogic : public cocos2d::Node
{
public:
  AILogic();
  AILogic(Player turn);
  ~AILogic();

  virtual bool init();

  void setTurn(Player turn);

  void ChooseFigure();
  void PlaceFigure();
  void SwitchPawnWithFigure();

  void calculateBestMove(cocos2d::Size& bestMove);

  void getBestMove(cocos2d::Size& bestMove);
  void getTypePieceMoves(std::vector<cocos2d::Size>& moveFrom, std::vector<cocos2d::Size>& moveTo, std::vector<PieceMove>& pieceMoves);

  float getAbsoluteValue(TypePiece typePiece, bool isWhite, int x, int y);
  float getPieceValue(TypePiece typePiece, bool isWhite, int x, int y);

  void minimaxRoot();
  void minimax();

  float evaluateBoard(int typePiece, cocos2d::Size& moveFrom, std::vector<cocos2d::Size> currentPossibleMoves, cocos2d::Size& bestMoveTo);

  void generateMoves();
  void buildMoves();
  void addMove();

  // Callbacks
  void callBackGetPiece(const std::function<int(int x, int y)>& callBack);

  // Call backs
  void getValideMovesCallBack(const std::function<void(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves)>& getValideMoves);
  //void callBackIsMoveValide(const std::function<bool(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> isMoveValide);
  void getTypeMoveCallBack(const std::function<void(const cocos2d::Size& from, const cocos2d::Size& to, Player turn, PieceMove& pieceMove)>& getPieceMove);

  //void createTypeMoves(std::vector<cocos2d::Size>& movesFrom, std::vector<cocos2d::Size>& movesTo, std::vector<PieceMove>& pieceMoves);

private:
  Player m_turn{ Player::BLACK_PLAYER };

  std::function<int(int, int)> m_callBackGetPiece{ nullptr };

  // Call backs
  //std::function<bool(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> m_isMoveValideCallBack;
  std::function<void(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves)> m_getValideMoves{ nullptr };
  std::function<void(const cocos2d::Size& from, const cocos2d::Size& to, Player turn, PieceMove& pieceMove)> m_getTypePieceMove{ nullptr };
};