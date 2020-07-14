#include "Logic.h"
#include "GameLayer.h"
#include "Figure.h"
#include "Constants.h"

#include <iostream>
#include <string>

USING_NS_CC;

Logic::Logic()
{

}

Logic::Logic(GameLayer* gameLayer) 
  : m_gameLayer(gameLayer)
{
  DataChess& dataChess = m_gameLayer->getDataChess();

  for (int i = 0; i < dataChess.figures.size(); i++)
  {
    std::vector<Figure*> figuresRow = dataChess.figures[i];
    std::vector<Figure*> newRow;
    for (int j = 0; j < figuresRow.size(); j++)
    {
      newRow.push_back(dataChess.figures[i][j]);
    }
    m_figures.push_back(newRow);
  }
}

Logic::~Logic()
{
}

bool Logic::init()
{
  if (!Node::init())
  {
    return false;
  }

  return true;
}

bool Logic::castlingAllowed(Side iSide, int iColor)
{
  if (Side::QUEEN_SIDE == iSide)
  {
    return m_bCastlingQueenSideAllowed[iColor];
  }
  else //if ( KING_SIDE == iSide )
  {
    return m_bCastlingKingSideAllowed[iColor];
  }
}

Figure* Logic::getFigureAtPosition(int i, int j)
{
  Figure* figure = m_figures[i][j];
  return figure;
}

std::string Logic::getLastMove()
{
  std::string last_move;

  // Who did the last move?
  if (Player::BLACK_PLAYER == static_cast<Player>(getCurrentTurn()))
  {
    // If it's black's turn now, white had the last move
    last_move = rounds[rounds.size() - 1].white_move;
  }
  else
  {
    // Last move was black's
    last_move = rounds[rounds.size() - 1].black_move;
  }

  return last_move;
}

int Logic::getCurrentTurn()
{
  return m_currentTurn;
}

void Logic::parseMove(std::string move, Position* pFrom, Position* pTo, char* chPromoted)
{
  pFrom->iColumn = move[0];
  pFrom->iRow = move[1];
  pTo->iColumn = move[3];
  pTo->iRow = move[4];

  // Convert columns from ['A'-'H'] to [0x00-0x07]
  pFrom->iColumn = pFrom->iColumn - 'A';
  pTo->iColumn = pTo->iColumn - 'A';

  // Convert row from ['1'-'8'] to [0x00-0x07]
  pFrom->iRow = pFrom->iRow - '1';
  pTo->iRow = pTo->iRow - '1';

  if (chPromoted != nullptr)
  {
    if (move[5] == '=')
    {
      *chPromoted = move[6];
    }
    else
    {
      *chPromoted = Constants::EMPTY_SQUARE;
    }
  }
}

void Logic::updateFigures(const std::vector<std::vector<Figure*>>& figures)
{
  //m_figures.clear();

  for (unsigned int i = 0; i < figures.size(); i++)
  {
    std::vector<Figure*> figuresRow = figures[i];
    std::vector<Figure*> newRow;
    for (unsigned int j = 0; j < figuresRow.size(); j++)
    {
      newRow.push_back(figures[i][j]);
    }
    m_figures.push_back(newRow);
  }

  /*for (int i = 0; i < m_figures.size(); i++)
  {
    std::vector<Figure*> figuresRow = m_figures[i];
    for (int j = 0; j < figuresRow.size(); j++)
    {
      figuresRow[j] = figures[i][j];
    }
  }*/
}

