#include "FiguresMoveLogic.h"
#include "GameLayer.h"
#include "TouchAndDragLayer.h"
#include "Figure.h"
#include "Constants.h"

#include <stdlib.h>
#include <string>

USING_NS_CC;

FiguresMoveLogic::FiguresMoveLogic() : Logic()
{
  
}

FiguresMoveLogic::FiguresMoveLogic(GameLayer* gameLayer) : Logic(gameLayer)
{

}

FiguresMoveLogic::~FiguresMoveLogic()
{
}

bool FiguresMoveLogic::init()
{
  if (!Logic::init())
  {
    return false;
  }

  return true;
}

bool FiguresMoveLogic::isMoveValid(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion)
{
  bool bValid = false;

  //char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);
  Figure* figure = m_gameLayer->getTouchAndDragLayer()->getFigure();
  TypeFigure typeFigure = figure->getType();

  // ----------------------------------------------------
  // 1. Is the piece  allowed to move in that direction?
  // ----------------------------------------------------
  switch (typeFigure)
  {
  case TypeFigure::PAWN:
  {
    // Wants to move forward
    if (future.iColumn == present.iColumn)
    {
      // Simple move forward
      if ((figure->isWhite() && future.iRow == present.iRow + 1) ||
        (!figure->isWhite() && future.iRow == present.iRow - 1))
      {
        if (Constants::EMPTY_SQUARE == getFigureAtPosition(future.iRow, future.iColumn))
        {
          bValid = true;
        }
      }

      // Double move forward
      else if ((figure->isWhite() && future.iRow == present.iRow + 2) ||
        (!figure->isWhite() && future.iRow == present.iRow - 2))
      {
        // This is only allowed if the pawn is in its original place
        if (figure->isWhite())
        {
          if (Constants::EMPTY_SQUARE == getFigureAtPosition(future.iRow - 1, future.iColumn) &&
            Constants::EMPTY_SQUARE == getFigureAtPosition(future.iRow, future.iColumn) &&
            1 == present.iRow)
          {
            bValid = true;
          }
        }
        else // if ( isBlackPiece(chPiece) )
        {
          if (Constants::EMPTY_SQUARE == getFigureAtPosition(future.iRow + 1, future.iColumn) &&
            Constants::EMPTY_SQUARE == getFigureAtPosition(future.iRow, future.iColumn) &&
            6 == present.iRow)
          {
            bValid = true;
          }
        }
      }
      else
      {
        // This is invalid
        return false;
      }
    }

    // The "en passant" move
    else if ((figure->isWhite() && 4 == present.iRow && 5 == future.iRow && 1 == abs(future.iColumn - present.iColumn)) ||
      (!figure->isWhite() && 3 == present.iRow && 2 == future.iRow && 1 == abs(future.iColumn - present.iColumn)))
    {
      // It is only valid if last move of the opponent was a double move forward by a pawn on a adjacent column
      std::string last_move = getLastMove();

      // Parse the line
      Position LastMoveFrom;
      Position LastMoveTo;
      parseMove(last_move, &LastMoveFrom, &LastMoveTo);

      // First of all, was it a pawn?
      Figure* LstMvFigure = getFigureAtPosition(LastMoveTo.iRow, LastMoveTo.iColumn);

      if (LstMvFigure->getType() != TypeFigure::PAWN)
      {
        return false;
      }

      // Did the pawn have a double move forward and was it an adjacent column?
      if (2 == abs(LastMoveTo.iRow - LastMoveFrom.iRow) && 1 == abs(LastMoveFrom.iColumn - present.iColumn))
      {
        //std::cout << "En passant move!\n";
        bValid = true;

        S_enPassant->bApplied = true;
        S_enPassant->PawnCaptured.iRow = LastMoveTo.iRow;
        S_enPassant->PawnCaptured.iColumn = LastMoveTo.iColumn;
      }
    }

    // Wants to capture a piece
    else if (1 == abs(future.iColumn - present.iColumn))
    {
      if ((figure->isWhite() && future.iRow == present.iRow + 1) || (!figure->isWhite() && future.iRow == present.iRow - 1))
      {
        // Only allowed if there is something to be captured in the square
        if (Constants::EMPTY_SQUARE != getFigureAtPosition(future.iRow, future.iColumn))
        {
          bValid = true;
          //std::cout << "Pawn captured a piece!\n";
        }
      }
    }
    else
    {
      // This is invalid
      return false;
    }

    // If a pawn reaches its eight rank, it must be promoted to another piece
    if ((figure->isWhite() && 7 == future.iRow) ||
      (!figure->isWhite() && 0 == future.iRow))
    {
      //std::cout << "Pawn must be promoted!\n";
      S_promotion->bApplied = true;
    }
  }
  break;

  case TypeFigure::ROOK:
  {
    // Horizontal move
    if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
    {
      // Check if there are no pieces on the way
      if (isPathFree(present, future, static_cast<int>(Direction::HORIZONTAL)))
      {
        bValid = true;
      }
    }
    // Vertical move
    else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
    {
      // Check if there are no pieces on the way
      if (isPathFree(present, future, static_cast<int>(Direction::VERTICAL)))
      {
        bValid = true;
      }
    }
  }
  break;

  case TypeFigure::KNIGHT:
  {
    if ((2 == abs(future.iRow - present.iRow)) && (1 == abs(future.iColumn - present.iColumn)))
    {
      bValid = true;
    }

    else if ((1 == abs(future.iRow - present.iRow)) && (2 == abs(future.iColumn - present.iColumn)))
    {
      bValid = true;
    }
  }
  break;

  case TypeFigure::BISHOP:
  {
    // Diagonal move
    if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
    {
      // Check if there are no pieces on the way
      if (isPathFree(present, future, static_cast<int>(Direction::DIAGONAL)))
      {
        bValid = true;
      }
    }
  }
  break;

  case TypeFigure::QUEEN:
  {
    // Horizontal move
    if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
    {
      // Check if there are no pieces on the way
      if (isPathFree(present, future, static_cast<int>(Direction::HORIZONTAL)))
      {
        bValid = true;
      }
    }
    // Vertical move
    else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
    {
      // Check if there are no pieces on the way
      if (isPathFree(present, future, static_cast<int>(Direction::VERTICAL)))
      {
        bValid = true;
      }
    }

    // Diagonal move
    else if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
    {
      // Check if there are no pieces on the way
      if (isPathFree(present, future, static_cast<int>(Direction::DIAGONAL)))
      {
        bValid = true;
      }
    }
  }
  break;

  case TypeFigure::KING:
  {
    // Horizontal move by 1
    if ((future.iRow == present.iRow) && (1 == abs(future.iColumn - present.iColumn)))
    {
      bValid = true;
    }

    // Vertical move by 1
    else if ((future.iColumn == present.iColumn) && (1 == abs(future.iRow - present.iRow)))
    {
      bValid = true;
    }

    // Diagonal move by 1
    else if ((1 == abs(future.iRow - present.iRow)) && (1 == abs(future.iColumn - present.iColumn)))
    {
      bValid = true;
    }

    // Castling
    else if ((future.iRow == present.iRow) && (2 == abs(future.iColumn - present.iColumn)))
    {
      // Castling is only allowed in these circunstances:

      // 1. King is not in check
      if (true == playerKingInCheck(nullptr))
      {
        return false;
      }

      // 2. No pieces in between the king and the rook
      if (false == isPathFree(present, future, static_cast<int>(Direction::HORIZONTAL)))
      {
        return false;
      }

      // 3. King and rook must not have moved yet;
      // 4. King must not pass through a square that is attacked by an enemy piece
      if (future.iColumn > present.iColumn)
      {
        // if future.iColumn is greather, it means king side
        int figureColor = (figure->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
        if (false == castlingAllowed(Side::KING_SIDE, figureColor))
        {
          //createNextMessage("Castling to the king side is not allowed.\n");
          return false;
        }
        else
        {
          // Check if the square that the king skips is not under attack
          UnderAttack square_skipped = isUnderAttack(present.iRow, present.iColumn + 1, getCurrentTurn(), nullptr);
          if (false == square_skipped.bUnderAttack)
          {
            // Fill the S_castling structure
            S_castling->bApplied = true;

            // Present position of the rook
            S_castling->rook_before.iRow = present.iRow;
            S_castling->rook_before.iColumn = present.iColumn + 3;

            // Future position of the rook
            S_castling->rook_after.iRow = future.iRow;
            S_castling->rook_after.iColumn = present.iColumn + 1; // future.iColumn -1

            bValid = true;
          }
        }
      }
      else //if (future.iColumn < present.iColumn)
      {
        // if present.iColumn is greather, it means queen side
        int figureColor = (figure->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
        if (false == castlingAllowed(Side::QUEEN_SIDE, figureColor))
        {
          //createNextMessage("Castling to the queen side is not allowed.\n");
          return false;
        }
        else
        {
          // Check if the square that the king skips is not attacked
          UnderAttack square_skipped = isUnderAttack(present.iRow, present.iColumn - 1, getCurrentTurn(), nullptr);
          if (false == square_skipped.bUnderAttack)
          {
            // Fill the S_castling structure
            S_castling->bApplied = true;

            // Present position of the rook
            S_castling->rook_before.iRow = present.iRow;
            S_castling->rook_before.iColumn = present.iColumn - 4;

            // Future position of the rook
            S_castling->rook_after.iRow = future.iRow;
            S_castling->rook_after.iColumn = present.iColumn - 1; // future.iColumn +1

            bValid = true;
          }
        }
      }
    }
  }
  break;

  default:
  {
    //std::cout << "!!!!Should not reach here. Invalid piece: " << char(chPiece) << "\n\n\n";
  }
  break;
  }

  // If it is a move in an invalid direction, do not even bother to check the rest
  if (false == bValid)
  {
    //std::cout << "Piece is not allowed to move to that square\n";
    return false;
  }


  // -------------------------------------------------------------------------
  // 2. Is there another piece of the same color on the destination square?
  // -------------------------------------------------------------------------
  if (isSquareOccupied(future.iRow, future.iColumn))
  {
    //char chAuxPiece = getPieceAtPosition(future.iRow, future.iColumn);
    Figure* auxFigure = getFigureAtPosition(future.iRow, future.iColumn);
    int auxColor = (auxFigure->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
    int figureColor = (figure->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
    if (figureColor == auxColor)
    {
      //std::cout << "Position is already taken by a piece of the same color\n";
      return false;
    }
  }

  // ----------------------------------------------
  // 3. Would the king be in check after the move?
  // ----------------------------------------------
  if (true == wouldKingBeInCheck(figure, present, future, S_enPassant))
  {
    //std::cout << "Move would put player's king in check\n";
    return false;
  }

  return bValid;
}
