#include "AILogic.h"
#include "AIData.h"
#include "PieceMove.h"

#include <vector>

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
  float bestValue = -9999.0f;

  std::vector<cocos2d::Size> valideMovesFrom;
  std::vector<cocos2d::Size> valideMovesTo;

  //std::vector<PieceMove> validePieceMoves;

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

  // get best move
  for (auto &valideMove : valideMovesTo)
  {

  }

  Size bestMoveTo;

  //float eB = evaluateBoard(piece, Size(i, j), possibleMovesTo, bestMoveTo);

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

float AILogic::getPieceValue(TypePiece typePiece, bool isWhite, int x, int y)
{
  float absoluteValue = getAbsoluteValue(typePiece, isWhite, x, y);

  return (isWhite) ? absoluteValue : -absoluteValue;
}

void AILogic::minimaxRoot()
{

}

void AILogic::minimax()
{
}

float AILogic::evaluateBoard(int typePiece, Size& moveFrom, std::vector<cocos2d::Size> possibleMovesTo, Size& bestMoveTo)
{
  float totalEvaluation{ 0.0f };
  bool isWhite{ true };
  //TypePiece typePiece{ TypePiece::PAWN };

  // Create simple PieceMove;
  PieceMove pieceMove;
  SimpleT simpleMove;
  pieceMove.details = simpleMove;

  for (auto &moveTo : possibleMovesTo)
  {

  }

  /*for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      int typePieceI = m_callBackGetPiece(i, j);

      isWhite = Piece::isWhite(typePieceI);

      typePiece = Piece::getTypeP(typePieceI);

      totalEvaluation = totalEvaluation + getPieceValue(typePiece, isWhite, i, j);
    }
  }*/
  return totalEvaluation;
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
