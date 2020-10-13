#include "PromptLogicHelper.h"

USING_NS_CC;

PromptLogicHelper::PromptLogicHelper()
{

}

PromptLogicHelper::~PromptLogicHelper()
{

}

bool PromptLogicHelper::init()
{
  if (!Node::init())
  {
    return false;
  }

  return true;
}

PromptLogicHelper* PromptLogicHelper::createPromptLogicHelper()
{
  PromptLogicHelper* pPromptLogicHelper = new (std::nothrow) PromptLogicHelper();
  if (pPromptLogicHelper && pPromptLogicHelper->init())
  {
    pPromptLogicHelper->autorelease();
    return pPromptLogicHelper;
  }
  else
  {
    delete pPromptLogicHelper;
    pPromptLogicHelper = nullptr;
    return nullptr;
  }

  return pPromptLogicHelper;
}

void PromptLogicHelper::getPossibleMoves(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& resultPossibleMovesPiece)
{

  switch (typePiece)
  {
  case 1: // Rook
    getPossibleMovesDirection(presentCell.width, presentCell.height, Direction::HORIZONTAL, resultPossibleMovesPiece);
    getPossibleMovesDirection(presentCell.width, presentCell.height, Direction::VERTICAL, resultPossibleMovesPiece);
    break;

  case 2: // Knight
    getPossibleMovesDirection(presentCell.width, presentCell.height, Direction::L_SHAPE, resultPossibleMovesPiece);
    break;

  case 3: // Bishop
    getPossibleMovesDirection(presentCell.width, presentCell.height, Direction::DIAGONAL, resultPossibleMovesPiece);
    break;

  case 4: // Queen
    getPossibleMovesDirection(presentCell.width, presentCell.height, Direction::DIAGONAL, resultPossibleMovesPiece);
    getPossibleMovesDirection(presentCell.width, presentCell.height, Direction::HORIZONTAL, resultPossibleMovesPiece);
    getPossibleMovesDirection(presentCell.width, presentCell.height, Direction::VERTICAL, resultPossibleMovesPiece);
    break;

  case 5: // King
  {
    constexpr int size = 10;
    Size king_moves[size] = { Size(1, 0), Size(-1, 0), Size(0, -1), Size(0, 1),
                            Size(1, -1), Size(1, 1), Size(-1, -1), Size(-1, 1),
                            Size(0, -2), Size(0, 2) };

    getPossibleMovesPawnKing(presentCell.width, presentCell.height, resultPossibleMovesPiece, king_moves, size);

  }
  break;

  case 6: // Pawn
  {
    constexpr int size = 8;
    Size pawn_moves[size]{ Size(1, 0), Size(1, 1), Size(1, -1), Size(2, 0),
                        Size(-1, 0), Size(-1, 1), Size(-1, -1), Size(-2, 0) };

    getPossibleMovesPawnKing(presentCell.width, presentCell.height, resultPossibleMovesPiece, pawn_moves, size);
  }
  break;
  }
}

void PromptLogicHelper::getPossibleMovesDirection(int iRow, int iColumn, Direction direction, std::vector<cocos2d::Size>& resultPossibleMovesPiece)
{
  switch (direction)
  {
  case Direction::DIAGONAL:
  {
    // Check the diagonal up-right
    for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
    {
      resultPossibleMovesPiece.push_back(Size(i, j));
    }

    // Check the diagonal up-left
    for (int i = iRow + 1, j = iColumn - 1; i < 8 && j >= 0; i++, j--)
    {
      resultPossibleMovesPiece.push_back(Size(i, j));
    }

    // Check the diagonal down-right
    for (int i = iRow - 1, j = iColumn + 1; i >= 0 && j < 8; i--, j++)
    {
      resultPossibleMovesPiece.push_back(Size(i, j));
    }

    // Check the diagonal down-left
    for (int i = iRow - 1, j = iColumn - 1; i >= 0 && j >= 0; i--, j--)
    {
      resultPossibleMovesPiece.push_back(Size(i, j));
    }
  }
  break;

  case Direction::HORIZONTAL:
  {
    // Check all the way to the right
    for (int i = iColumn + 1; i < 8; i++)
    {
      resultPossibleMovesPiece.push_back(Size(iRow, i));
    }

    // Check all the way to the left
    for (int i = iColumn - 1; i >= 0; i--)
    {
      resultPossibleMovesPiece.push_back(Size(iRow, i));
    }
  }
  break;

  case Direction::VERTICAL:
  {
    // Check all the way up
    for (int i = iRow + 1; i < 8; i++)
    {
      resultPossibleMovesPiece.push_back(Size(i, iColumn));
    }

    // Check all the way down
    for (int i = iRow - 1; i >= 0; i--)
    {
      resultPossibleMovesPiece.push_back(Size(i, iColumn));
    }
  }
  break;

  case Direction::L_SHAPE:
  {
    // Knight
    Size knight_moves[8] = { Size(1, -2), Size(2, -1), Size(2, 1), Size(1, 2),
                          Size(-1, -2), Size(-2, -1), Size(-2, 1), Size(-1, 2) };
    for (int i = 0; i < 8; i++)
    {
      int row = iRow + knight_moves[i].width;
      int column = iColumn + knight_moves[i].height;

      if (row < 0 || row > 7 || column < 0 || column > 7)
      {
        // This square does not even exist, so no need to test
        continue;
      }

      resultPossibleMovesPiece.push_back(Size(row, column));
    }
  }
  break;
  }
}

void PromptLogicHelper::getPossibleMovesPawnKing(int iRow, int iColumn, std::vector<cocos2d::Size>& resultPossibleMovesPiece, cocos2d::Size* piece_moves, int size)
{
  for (int i = 0; i < size; i++)
  {
    int row = iRow + piece_moves[i].width;
    int column = iColumn + piece_moves[i].height;

    if (row < 0 || row > 7 || column < 0 || column > 7)
    {
      // This square does not even exist, so no need to test
      continue;
    }

    resultPossibleMovesPiece.push_back(Size(row, column));
  }
}

void PromptLogicHelper::callBackIsMoveValide(const std::function<void(const cocos2d::Size&presentCell, const cocos2d::Size&futureCell)> isMoveValide)
{
  m_isMoveValideCallback = isMoveValide;
}
