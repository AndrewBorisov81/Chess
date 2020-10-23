#include "AILogic.h"
#include "AIData.h"
#include "PieceMove.h"

#include <vector>
#include <limits>

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

void AILogic::calculateBestMove(Size& bestMove)
{
  std::vector<cocos2d::Size> valideMovesFrom;
  std::vector<cocos2d::Size> valideMovesTo;

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
            valideMovesFrom.push_back(Size(i,j));
            valideMovesTo.push_back(valideMove);
          }
        }
      }
    }


  // Get type piece move
  std::vector<PieceMove> pieceMoves;
  getTypePieceMoves(valideMovesFrom, valideMovesTo, pieceMoves);


  // Calculate best Move
  float bestValue = std::numeric_limits<float>::min();
  // to do ??????????????????????????? equalValueBestMoves get random value
  std::vector<PieceMove> equalValueBestMoves;
  PieceMove bestPieceMove;

  for (unsigned int i = 0; i < pieceMoves.size(); i++)
  {
    PieceMove newGameMove = pieceMoves[i];

    //take the negative as AI plays as black
    float boardValue = -evaluateBoard(newGameMove);

    if (boardValue > bestValue) {
      bestValue = boardValue;
      bestPieceMove = newGameMove;
    }
  }
}

float AILogic::evaluateBoard(const PieceMove& pieceMove)
{
  float totalEvaluation{ 0.0f };
  bool isWhite{ false };
  TypePiece typePiece{ TypePiece::PAWN };

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      int typePieceI = m_callBackGetPiece(i, j);

      isWhite = Piece::isWhite(typePieceI);

      typePiece = Piece::getTypeP(typePieceI);

      if (pieceMove.isSimple())
      {
        totalEvaluation = totalEvaluation + getPieceValue(typePiece, isWhite, i, j);
      }
      else if (pieceMove.isCapturing())
      {
        //???????????????????????????
        continue;
      }
      else if (pieceMove.isPromotion)
      {
        std::vector<TypePiece> promotionType { TypePiece::ROOK, TypePiece::BISHOP, TypePiece::KNIGHT, TypePiece::QUEEN };
      }
      else if (pieceMove.isCapturingPromotion)
      {

      }

      //totalEvaluation = totalEvaluation + getPieceValue(typePiece, isWhite, i, j);
    }
  }
  return totalEvaluation;
}

float AILogic::getPieceValue(TypePiece typePiece, bool isWhite, int x, int y)
{
  float absoluteValue = getAbsoluteValue(typePiece, isWhite, x, y);

  return (isWhite) ? absoluteValue : -absoluteValue;
}

void AILogic::getBestMove(Size& bestMove)
{
  calculateBestMove(bestMove);
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

}

void AILogic::minimax()
{
}


void AILogic::generateMoves()
{
}

void AILogic::buildMoves()
{

}

void AILogic::addMove()
{

}

void AILogic::getValideMovesCallBack(const std::function<void(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves)>& getValideMoves)
{
  m_getValideMoves = getValideMoves;

  // No check is move valide
  /*std::vector<cocos2d::Size> possibleMoves;

  m_promptLogicHelper->getPossibleMoves(typePiece, presentCell, possibleMoves);

  for (auto el : possibleMoves)
  {
    bool isMoveValide{ false };

    if (m_isMoveValideCallBack)
      isMoveValide = m_isMoveValideCallBack(presentCell, el);

    if (isMoveValide)
      valideMoves.push_back(el);
  }*/
}

/*void AILogic::createTypeMoves(std::vector<Size>& movesFrom, std::vector<Size>& movesTo, std::vector<PieceMove>& pieceMoves)
{

}*/

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

/*void AILogic::callBackIsMoveValide(const std::function<bool(const cocos2d::Size&presentCell, const cocos2d::Size&futureCell)> isMoveValideCallBack)
{
  m_isMoveValideCallBack = isMoveValideCallBack;
}*/
