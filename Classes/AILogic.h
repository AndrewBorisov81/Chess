#pragma once

#include "cocos2d.h"

#include "Piece.h"

class AILogic: public cocos2d::Node
{
public:
  AILogic();
  ~AILogic();

  void ChooseFigure();
  void PlaceFigure();
  void SwitchPawnWithFigure();

  void calculateBestMove(cocos2d::Size& bestMove);

  void getBestMove();

  float getAbsoluteValue(TypePiece typePiece, bool isWhite, int x, int y);
  void getPieceValue(TypePiece typePiece);

  void minimaxRoot();
  void minimax();

  float evaluateBoard();

private:

};
