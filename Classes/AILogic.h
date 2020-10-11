#pragma once

#include "cocos2d.h"

#include <functional>
//#include <variant>

#include "Piece.h"
#include "LogicData.h"

/*struct Simple{ int fromx, fromy, tox, toy; };
struct Capture { int fromx, fromy, tox, toy; bool enpassant; };
struct Castle { bool kingSide; };*/

/*class ChessMove
{
  std::variant<Simple, Capture, Castle> m_details;

  int iPiece;
  Player turn;
  Position from;
  Position to;

  bool isSimple() const{ return m_details.index() == 0; };
  bool isCapturing() const { return m_details.index() == 1; };
  bool isCastling() const { return m_details.index() == 2; };
};*/

class PromptLogicHelper;

class AILogic: public cocos2d::Node
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
  //void getPossibleMoves(const std::vector<cocos2d::Size>& possibleMoves, Player turn = Player::BLACK_PLAYER);

  float getAbsoluteValue(TypePiece typePiece, bool isWhite, int x, int y);
  float getPieceValue(TypePiece typePiece, bool isWhite, int x, int y);

  void minimaxRoot();
  void minimax();

  float evaluateBoard();

  void generateMoves();
  void buildMoves();
  void addMove();

  // Callbacks
  void callBackGetPiece(const std::function<int(int x, int y)>& callBack);

  // Call backs
  void getValideMoves(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves);
  void callBackIsMoveValide(const std::function<bool(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> isMoveValide);

private:
  Player m_turn{ Player::BLACK_PLAYER };

  PromptLogicHelper* m_promptLogicHelper{ nullptr };

  std::function<int(int, int)> m_callBackGetPiece{ nullptr };

  // Call backs
  std::function<bool(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> m_isMoveValideCallBack;
  
};