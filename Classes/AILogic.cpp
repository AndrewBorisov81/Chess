#include "AILogic.h"
#include "AIData.h"
#include "PieceMove.h"

#include <vector>
#include <limits>
#include <stdlib.h>     
#include <time.h> 
#include <algorithm>

USING_NS_CC;

AILogic::AILogic()
{

}

AILogic::AILogic(Player turn)
  : m_turn(turn)
{

}

AILogic::~AILogic()
{
}

void AILogic::setTurn(Player turn)
{
  m_turn = turn;
}

bool AILogic::init()
{
  if (!Node::init())
  {
    return false;
  }

  srand(time(NULL));

  return true;
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

void AILogic::addMove()
{

}

/*var positionCount;
var getBestMove = function(game) {
  if (game.game_over()) {
    alert('Game over');
  }

  positionCount = 0;
  var depth = parseInt($('#search-depth').find(':selected').text());

  var d = new Date().getTime();
  var bestMove = minimaxRoot(depth, game, true);
  var d2 = new Date().getTime();
  var moveTime = (d2 - d);
  var positionsPerS = (positionCount * 1000 / moveTime);

  $('#position-count').text(positionCount);
  $('#time').text(moveTime / 1000 + 's');
  $('#positions-per-s').text(positionsPerS);
  return bestMove;
};*/

void AILogic::calculateBestMove(std::vector<PieceMove>& buildMoves, PieceMove& resBestMove)
{
  // Calculate best Move
  float bestValue = -9999.0f;
  PieceMove bestMove;

  for (unsigned int i = 0; i < buildMoves.size(); i++)
  {
    PieceMove newGameMove = buildMoves[i];

    //take the negative as AI plays as black
    float boardValue = -evaluateBoard(newGameMove);

    if (boardValue > bestValue) {
      bestValue = boardValue;
      bestMove = newGameMove;
    }
  }

  resBestMove = bestMove;
}

void AILogic::getPossibleMoves(std::vector<cocos2d::Size>& valideMovesFrom, std::vector<cocos2d::Size>& valideMovesTo)
{
  // Get possible moves
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      int piece = m_callBackGetPiece(i, j);

      if (abs(piece) > 0)
      {
        Player currentTurn = (Piece::isBlack(piece)) ? Player::BLACK_PLAYER : Player::WHITE_PLAYER;

        // Get valide moves
        std::vector<cocos2d::Size> curValideMovesTo;

        if (m_turn == currentTurn)
        {
          m_getValideMoves(abs(piece), Size(i, j), curValideMovesTo);

          for (auto &valideMove : curValideMovesTo)
          {
            valideMovesFrom.push_back(Size(i, j));
            valideMovesTo.push_back(valideMove);
          }
        }
      }
    }
}

float AILogic::evaluateBoard(PieceMove& pieceMove)
{
  float totalEvaluation{ 0.0f };

  float promotionPieceValue{ 0.0f };
  float capturedPieceValue{ 0.0f };

  MoveData moveData;
  pieceMove.getMoveData(moveData);

  Position moveTo = moveData.to;
  Position moveFrom = moveData.from;

  if (pieceMove.isCapturing())
  {
    capturedPieceValue = getPieceValue(Piece::getTypeP(abs(moveData.captured)), Piece::isWhite(moveData.captured), moveTo.iRow, moveTo.iColumn );
  }
  else if (pieceMove.isPromotion())
  {
    TypePiece randomPromptionPiece = generatePromotionPiece();

    promotionPieceValue = getPieceValue(Piece::getTypeP(abs(moveData.typeAfter)), Piece::isWhite(moveData.typeAfter), moveTo.iRow, moveTo.iColumn);
  }
  else if (pieceMove.isCapturingPromotion())
  {
    capturedPieceValue = getPieceValue(Piece::getTypeP(abs(moveData.captured)), Piece::isWhite(moveData.captured), moveTo.iRow, moveTo.iColumn);

    TypePiece randomPromptionPiece = generatePromotionPiece();

    promotionPieceValue = getPieceValue(Piece::getTypeP(abs(moveData.typeAfter)), Piece::isWhite(moveData.typeAfter), moveTo.iRow, moveTo.iColumn);
  }

  bool isWhite{ false };
  TypePiece typePiece{ TypePiece::PAWN };

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      int typePieceI = m_callBackGetPiece(i, j);

      isWhite = Piece::isWhite(typePieceI);

      typePiece = Piece::getTypeP(typePieceI);

      float testPieceValue = getPieceValue(typePiece, isWhite, i, j);

      if (i == moveFrom.iRow && j == moveFrom.iColumn)
        totalEvaluation = totalEvaluation + getPieceValue(typePiece, isWhite, moveTo.iRow, moveTo.iColumn) - capturedPieceValue + promotionPieceValue;
      else
        totalEvaluation = totalEvaluation + getPieceValue(typePiece, isWhite, i, j);
    }
  }

  return totalEvaluation;
}

float AILogic::getPieceValue(TypePiece typePiece, bool isWhite, int x, int y)
{
  float absoluteValue = getAbsoluteValue(typePiece, isWhite, x, y);

  return (isWhite) ? absoluteValue : -absoluteValue;
}

TypePiece AILogic::generatePromotionPiece()
{
  std::vector<TypePiece> promotionType{ TypePiece::ROOK, TypePiece::BISHOP, TypePiece::KNIGHT, TypePiece::QUEEN };
  int r = rand() % 4;
  
  return promotionType[r];
}

