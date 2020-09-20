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
  std::vector<cocos2d::Size> futurePossibleMovesPiece;

  switch (typePiece)
  {
    case 1: // Rook
      break;

    case 2:
    {// Knight
      Size knight_moves[8] = { Size(1, -2), Size(2, -1), Size(2, 1), Size(1, 2),
                                Size(-1, -2), Size(-2, -1), Size(-2, 1), Size(-1, 2) };
      for (int i = 0; i < 8; i++)
      {
        int iRowToTest = presentCell.width + knight_moves[i].width;
        int iColumnToTest = presentCell.height + knight_moves[i].height;

        if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
        {
          // This square does not even exist, so no need to test
          continue;
        }
        
        resultPossibleMovesPiece.push_back(Size(iRowToTest, iColumnToTest));
      }
    }
      break;

    case 3: // Bishop
      break;

    case 4: // Queen
      break;

    case 5: // King
    {
      /*Size king_moves[10] = { { 1, 0 }, { -1, 0 }, { 0, -1 }, { 0, 1 },
                              { 1, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 } };*/
    }
      break;

    case 6: // Pawn
    {
      /*Size pawn_moves[8]{ { 1, 0 }, { 1, 1 }, { 1, -1}, { 2, 0 },
                           { -1, 0 }, { -1, 1 }, { -1, -1 }, {-2, 0} };*/
    }
      break;
  }
}

/*bool PromptLogicHelper::isReachable(int iRow, int iColumn, int iColor)
{
  bool bReachable = false;

  // a) Direction: HORIZONTAL
  {
    // Check all the way to the right
    for (int i = iColumn + 1; i < 8; i++)
    {
     
    }

    // Check all the way to the left
    for (int i = iColumn - 1; i >= 0; i--)
    {
      
    }
  }

  // b) Direction: VERTICAL
  {
    // Check all the way up
    for (int i = iRow + 1; i < 8; i++)
    {
      
    }

    // Check all the way down
    for (int i = iRow - 1; i >= 0; i--)
    {
    
    }
  }

  // c) Direction: DIAGONAL
  {
    // Check the diagonal up-right
    for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
    {

    }

    // Check the diagonal up-left
    for (int i = iRow + 1, j = iColumn - 1; i < 8 && j > 0; i++, j--)
    {

    }

    // Check the diagonal down-right
    for (int i = iRow - 1, j = iColumn + 1; i > 0 && j < 8; i--, j++)
    {
  
    }

    // Check the diagonal down-left
    for (int i = iRow - 1, j = iColumn - 1; i > 0 && j > 0; i--, j--)
    {
  
    }
  }

  // d) Direction: L_SHAPED
  {
    // Check if the piece is put in jeopardy by a knigh

    /*Position knight_moves[8] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
                                 { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };
    for (int i = 0; i < 8; i++)
    {
      int iRowToTest = iRow + knight_moves[i].iRow;
      int iColumnToTest = iColumn + knight_moves[i].iColumn;

      if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
      {
        // This square does not even exist, so no need to test
        continue;
      }*/

      /*if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        continue;
      }
      else if (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::KNIGHT)
      {
        bReachable = true;
        break;
      }
    }
  }

  return bReachable;
}*/

void PromptLogicHelper::callBackIsMoveValide(const std::function<void(const cocos2d::Size&presentCell, const cocos2d::Size&futureCell)> isMoveValide)
{
  m_isMoveValideCallback = isMoveValide;
}
