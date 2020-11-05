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

  void calculateBestMove(std::vector<PieceMove>& buildMoves, PieceMove& resultBestPieceMove);

  void getBestMove(PieceMove& bestPieceMove);
  void getPossibleMoves(std::vector<cocos2d::Size>& valideMovesFrom, std::vector<cocos2d::Size>& valideMovesTo, Player turn);
  void getUglyMoves(PieceMove& forwardMove, std::vector<PieceMove>& uglyMoves);
  void getTypePieceMoves(std::vector<cocos2d::Size>& moveFrom, std::vector<cocos2d::Size>& moveTo, std::vector<PieceMove>& pieceMoves);

  float getAbsoluteValue(TypePiece typePiece, bool isWhite, int x, int y);
  float getPieceValue(TypePiece typePiece, bool isWhite, int x, int y);
  TypePiece generatePromotionPiece();

  void minimaxRoot(int depth, bool isMaximisingPlayer, std::vector<PieceMove>& pieceMoves, PieceMove& resBestMove);
  float minimax(int depth, float alpha, float beta, bool isMaximisingPlayer, float pieceMoveValue, std::vector<PieceMove>& pieceMoves);

  float evaluateBoard(PieceMove& pieceMove);

  void generateMoves(std::vector<cocos2d::Size>& moveFrom, std::vector<cocos2d::Size>& moveTo, std::vector<PieceMove>& pieceMoves);
  void buildMoves(std::vector<cocos2d::Size>& moveFrom, std::vector<cocos2d::Size>& moveTo, std::vector<PieceMove>& pieceMoves);
  void addMove();

  // Callbacks
  void callBackGetPiece(const std::function<int(int x, int y)>& callBack);
  void getValideMovesCallBack(const std::function<void(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves)>& getValideMoves);
  void getTypeMoveCallBack(const std::function<void(const cocos2d::Size& from, const cocos2d::Size& to, Player turn, PieceMove& pieceMove)>& getPieceMove);
  void forwardOneMoveLogicCallBack(const std::function<void(PieceMove& pieceMove)>& forwardOneMoveLogic);
  void undoOneMoveLogicCallBack(const std::function<void(PieceMove& pieceMove)>& undoOneMoveLogic);

private:
  Player m_turn{ Player::BLACK_PLAYER };

  // Call backs
  std::function<int(int, int)> m_callBackGetPiece{ nullptr };
  std::function<void(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves)> m_getValideMoves{ nullptr };
  std::function<void(const cocos2d::Size& from, const cocos2d::Size& to, Player turn, PieceMove& pieceMove)> m_getTypePieceMove{ nullptr };
  std::function<void(PieceMove& pieceMove)> m_forwardOneMoveLogicCallBack{ nullptr };
  std::function<void(PieceMove& pieceMove)> m_undoOneMoveLogicCallBack{ nullptr };
};