bool Logic::isPathFree(Position startingPos, Position finishingPos, int iDirection)
{
  bool bFree = false;

  Direction direction = static_cast<Direction>(iDirection);

  switch (direction)
  {
  case Direction::HORIZONTAL:
  {
    // If it is a horizontal move, we can assume the startingPos.iRow == finishingPos.iRow
    // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
    if (startingPos.iColumn == finishingPos.iColumn)
    {
      //std::cout << "Error. Movement is horizontal but column is the same\n";
    }

    // Moving to the right
    else if (startingPos.iColumn < finishingPos.iColumn)
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = startingPos.iColumn + 1; i < finishingPos.iColumn; i++)
      {
        if (isSquareOccupied(startingPos.iRow, i))
        {
          bFree = false;
          std::cout << "Horizontal path to the right is not clear!\n";
        }
      }
    }

    // Moving to the left
    else //if (startingPos.iColumn > finishingPos.iColumn)
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = startingPos.iColumn - 1; i > finishingPos.iColumn; i--)
      {
        if (isSquareOccupied(startingPos.iRow, i))
        {
          bFree = false;
          std::cout << "Horizontal path to the left is not clear!\n";
        }
      }
    }
  }
  break;

  case Direction::VERTICAL:
  {
    // If it is a vertical move, we can assume the startingPos.iColumn == finishingPos.iColumn
    // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
    if (startingPos.iRow == finishingPos.iRow)
    {
      std::cout << "Error. Movement is vertical but row is the same\n";
      throw("Error. Movement is vertical but row is the same");
    }

    // Moving up
    else if (startingPos.iRow < finishingPos.iRow)
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = startingPos.iRow + 1; i < finishingPos.iRow; i++)
      {
        if (isSquareOccupied(i, startingPos.iColumn))
        {
          bFree = false;
          std::cout << "Vertical path up is not clear!\n";
        }
      }
    }

    // Moving down
    else //if (startingPos.iColumn > finishingPos.iRow)
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = startingPos.iRow - 1; i > finishingPos.iRow; i--)
      {
        if (isSquareOccupied(i, startingPos.iColumn))
        {
          bFree = false;
          std::cout << "Vertical path down is not clear!\n";
        }
      }
    }
  }
  break;

  case Direction::DIAGONAL:
  {
    // Moving up and right
    if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (isSquareOccupied(startingPos.iRow + i, startingPos.iColumn + i))
        {
          bFree = false;
          std::cout << "Diagonal path up-right is not clear!\n";
        }
      }
    }

    // Moving up and left
    else if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (isSquareOccupied(startingPos.iRow + i, startingPos.iColumn - i))
        {
          bFree = false;
          std::cout << "Diagonal path up-left is not clear!\n";
        }
      }
    }

    // Moving down and right
    else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (isSquareOccupied(startingPos.iRow - i, startingPos.iColumn + i))
        {
          bFree = false;
          std::cout << "Diagonal path down-right is not clear!\n";
        }
      }
    }

    // Moving down and left
    else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
    {
      // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
      bFree = true;

      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (isSquareOccupied(startingPos.iRow - i, startingPos.iColumn - i))
        {
          bFree = false;
          std::cout << "Diagonal path down-left is not clear!\n";
        }
      }
    }

    else
    {
      throw("Error. Diagonal move not allowed");
    }
  }
  break;
  }

  return bFree;
}

bool Logic::isSquareOccupied(int iRow, int iColumn)
{
  bool bOccupied = false;

  if (Constants::EMPTY_SQUARE != getFigureAtPosition(iRow, iColumn))
  {
    bOccupied = true;
  }

  return bOccupied;
}

bool Logic::wouldKingBeInCheck(Figure* figure, Position present, Position future, EnPassant* S_enPassant)
{
  IntendedMove intended_move;

  intended_move.figure = figure;
  intended_move.from.iRow = present.iRow;
  intended_move.from.iColumn = present.iColumn;
  intended_move.to.iRow = future.iRow;
  intended_move.to.iColumn = future.iColumn;

  return playerKingInCheck(&intended_move);
}

Position Logic::findKing(int iColor)
{
  bool isWhiteKing = (static_cast<int>(FigureColor::WHITE_FIGURE) == iColor) ? true : false;
  Position king = { 0 };

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      Figure* figure = getFigureAtPosition(i, j);
      if(figure->getType() == TypeFigure::KING && figure->isWhite() == isWhiteKing)
      {
        king.iRow = i;
        king.iColumn = j;
      }
    }
  }

  return king;
}

bool Logic::isKingInCheck(int iColor, IntendedMove* pintended_move)
{
  bool bCheck = false;

  Position king = { 0 };

  // Must check if the intended move is to move the king itself
  if (nullptr != pintended_move && TypeFigure::KING == pintended_move->figure->getType() && true == pintended_move->figure->isWhite())
  {
    king.iRow = pintended_move->to.iRow;
    king.iColumn = pintended_move->to.iColumn;
  }
  else
  {
    king = findKing(iColor);
  }

  UnderAttack king_attacked = isUnderAttack(king.iRow, king.iColumn, iColor, pintended_move);

  if (true == king_attacked.bUnderAttack)
  {
    bCheck = true;
  }

  return bCheck;
}

