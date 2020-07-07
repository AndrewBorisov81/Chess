#include "FiguresMoveLogic.h"
#include "GameLayer.h"
#include "TouchAndDragLayer.h"
#include "Figure.h"

USING_NS_CC;

FiguresMoveLogic::FiguresMoveLogic() : Node()
{

}

FiguresMoveLogic::FiguresMoveLogic(GameLayer* gameLayer) 
  : m_gameLayer(gameLayer),
    Node()
{

}

FiguresMoveLogic::~FiguresMoveLogic()
{
}

bool FiguresMoveLogic::isMoveValid(Position current, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion)
{
  bool bValid = false;

  //char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);
  Figure* figure = m_gameLayer->getTouchAndDragLayer()->getFigure();
  TypeFigure typeFigure = figure->getType();

  // ----------------------------------------------------
  // 1. Is the piece  allowed to move in that direction?
  // ----------------------------------------------------
  /*switch (toupper(chPiece))
  {
  case 'P':
  {
    // Wants to move forward
    if (future.iColumn == present.iColumn)
    {
      // Simple move forward
      if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) ||
        (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1))
      {
        if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn))
        {
          bValid = true;
        }
      }

      // Double move forward
      else if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 2) ||
        (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 2))
      {
        // This is only allowed if the pawn is in its original place
        if (Chess::isWhitePiece(chPiece))
        {
          if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow - 1, future.iColumn) &&
            EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn) &&
            1 == present.iRow)
          {
            bValid = true;
          }
        }
        else // if ( isBlackPiece(chPiece) )
        {
          if (EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow + 1, future.iColumn) &&
            EMPTY_SQUARE == current_game->getPieceAtPosition(future.iRow, future.iColumn) &&
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
    else if ((Chess::isWhitePiece(chPiece) && 4 == present.iRow && 5 == future.iRow && 1 == abs(future.iColumn - present.iColumn)) ||
      (Chess::isBlackPiece(chPiece) && 3 == present.iRow && 2 == future.iRow && 1 == abs(future.iColumn - present.iColumn)))
    {
      // It is only valid if last move of the opponent was a double move forward by a pawn on a adjacent column
      string last_move = current_game->getLastMove();

      // Parse the line
      Chess::Position LastMoveFrom;
      Chess::Position LastMoveTo;
      current_game->parseMove(last_move, &LastMoveFrom, &LastMoveTo);

      // First of all, was it a pawn?
      char chLstMvPiece = current_game->getPieceAtPosition(LastMoveTo.iRow, LastMoveTo.iColumn);

      if (toupper(chLstMvPiece) != 'P')
      {
        return false;
      }

      // Did the pawn have a double move forward and was it an adjacent column?
      if (2 == abs(LastMoveTo.iRow - LastMoveFrom.iRow) && 1 == abs(LastMoveFrom.iColumn - present.iColumn))
      {
        cout << "En passant move!\n";
        bValid = true;

        S_enPassant->bApplied = true;
        S_enPassant->PawnCaptured.iRow = LastMoveTo.iRow;
        S_enPassant->PawnCaptured.iColumn = LastMoveTo.iColumn;
      }
    }

    // Wants to capture a piece
    else if (1 == abs(future.iColumn - present.iColumn))
    {
      if ((Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) || (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1))
      {
        // Only allowed if there is something to be captured in the square
        if (EMPTY_SQUARE != current_game->getPieceAtPosition(future.iRow, future.iColumn))
        {
          bValid = true;
          cout << "Pawn captured a piece!\n";
        }
      }
    }
    else
    {
      // This is invalid
      return false;
    }

    // If a pawn reaches its eight rank, it must be promoted to another piece
    if ((Chess::isWhitePiece(chPiece) && 7 == future.iRow) ||
      (Chess::isBlackPiece(chPiece) && 0 == future.iRow))
    {
      cout << "Pawn must be promoted!\n";
      S_promotion->bApplied = true;
    }
  }
  break;

  case 'R':
  {
    // Horizontal move
    if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
    {
      // Check if there are no pieces on the way
      if (current_game->isPathFree(present, future, Chess::HORIZONTAL))
      {
        bValid = true;
      }
    }
    // Vertical move
    else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
    {
      // Check if there are no pieces on the way
      if (current_game->isPathFree(present, future, Chess::VERTICAL))
      {
        bValid = true;
      }
    }
  }
  break;

  case 'N':
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

  case 'B':
  {
    // Diagonal move
    if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
    {
      // Check if there are no pieces on the way
      if (current_game->isPathFree(present, future, Chess::DIAGONAL))
      {
        bValid = true;
      }
    }
  }
  break;

  case 'Q':
  {
    // Horizontal move
    if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn))
    {
      // Check if there are no pieces on the way
      if (current_game->isPathFree(present, future, Chess::HORIZONTAL))
      {
        bValid = true;
      }
    }
    // Vertical move
    else if ((future.iRow != present.iRow) && (future.iColumn == present.iColumn))
    {
      // Check if there are no pieces on the way
      if (current_game->isPathFree(present, future, Chess::VERTICAL))
      {
        bValid = true;
      }
    }

    // Diagonal move
    else if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
    {
      // Check if there are no pieces on the way
      if (current_game->isPathFree(present, future, Chess::DIAGONAL))
      {
        bValid = true;
      }
    }
  }
  break;

  case 'K':
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
      if (true == current_game->playerKingInCheck())
      {
        return false;
      }

      // 2. No pieces in between the king and the rook
      if (false == current_game->isPathFree(present, future, Chess::HORIZONTAL))
      {
        return false;
      }

      // 3. King and rook must not have moved yet;
      // 4. King must not pass through a square that is attacked by an enemy piece
      if (future.iColumn > present.iColumn)
      {
        // if future.iColumn is greather, it means king side
        if (false == current_game->castlingAllowed(Chess::Side::KING_SIDE, Chess::getPieceColor(chPiece)))
        {
          createNextMessage("Castling to the king side is not allowed.\n");
          return false;
        }
        else
        {
          // Check if the square that the king skips is not under attack
          Chess::UnderAttack square_skipped = current_game->isUnderAttack(present.iRow, present.iColumn + 1, current_game->getCurrentTurn());
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
        if (false == current_game->castlingAllowed(Chess::Side::QUEEN_SIDE, Chess::getPieceColor(chPiece)))
        {
          createNextMessage("Castling to the queen side is not allowed.\n");
          return false;
        }
        else
        {
          // Check if the square that the king skips is not attacked
          Chess::UnderAttack square_skipped = current_game->isUnderAttack(present.iRow, present.iColumn - 1, current_game->getCurrentTurn());
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
    cout << "!!!!Should not reach here. Invalid piece: " << char(chPiece) << "\n\n\n";
  }
  break;
  }

  // If it is a move in an invalid direction, do not even bother to check the rest
  if (false == bValid)
  {
    cout << "Piece is not allowed to move to that square\n";
    return false;
  }


  // -------------------------------------------------------------------------
  // 2. Is there another piece of the same color on the destination square?
  // -------------------------------------------------------------------------
  /*if (current_game->isSquareOccupied(future.iRow, future.iColumn))
  {
    char chAuxPiece = current_game->getPieceAtPosition(future.iRow, future.iColumn);
    if (Chess::getPieceColor(chPiece) == Chess::getPieceColor(chAuxPiece))
    {
      cout << "Position is already taken by a piece of the same color\n";
      return false;
    }
  }*/

  // ----------------------------------------------
  // 3. Would the king be in check after the move?
  // ----------------------------------------------
  /*if (true == current_game->wouldKingBeInCheck(chPiece, present, future, S_enPassant))
  {
    cout << "Move would put player's king in check\n";
    return false;
  }*/

  return bValid;
}
