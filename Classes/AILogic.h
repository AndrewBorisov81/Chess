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

class AILogic: public cocos2d::Node
{
public:
  AILogic();
  ~AILogic();

  virtual bool init();

  void ChooseFigure();
  void PlaceFigure();
  void SwitchPawnWithFigure();

  void calculateBestMove(cocos2d::Size& bestMove);

  void getBestMove();

  float getAbsoluteValue(TypePiece typePiece, bool isWhite, int x, int y);
  float getPieceValue(TypePiece typePiece, bool isWhite, int x, int y);

  void minimaxRoot();
  void minimax();

  float evaluateBoard();

  void generateMoves();
  void buildMoves();
  void addMove();

  // Callbacks
  void callBackGetPieceTypeColor(const std::function<int(int x, int y)>& callBack);

private:

  std::function<int(int, int)> m_callBackPieceTypeColor{ nullptr };
  
};
