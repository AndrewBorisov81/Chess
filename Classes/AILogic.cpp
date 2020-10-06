#include "AILogic.h"
#include "AIData.h"

USING_NS_CC;

AILogic::AILogic()
{

}

AILogic::~AILogic()
{
}

void AILogic::ChooseFigure()
{

}

void AILogic::PlaceFigure()
{
}

void AILogic::SwitchPawnWithFigure()
{

}

void AILogic::calculateBestMove(Size& bestMove)
{
  std::vector<Size*> newGameMoves;
  //use any negative large number
  float bestValue = -9999;

  /*for (var i = 0; i < newGameMoves.length; i++) {
    var newGameMove = newGameMoves[i];
    game.ugly_move(newGameMove);

    //take the negative as AI plays as black
    var boardValue = -evaluateBoard(game.board())
      game.undo();
    if (boardValue > bestValue) {
      bestValue = boardValue;
      bestMove = newGameMove
    }
  }*/
}

void AILogic::getBestMove()
{
}

float AILogic::getPieceValue(TypePiece typePiece)
{
  switch (typePiece)
  {
  case TypePiece::PAWN:
    break;

  case TypePiece::KNIGHT:
    break;

  case TypePiece::BISHOP:
    break;

  case TypePiece::ROOK:
    break;

  case TypePiece::QUEEN:
    break;

  case TypePiece::KING:
    break;
  }

  return 0.0;
}

void AILogic::minimaxRoot()
{

}

void AILogic::minimax()
{
}

float AILogic::evaluateBoard()
{
  float totalEvaluation{ 0.0f };

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      TypePiece typePiece{ TypePiece::PAWN };
      totalEvaluation = totalEvaluation + getPieceValue(typePiece);
    }
  }
  return totalEvaluation;

  return 0.0;
}

float AILogic::getAbsoluteValue(TypePiece typePiece, bool isWhite, int x, int y)
{
  if (typePiece == TypePiece::PAWN) {
    return 10 + (isWhite ? pawnEvalWhite[y][x] : pawnEvalBlack[y][x]);
  }
  else if (typePiece == TypePiece::ROOK) {
    return 50 + (isWhite ? rookEvalWhite[y][x] : rookEvalBlack[y][x]);
  }
  else if (typePiece == TypePiece::KNIGHT) {
    return 30 + knightEval[y][x];
  }
  else if (typePiece == TypePiece::BISHOP) {
    return 30 + (isWhite ? bishopEvalWhite[y][x] : bishopEvalBlack[y][x]);
  }
  else if (typePiece == TypePiece::QUEEN) {
    return 90 + evalQueen[y][x];
  }
  else if (typePiece == TypePiece::KING) {
    return 900 + (isWhite ? kingEvalWhite[y][x] : kingEvalBlack[y][x]);
  }

  throw "Unknown piece type";
}
