#include "AILogic.h"
#include "AIData.h"
#include "PieceMove.h"

#include <vector>
#include <limits>
#include <stdlib.h>     
#include <time.h> 

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

      if (i > 6)
      {
        if (i == moveTo.iRow && j == moveTo.iColumn)
          totalEvaluation = totalEvaluation + getPieceValue(typePiece, isWhite, i, j) - capturedPieceValue + promotionPieceValue;
        else
          totalEvaluation = totalEvaluation + getPieceValue(typePiece, isWhite, i, j);
      }
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
  calculateBestMove(pieceMoves, bestMove);
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

void AILogic::minimaxRoot()
{
  /*var newGameMoves = game.ugly_moves();
  var bestMove = -9999;
  var bestMoveFound;

  for (var i = 0; i < newGameMoves.length; i++) {
    var newGameMove = newGameMoves[i];
    game.ugly_move(newGameMove);
    var value = minimax(depth - 1, game, !isMaximisingPlayer);
    game.undo();
    if (value >= bestMove) {
      bestMove = value;
      bestMoveFound = newGameMove;
    }
  }
  return bestMoveFound;*/
}

void AILogic::minimax()
{
  /*positionCount++;
  if (depth == = 0) {
    return -evaluateBoard(game.board());
  }

  var newGameMoves = game.ugly_moves();

  if (isMaximisingPlayer) {
    var bestMove = -9999;
    for (var i = 0; i < newGameMoves.length; i++) {
      game.ugly_move(newGameMoves[i]);
      bestMove = Math.max(bestMove, minimax(depth - 1, game, !isMaximisingPlayer));
      game.undo();
    }
    return bestMove;
  }
  else {
    var bestMove = 9999;
    for (var i = 0; i < newGameMoves.length; i++) {
      game.ugly_move(newGameMoves[i]);
      bestMove = Math.min(bestMove, minimax(depth - 1, game, !isMaximisingPlayer));
      game.undo();
    }
    return bestMove;
  }*/
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