void AILogic::getBestMove(PieceMove& bestMove)
{
  std::vector<cocos2d::Size> valideMovesFrom;
  std::vector<cocos2d::Size> valideMovesTo;

  getPossibleMoves(valideMovesFrom, valideMovesTo);

  // Get type piece move
  std::vector<PieceMove> pieceMoves;
  buildMoves(valideMovesFrom, valideMovesTo, pieceMoves);

  // Calculate best Move
  //calculateBestMove(pieceMoves, bestMove);
  int depth = 4;
  minimaxRoot(depth, true, pieceMoves, bestMove);
}

void AILogic::getTypePieceMoves(std::vector<cocos2d::Size>& moveFrom, std::vector<cocos2d::Size>& moveTo, std::vector<PieceMove>& pieceMoves)
{
  for (unsigned int i = 0; i < moveFrom.size(); i++)
  {
    PieceMove pieceMove;

    Size movePieceFrom = moveFrom[i];
    Size movePieceTo = moveTo[i];

    m_getTypePieceMove(movePieceFrom, movePieceTo, m_turn, pieceMove);

    pieceMoves.push_back(pieceMove);
  }
}

void AILogic::minimaxRoot(int depth, bool isMaximisingPlayer, std::vector<PieceMove>& pieceMoves, PieceMove& resBestMove)
{
  float bestMoveValue = -9999.0f;
  PieceMove bestMoveFound;

  for (unsigned int i = 0; i < pieceMoves.size(); i++) {
    PieceMove newGameMove = pieceMoves[i];

    //take the negative as AI plays as black
    float boardValue = -evaluateBoard(newGameMove);

    //float value = minimax(depth - 1, -1000, 1000, !isMaximisingPlayer, boardValue, pieceMoves);
    float value = minimax(depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), !isMaximisingPlayer, boardValue, pieceMoves);

    if (value >= bestMoveValue) {
      bestMoveValue = value;
      bestMoveFound = newGameMove;
    }
  }

  resBestMove = bestMoveFound;
}

int positionCount{ 0 };

float AILogic::minimax(int depth, float alpha, float beta, bool isMaximisingPlayer, float pieceMoveValue, std::vector<PieceMove>& pieceMoves)
{
  positionCount++;

  if (depth == 0) {
    // No move, calculate only value dashboard
    Position from{ -1, -1 }, to{ -1, -1 };

    PieceMove pieceMove;
    SimpleT simpleMove { from, to, 0 };

    pieceMove.details = simpleMove;
    
    return -evaluateBoard(pieceMove);
  }

  if (isMaximisingPlayer) {
    //float bestMoveValue = -9999.0f;
    float bestMoveValue = std::numeric_limits<float>::min();
    for (unsigned int i = 0; i < pieceMoves.size(); i++) {
   
      PieceMove newGameMove = pieceMoves[i];

      //take the negative as AI plays as black
      float boardValue = -evaluateBoard(newGameMove);

      bestMoveValue = std::max(bestMoveValue, minimax(depth - 1, alpha, beta, !isMaximisingPlayer, boardValue, pieceMoves));
      
      alpha = std::max(alpha, bestMoveValue);
      if (beta <= alpha) {
        return bestMoveValue;
      }
    }
    return bestMoveValue;
  }
  else {
    //float bestMoveValue = 9999.0f;
    float bestMoveValue = std::numeric_limits<float>::max();
    for (unsigned int i = 0; i < pieceMoves.size(); i++) {

      PieceMove newGameMove = pieceMoves[i];

      //take the negative as AI plays as black
      float boardValue = evaluateBoard(newGameMove);
      
      bestMoveValue = std::min(bestMoveValue, minimax(depth - 1, alpha, beta, !isMaximisingPlayer, boardValue, pieceMoves));
      
      beta = std::min(beta, bestMoveValue);
      if (beta <= alpha) {
        return bestMoveValue;
      }
    }
    return bestMoveValue;
  }
}


void AILogic::generateMoves(std::vector<cocos2d::Size>& moveFrom, std::vector<cocos2d::Size>& moveTo, std::vector<PieceMove>& pieceMoves)
{
  
}

void AILogic::buildMoves(std::vector<cocos2d::Size>& moveFrom, std::vector<cocos2d::Size>& moveTo, std::vector<PieceMove>& pieceMoves)
{
  for (unsigned int i = 0; i < moveFrom.size(); i++)
  {
    PieceMove pieceMove;

    Size movePieceFrom = moveFrom[i];
    Size movePieceTo = moveTo[i];

    m_getTypePieceMove(movePieceFrom, movePieceTo, m_turn, pieceMove);

    pieceMoves.push_back(pieceMove);
  }
}

void AILogic::getValideMovesCallBack(const std::function<void(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves)>& getValideMoves)
{
  m_getValideMoves = getValideMoves;
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

void AILogic::getTypeMoveCallBack(const std::function<void(const cocos2d::Size&from, const cocos2d::Size&to, Player turn, PieceMove&pieceMove)>& getPieceMove)
{
  m_getTypePieceMove = getPieceMove;
}

void AILogic::callBackGetPiece(const std::function<int(int x, int y)>& callBack)
{
  m_callBackGetPiece = callBack;
}