bool Logic::playerKingInCheck(IntendedMove* intended_move)
{
  return isKingInCheck(getCurrentTurn(), intended_move);
}

Figure* Logic::getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move)
{
  //char chPiece;
  Figure* figure;

  // If there is no intended move, just return the current position of the board
  if (nullptr == intended_move)
  {
    figure = getFigureAtPosition(iRow, iColumn);
  }
  else
  {
    // In this case, we are trying to understand what WOULD happed if the move was made,
    // so we consider a move that has not been made yet
    if (intended_move->from.iRow == iRow && intended_move->from.iColumn == iColumn)
    {
      // The figure wants to move from that square, so it would be empty
      figure = Constants::EMPTY_SQUARE;
    }
    else if (intended_move->to.iRow == iRow && intended_move->to.iColumn == iColumn)
    {
      // The piece wants to move to that square, so return the piece
      figure = intended_move->figure;
    }
    else
    {
      figure = getFigureAtPosition(iRow, iColumn);
    }
  }

  return figure;
}

UnderAttack Logic::isUnderAttack(int iRow, int iColumn, int iColor, IntendedMove* pintended_move)
{
  UnderAttack attack = { 0 };

  // a) Direction: HORIZONTAL
  {
    // Check all the way to the right
    for (int i = iColumn + 1; i < 8; i++)
    {
      //char chPieceFound = getPiece_considerMove(iRow, i, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(iRow, i, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN ) ||
        (chFigureFound->getType() == TypeFigure::ROOK))
      {
        // There is a queen or a rook to the right, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = iRow;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = i;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::HORIZONTAL;
        break;
      }
      else
      {
        // There is a piece that does not attack horizontally, so no problem
        break;
      }
    }

    // Check all the way to the left
    for (int i = iColumn - 1; i >= 0; i--)
    {
      //char chPieceFound = getPiece_considerMove(iRow, i, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(iRow, i, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN) ||
        (chFigureFound->getType() == TypeFigure::ROOK))
      {
        // There is a queen or a rook to the right, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = iRow;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = i;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::HORIZONTAL;
        break;
      }
      else
      {
        // There is a piece that does not attack horizontally, so no problem
        break;
      }
    }
  }

  // b) Direction: VERTICAL
  {
    // Check all the way up
    for (int i = iRow + 1; i < 8; i++)
    {
      //char chPieceFound = getPiece_considerMove(i, iColumn, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(i, iColumn, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN) ||
        (chFigureFound->getType() == TypeFigure::ROOK))
      {
        // There is a queen or a rook to the right, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = iColumn;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::VERTICAL;
        break;
      }
      else
      {
        // There is a piece that does not attack vertically, so no problem
        break;
      }
    }

    // Check all the way down
    for (int i = iRow - 1; i >= 0; i--)
    {
      //char chPieceFound = getPiece_considerMove(i, iColumn, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(i, iColumn, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN) ||
        (chFigureFound->getType() == TypeFigure::ROOK))
      {
        // There is a queen or a rook to the right, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = iColumn;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::VERTICAL;
        break;
      }
      else
      {
        // There is a piece that does not attack vertically, so no problem
        break;
      }
    }
  }

  // c) Direction: DIAGONAL
  {
    // Check the diagonal up-right
    for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
    {
      Figure* chFigureFound = getPiece_considerMove(i, j, pintended_move);
      //char chPieceFound = getPiece_considerMove(i, j, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::PAWN) &&
        (i == iRow + 1) &&
        (j == iColumn + 1) &&
        (iColor == static_cast<int>(FigureColor::WHITE_FIGURE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN) ||
        (chFigureFound->getType() == TypeFigure::BISHOP))
      {
        // There is a queen or a bishop in that direction, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else
      {
        // There is a piece that does not attack diagonally, so no problem
        break;
      }
    }

    // Check the diagonal up-left
    for (int i = iRow + 1, j = iColumn - 1; i < 8 && j > 0; i++, j--)
    {
      //char chPieceFound = getPiece_considerMove(i, j, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(i, j, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::PAWN) &&
        (i == iRow + 1) &&
        (j == iColumn - 1) &&
        (iColor == static_cast<int>(FigureColor::WHITE_FIGURE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN) ||
        (chFigureFound->getType() == TypeFigure::BISHOP))
      {
        // There is a queen or a bishop in that direction, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else
      {
        // There is a piece that does not attack diagonally, so no problem
        break;
      }
    }

    // Check the diagonal down-right
    for (int i = iRow - 1, j = iColumn + 1; i > 0 && j < 8; i--, j++)
    {
      //char chPieceFound = getPiece_considerMove(i, j, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(i, j, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::PAWN) &&
        (i == iRow - 1) &&
        (j == iColumn + 1) &&
        (iColor == static_cast<int>(FigureColor::BLACK_FIGURE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN) ||
        (chFigureFound->getType() == TypeFigure::BISHOP))
      {
        // There is a queen or a bishop in that direction, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else
      {
        // There is a piece that does not attack diagonally, so no problem
        break;
      }
    }

    // Check the diagonal down-left
    for (int i = iRow - 1, j = iColumn - 1; i > 0 && j > 0; i--, j--)
    {
      //char chPieceFound = getPiece_considerMove(i, j, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(i, j, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::PAWN) &&
        (i == iRow - 1) &&
        (j == iColumn - 1) &&
        (iColor == static_cast<int>(FigureColor::BLACK_FIGURE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if ((chFigureFound->getType() == TypeFigure::QUEEN) ||
        ((chFigureFound->getType() == TypeFigure::BISHOP)))
      {
        // There is a queen or a bishop in that direction, so the piece is in jeopardy
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else
      {
        // There is a piece that does not attack diagonally, so no problem
        break;
      }
    }
  }

  // d) Direction: L_SHAPED
  {
    // Check if the piece is put in jeopardy by a knigh

    Position knight_moves[8] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
                                 { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };
    for (int i = 0; i < 8; i++)
    {
      int iRowToTest = iRow + knight_moves[i].iRow;
      int iColumnToTest = iColumn + knight_moves[i].iColumn;

      if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
      {
        // This square does not even exist, so no need to test
        continue;
      }

      //char chPieceFound = getPiece_considerMove(iRowToTest, iColumnToTest, pintended_move);
      Figure* chFigureFound = getPiece_considerMove(iRowToTest, iColumnToTest, pintended_move);
      if (Constants::EMPTY_SQUARE == chFigureFound)
      {
        // This square is empty, move on
        continue;
      }

      int chFigureColor = (chFigureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == chFigureColor)
      {
        // This is a piece of the same color, so no problem
        continue;
      }
      else if ((chFigureFound->getType() == TypeFigure::KNIGHT))
      {
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = iRowToTest;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = iColumnToTest;
        break;
      }
    }
  }

  return attack;
}

void Logic::logMove(std::string &to_record)
{
  // If record contains only 5 chracters, add spaces
  // Because when 
  if (to_record.length() == 5)
  {
    to_record += "  ";
  }

  if (static_cast<int>(Player::WHITE_PLAYER) == getCurrentTurn())
  {
    // If this was a white player move, create a new round and leave the black_move empty
    Round round;
    round.white_move = to_record;
    round.black_move = "";

    rounds.push_back(round);
  }
  else
  {
    // If this was a black_move, just update the last Round
    Round round = rounds[rounds.size() - 1];
    round.black_move = to_record;

    // Remove the last round and put it back, now with the black move
    rounds.pop_back();
    rounds.push_back(round);
  }
}

void Logic::deleteLastMove(void)
{
  // Notice we already changed turns back
  if (static_cast<int>(Player::WHITE_PLAYER) == getCurrentTurn())
  {
    // Last move was white's turn, so simply pop from the back
    rounds.pop_back();
  }
  else
  {
    // Last move was black's, so let's 
    Round round = rounds[rounds.size() - 1];
    round.black_move = "";

    // Pop last round and put it back, now without the black move
    rounds.pop_back();
    rounds.push_back(round);
  }
}

