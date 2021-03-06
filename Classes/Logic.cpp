#include "Logic.h"
#include "GameLayer.h"
#include "Board.h"
#include "Piece.h"
#include "Constants.h"

#include <iostream>
#include <stdlib.h> 
#include <string>
#include <deque>

USING_NS_CC;

Logic::Logic()
{

}

Logic::Logic(GameLayer* gameLayer)
  : m_gameLayer(gameLayer)
{
  // Board presentation
  for (int i = 0; i < Constants::ROWS; i++)
  {
    for (int j = 0; j < Constants::COLUMNS; j++)
    {
      m_boardA[i][j] = Constants::INITIAL_PIECE_BOARD[i][j];
    }
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

  // -------------------------------------------------------------------
   // Game class
   // -------------------------------------------------------------------
   // White player always starts
  m_currentTurn = static_cast<int>(Player::WHITE_PLAYER);

  // Game on!
  m_bGameFinished = false;

  // Castling is allowed (to each side) until the player moves the king or the rook
  m_bCastlingKingSideAllowed[static_cast<int>(Player::WHITE_PLAYER)] = true;
  m_bCastlingKingSideAllowed[static_cast<int>(Player::BLACK_PLAYER)] = true;

  m_bCastlingQueenSideAllowed[static_cast<int>(Player::WHITE_PLAYER)] = true;
  m_bCastlingQueenSideAllowed[static_cast<int>(Player::BLACK_PLAYER)] = true;

  return true;
}

void Logic::initUndo(Undo& undo)
{
  // Nothing has happend yet
  undo.bCapturedLastMove = false;
  undo.bCanUndo = false;
  undo.bCastlingKingSideAllowed = false;
  undo.bCastlingQueenSideAllowed = false;
  undo.en_passant.bApplied = false;
  undo.castling.bApplied = false;
}

void Logic::movePiece(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion)
{
  Undo curUndo;
  initUndo(curUndo);

  // Get the piece to be moved
  int iPiece = getPieceAtPositionI(present.iRow, present.iColumn);

  // Is the destination square occupied?
  int iCapturedPiece = getPieceAtPositionI(future.iRow, future.iColumn);

  // So, was a piece captured in this move?
  if (Constants::EMPTY_SQUAREI != iCapturedPiece)
  {
    if (Piece::isWhite(iCapturedPiece))
    {
      // A white piece was captured
      iwhite_captured.push_back(iCapturedPiece);
    }
    else
    {
      // A black piece was captured
      iblack_captured.push_back(iCapturedPiece);
    }

    // Set Undo structure. If a piece was captured, then no "en passant" move performed
    curUndo.bCapturedLastMove = true;

    // Reset m_undo.castling
    memset(&curUndo.en_passant, 0, sizeof(EnPassant));
  }
  else if (true == S_enPassant->bApplied)
  {
    int iCapturedEP = getPieceAtPositionI(S_enPassant->PawnCaptured.iRow, S_enPassant->PawnCaptured.iColumn);

    if (Piece::isWhite(iCapturedEP))
    {
      iwhite_captured.push_back(iCapturedEP);
    }
    else
    {
      iblack_captured.push_back(iCapturedEP);
    }

    // Now, remove the captured pawn
    m_boardA[S_enPassant->PawnCaptured.iRow][S_enPassant->PawnCaptured.iColumn] = Constants::EMPTY_SQUAREI;
    m_deletePiece(Size(S_enPassant->PawnCaptured.iRow, S_enPassant->PawnCaptured.iColumn));

    // Set Undo structure as piece was captured and "en passant" move was performed
    curUndo.bCapturedLastMove = true;
    memcpy(&curUndo.en_passant, S_enPassant, sizeof(EnPassant));
  }
  else
  {
    curUndo.bCapturedLastMove = false;

    // Reset m_undo.castling
    memset(&curUndo.en_passant, 0, sizeof(EnPassant));
  }

  // Remove piece from present position
  m_boardA[present.iRow][present.iColumn] = Constants::EMPTY_SQUAREI;

  // Remove piece from future position on board
  if (Constants::EMPTY_SQUAREI != iCapturedPiece)
    m_deletePiece(Size(future.iRow, future.iColumn));

  // Move piece to new position
  if (S_promotion->bApplied)
  {
    m_boardA[future.iRow][future.iColumn] = S_promotion->typeAfter;

    S_promotion->typeBefore = iPiece;

	m_movePiece(Size(present.iRow, present.iColumn), Size(future.iRow, future.iColumn));

    // Set Undo structure as a promotion occured
    memcpy(&curUndo.promotion, S_promotion, sizeof(Promotion));
  }
  else
  {
    m_boardA[future.iRow][future.iColumn] = iPiece;

	m_movePiece(Size(present.iRow, present.iColumn), Size(future.iRow, future.iColumn));

    // Reset m_undo.promotion
    memset(&curUndo.promotion, 0, sizeof(Promotion));
  }

  // Was it a castling move?
  if (S_castling->bApplied == true)
  {
    // The king was already move, but we still have to move the rook to 'jump' the king
    int iPiece = getPieceAtPositionI(S_castling->rook_before.iRow, S_castling->rook_before.iColumn);

    // Remove the rook from present position
    m_boardA[S_castling->rook_before.iRow][S_castling->rook_before.iColumn] = Constants::EMPTY_SQUAREI;
    m_boardA[S_castling->rook_after.iRow][S_castling->rook_after.iColumn] = iPiece;

    m_movePiece(Size(S_castling->rook_before.iRow, S_castling->rook_before.iColumn), Size(S_castling->rook_after.iRow, S_castling->rook_after.iColumn));

    // Write this information to the m_undo struct
    memcpy(&curUndo.castling, S_castling, sizeof(Castling));

    // Save the 'CastlingAllowed' information in case the move is undone
    curUndo.bCastlingKingSideAllowed = m_bCastlingKingSideAllowed[getCurrentTurn()];
    curUndo.bCastlingQueenSideAllowed = m_bCastlingQueenSideAllowed[getCurrentTurn()];
  }
  else
  {
    // Reset m_undo.castling
    memset(&curUndo.castling, 0, sizeof(Castling));
  }

  // Castling requirements
  if (TypePiece::KING == Piece::getTypeP(abs(iPiece)))
  {
    // After the king has moved once, no more castling allowed
    m_bCastlingKingSideAllowed[getCurrentTurn()] = false;
    m_bCastlingQueenSideAllowed[getCurrentTurn()] = false;
  }
  else if (TypePiece::ROOK == Piece::getTypeP(abs(iPiece)))
  {
    // If the rook moved from column 'A', no more castling allowed on the queen side
    if (0 == present.iColumn)
    {
      m_bCastlingQueenSideAllowed[getCurrentTurn()] = false;
    }

    // If the rook moved from column 'A', no more castling allowed on the queen side
    else if (7 == present.iColumn)
    {
      m_bCastlingKingSideAllowed[getCurrentTurn()] = false;
    }
  }

  // Change turns
  changeTurns();

  // This move can be undone
  curUndo.bCanUndo = true;

  m_undos.push(curUndo);
  m_currentUndo = curUndo;
}

void Logic::undoLastMove()
{
  Undo curUndo = m_undos.top();

  if (m_undos.size() <= 0) return;

  std::string last_move = getLastMove();

  // Parse the line
  Position from;
  Position to;

  parseMoveStringToCell(last_move, &from, &to);

  // Since we want to undo a move, we will be moving the piece from (iToRow, iToColumn) to (iFromRow, iFromColumn)
  int iPiece = getPieceAtPositionI(to.iRow, to.iColumn);

  // Moving it back
  // If there was a promotion
  if (curUndo.promotion.bApplied)
  {
    // Delete piece from board
    m_deletePiece(Size(to.iRow, to.iColumn));

    // Add piece to the board
    m_addPiece(abs(curUndo.promotion.typeBefore), Piece::isWhite(curUndo.promotion.typeBefore), Size(to.iRow, to.iColumn));

    m_boardA[from.iRow][from.iColumn] = curUndo.promotion.typeBefore;
  }
  else
  {
    m_boardA[from.iRow][from.iColumn] = iPiece;
  }

  m_movePiece(Size(to.iRow, to.iColumn), Size(from.iRow, from.iColumn));

  // Change turns
  changeTurns();

  // If a piece was captured, move it back to the board
  if (curUndo.bCapturedLastMove)
  {
    // Let's retrieve the last captured piece
    int iCaptured;

    // Since we already changed turns back, it means we should we pop a piece from the oponents vector
    if (static_cast<int>(Player::WHITE_PLAYER) == m_currentTurn)
    {
      iCaptured = iblack_captured.back();
      iblack_captured.pop_back();
    }
    else
    {
      iCaptured = iwhite_captured.back();
      iwhite_captured.pop_back();
    }


    // Move the captured piece back. Was this an "en passant" move?
    if (curUndo.en_passant.bApplied)
    {
      // Remove the attacker
      m_boardA[to.iRow][to.iColumn] = Constants::EMPTY_SQUAREI;


      // Move the captured piece back
      m_boardA[curUndo.en_passant.PawnCaptured.iRow][curUndo.en_passant.PawnCaptured.iColumn] = iCaptured;

      //Add captured piece to board
      m_addPiece(abs(iCaptured), Piece::isWhite(iCaptured), Size(curUndo.en_passant.PawnCaptured.iRow, curUndo.en_passant.PawnCaptured.iColumn));
    }
    else
    {
      m_boardA[to.iRow][to.iColumn] = iCaptured;

      //Add captured piece to board
      m_addPiece(abs(iCaptured), Piece::isWhite(iCaptured), Size(to.iRow, to.iColumn));
    }
  }
  else
  {
    m_boardA[to.iRow][to.iColumn] = Constants::EMPTY_SQUAREI;
  }

  // If there was a castling
  if (curUndo.castling.bApplied)
  {
    int iRook = getPieceAtPositionI(curUndo.castling.rook_after.iRow, curUndo.castling.rook_after.iColumn);

    // Remove the rook from present position
    m_boardA[curUndo.castling.rook_after.iRow][curUndo.castling.rook_after.iColumn] = Constants::EMPTY_SQUAREI;

    // 'Jump' into to new position
    m_boardA[curUndo.castling.rook_before.iRow][curUndo.castling.rook_before.iColumn] = iRook;

    // Move rook on the board
    m_movePiece(Size(curUndo.castling.rook_after.iRow, curUndo.castling.rook_after.iColumn), Size(curUndo.castling.rook_before.iRow, curUndo.castling.rook_before.iColumn));

    // Restore the values of castling allowed or not
    m_bCastlingKingSideAllowed[getCurrentTurn()] = curUndo.bCastlingKingSideAllowed;
    m_bCastlingQueenSideAllowed[getCurrentTurn()] = curUndo.bCastlingQueenSideAllowed;
  }

  // If it was a checkmate, toggle back to game not finished
  m_bGameFinished = false;

  //  CallBack
  if (m_undoLastMove)
    m_undoLastMove(Size(from.iRow, from.iColumn), Size(to.iRow, to.iColumn));

  // Finally, remove the last move from the list
  deleteLastMove();
}

void Logic::changeTurns(void)
{
  if (static_cast<int>(Player::WHITE_PLAYER) == m_currentTurn)
  {
    m_currentTurn = static_cast<int>(Player::BLACK_PLAYER);
  }
  else
  {
    m_currentTurn = static_cast<int>(Player::WHITE_PLAYER);
  }
}

bool Logic::undoIsPossible()
{
  if (m_undos.size() > 0 && m_currentUndo.bCanUndo == true)
  {
    return true;
  }

  return false;
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

int Logic::getPieceAtPositionI(int i, int j)
{
  return m_boardA[i][j];
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

std::string Logic::getPenultMove()
{
  std::string penult_move = "d7d5";

  if (rounds.size() >= 2)
  {
    // Who did the penult move?
    if (Player::WHITE_PLAYER == static_cast<Player>(getCurrentTurn()))
    {
      // If it's black's turn now, white had the last move
      penult_move = rounds[rounds.size() - 2].white_move;
    }
    else
    {
      // Last move was black's
      penult_move = rounds[rounds.size() - 2].black_move;
    }
  }

  return penult_move;
}

int Logic::getCurrentTurn()
{
  return m_currentTurn;
}

int Logic::getOppositCurrentTurn()
{
  int oppositCurrentTurn = (m_currentTurn == static_cast<int>(Player::WHITE_PLAYER)) ? static_cast<int>(Player::BLACK_PLAYER) : static_cast<int>(Player::WHITE_PLAYER);
  return oppositCurrentTurn;
}

std::string Logic::parseMoveCellIntToString(const Position& pos)
{
  // A B C D E F G H - COLUMNS
  std::string s = "";
  s += char(pos.iColumn + 97);
  s += char(pos.iRow + 49);
  return s;
}

void Logic::parseMoveStringToCell(std::string move, Position* pFrom, Position* pTo)
{
  // a5a9
  pFrom->iColumn = static_cast<int>(move[0]) - 97;
  pFrom->iRow = static_cast<int>(move[1]) - 49;
  pTo->iColumn = static_cast<int>(move[2]) - 97;
  pTo->iRow = static_cast<int>(move[3]) - 49;
}

std::array<std::array<int, 8>, 8>& Logic::getBoardA()
{
  return m_boardA;
}

void Logic::updateBoardA(int typePiece, cocos2d::Size& cellUpdate)
{
  m_boardA[cellUpdate.width][cellUpdate.height] = typePiece;
}

void Logic::loadBoard(const int piece_board[8][8])
{
  for (unsigned int i = 0; i < m_boardA.size(); i++)
  {
    std::array<int, 8> row = m_boardA[i];
    for (unsigned int j = 0; j < row.size(); j++)
    {
      row[j] = piece_board[i][j];
    }
  }
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
      if (isSquareOccupied(startingPos.iRow, startingPos.iColumn))
      {
        bFree = false;
        //throw("Error. Diagonal move not allowed\n");
        std::cout << "Diagonal move not allowed\n";
      }
    }
  }
  break;
  }

  return bFree;
}

bool Logic::isReachable(int iRow, int iColumn, int iColor)
{
  bool bReachable = false;

  // a) Direction: HORIZONTAL
  {
    // Check all the way to the right
    for (int i = iColumn + 1; i < 8; i++)
    {
      int iPieceFound = getPieceAtPositionI(iRow, i);
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
      {
        // There is a queen or a rook to the right, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move horizontally
        break;
      }
    }

    // Check all the way to the left
    for (int i = iColumn - 1; i >= 0; i--)
    {
      int iPieceFound = getPieceAtPositionI(iRow, i);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
      {
        // There is a queen or a rook to the left, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move horizontally
        break;
      }
    }
  }

  // b) Direction: VERTICAL
  {
    // Check all the way up
    for (int i = iRow + 1; i < 8; i++)
    {
      int iPieceFound = getPieceAtPositionI(i, iColumn);
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::PAWN) &&
        (static_cast<int>(Piece::getColor(iPieceFound)) == static_cast<int>(PieceColor::BLACK_PIECE)) &&
        (i == iRow + 1))
      {
        // There is a pawn one square up, so the square is reachable
        bReachable = true;
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
      {
        // There is a queen or a rook on the way up, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move vertically
        break;
      }
    }

    // Check all the way down
    for (int i = iRow - 1; i >= 0; i--)
    {
      int iPieceFound = getPieceAtPositionI(i, iColumn);
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::PAWN) &&
        (static_cast<int>(Piece::getColor(iPieceFound)) == static_cast<int>(PieceColor::WHITE_PIECE)) &&
        (i == iRow - 1))
      {
        // There is a pawn one square down, so the square is reachable
        bReachable = true;
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
      {
        // There is a queen or a rook on the way down, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move vertically
        break;
      }
    }
  }

  // c) Direction: DIAGONAL
  {
    // Check the diagonal up-right
    for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
    {
      int iPieceFound = getPieceAtPositionI(i, j);
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP))
      {
        // There is a queen or a bishop in that direction, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move diagonally
        break;
      }
    }

    // Check the diagonal up-left
    for (int i = iRow + 1, j = iColumn - 1; i < 8 && j > 0; i++, j--)
    {
      int iPieceFound = getPieceAtPositionI(i, j);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP))
      {
        // There is a queen or a bishop in that direction, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move diagonally
        break;
      }
    }

    // Check the diagonal down-right
    for (int i = iRow - 1, j = iColumn + 1; i > 0 && j < 8; i--, j++)
    {
      int iPieceFound = getPieceAtPositionI(i, j);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP))
      {
        // There is a queen or a bishop in that direction, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move diagonally
        break;
      }
    }

    // Check the diagonal down-left
    for (int i = iRow - 1, j = iColumn - 1; i > 0 && j > 0; i--, j--)
    {
      int iPieceFound = getPieceAtPositionI(i, j);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP))
      {
        // There is a queen or a bishop in that direction, so the square is reachable
        bReachable = true;
        break;
      }
      else
      {
        // There is a piece that does not move diagonally
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

      int iPieceFound = getPieceAtPositionI(iRowToTest, iColumnToTest);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
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
}


bool Logic::isSquareOccupied(int iRow, int iColumn)
{
  bool bOccupied = false;

  if (Constants::EMPTY_SQUAREI != getPieceAtPositionI(iRow, iColumn))
  {
    bOccupied = true;
  }

  return bOccupied;
}

bool Logic::wouldKingBeInCheck(int iPiece, Position present, Position future, EnPassant* S_enPassant)
{
  IntendedMove intended_move;

  //intended_move.piece = piece;
  intended_move.iPiece = iPiece;
  intended_move.from.iRow = present.iRow;
  intended_move.from.iColumn = present.iColumn;
  intended_move.to.iRow = future.iRow;
  intended_move.to.iColumn = future.iColumn;

  return playerKingInCheck(&intended_move);
}

Position Logic::findKing(int iColor)
{
  Position king = { 0 };

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      int iPiece = getPieceAtPositionI(i, j);
      bool isWhitePiece = static_cast<bool>(Piece::getColor(iPiece));
      if (static_cast<TypePiece>(abs(iPiece)) == TypePiece::KING && iColor == static_cast<int>(Piece::getColor(iPiece)))
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
  if (nullptr != pintended_move && TypePiece::KING == static_cast<TypePiece>(abs(pintended_move->iPiece)))
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
  int colorKing = getCurrentTurn();
  return isKingInCheck(static_cast<int>(getCurrentTurn()), intended_move);
}

int Logic::getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move)
{
  //char chPiece;
  int iPiece;
  //Piece* piece;

  // If there is no intended move, just return the current position of the board
  if (nullptr == intended_move)
  {
    //piece = getPieceAtPosition(iRow, iColumn);
    iPiece = getPieceAtPositionI(iRow, iColumn);
  }
  else
  {
    // In this case, we are trying to understand what WOULD happed if the move was made,
    // so we consider a move that has not been made yet
    if (intended_move->from.iRow == iRow && intended_move->from.iColumn == iColumn)
    {
      // The Piece wants to move from that square, so it would be empty
      //piece = Constants::EMPTY_SQUARE;
      iPiece = Constants::EMPTY_SQUAREI;
    }
    else if (intended_move->to.iRow == iRow && intended_move->to.iColumn == iColumn)
    {
      // The piece wants to move to that square, so return the piece
      //piece = intended_move->piece;
      iPiece = intended_move->iPiece;
    }
    else
    {
      //piece = getPieceAtPosition(iRow, iColumn);
      iPiece = getPieceAtPositionI(iRow, iColumn);
    }
  }

  return iPiece;
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
      //Piece* chPieceFound = getPiece_considerMove(iRow, i, pintended_move);
      int iPieceFound = getPiece_considerMove(iRow, i, pintended_move);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      //int chPieceColor = (chPieceFound->isWhite()) ? static_cast<int>(PieceColor::WHITE_PIECE) : static_cast<int>(PieceColor::BLACK_PIECE);
      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
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
      //Piece* chPieceFound = getPiece_considerMove(iRow, i, pintended_move);
      int iPieceFound = getPiece_considerMove(iRow, i, pintended_move);
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      //int chPieceColor = (chPieceFound->isWhite()) ? static_cast<int>(PieceColor::WHITE_PIECE) : static_cast<int>(PieceColor::BLACK_PIECE);
      int iPieceColor = static_cast<int>(Piece::getColor(iPieceFound));
      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
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
      int iPieceFound = getPiece_considerMove(i, iColumn, pintended_move);
      if (Constants::EMPTY_SQUAREI == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
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
      
      int iPieceFound = getPiece_considerMove(i, iColumn, pintended_move);
      
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::ROOK))
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
      int iPieceFound = getPiece_considerMove(i, j, pintended_move);

      if (Constants::EMPTY_SQUAREI == abs(iPieceFound))
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::PAWN) &&
        (i == iRow + 1) &&
        (j == iColumn + 1) &&
        (iColor == static_cast<int>(PieceColor::WHITE_PIECE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP))
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
    for (int i = iRow + 1, j = iColumn - 1; i < 8 && j >= 0; i++, j--)
    {
      int iPieceFound = getPiece_considerMove(i, j, pintended_move);
      
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (static_cast<PieceColor>(iColor) == Piece::getColor(iPieceFound))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::PAWN) &&
        (i == iRow + 1) &&
        (j == iColumn - 1) &&
        (iColor == static_cast<int>(PieceColor::WHITE_PIECE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        (static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP))
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
      int iPieceFound = getPiece_considerMove(i, j, pintended_move);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::PAWN) &&
        (i == iRow - 1) &&
        (j == iColumn + 1) &&
        (iColor == static_cast<int>(PieceColor::BLACK_PIECE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP)))
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
    for (int i = iRow - 1, j = iColumn - 1; i >= 0 && j >= 0; i--, j--)
    {
      int iPieceFound = getPiece_considerMove(i, j, pintended_move);

      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::PAWN) &&
        (i == iRow - 1) &&
        (j == iColumn - 1) &&
        (iColor == static_cast<int>(PieceColor::BLACK_PIECE)))
      {
        // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
        attack.bUnderAttack = true;
        attack.iNumAttackers += 1;

        attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
        attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
        attack.attacker[attack.iNumAttackers - 1].dir = Direction::DIAGONAL;
        break;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::QUEEN) ||
        ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::BISHOP)))
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

      int iPieceFound = getPiece_considerMove(iRowToTest, iColumnToTest, pintended_move);
      if (Constants::EMPTY_SQUAREI == iPieceFound)
      {
        // This square is empty, move on
        continue;
      }

      if (iColor == static_cast<int>(Piece::getColor(iPieceFound)))
      {
        // This is a piece of the same color, so no problem
        continue;
      }
      else if ((static_cast<TypePiece>(abs(iPieceFound)) == TypePiece::KNIGHT))
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

  m_undos.pop();
}

int Logic::getOpponentColor(void)
{
  int iColor;

  if (static_cast<int>(Player::WHITE_PLAYER) == getCurrentTurn())
  {
    iColor = static_cast<int>(Player::BLACK_PLAYER);
  }
  else
  {
    iColor = static_cast<int>(Player::WHITE_PLAYER);
  }

  return iColor;
}

bool Logic::canBeBlocked(Position startingPos, Position finishingPos, int iDirection)
{
  bool bBlocked = false;

  UnderAttack blocker = { 0 };

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
      for (int i = startingPos.iColumn + 1; i < finishingPos.iColumn; i++)
      {
        if (true == isReachable(startingPos.iRow, i, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
        }
      }
    }

    // Moving to the left
    else //if (startingPos.iColumn > finishingPos.iColumn)
    {
      for (int i = startingPos.iColumn - 1; i > finishingPos.iColumn; i--)
      {
        if (true == isReachable(startingPos.iRow, i, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
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
      //std::cout << "Error. Movement is vertical but row is the same\n";
      throw("Error. Movement is vertical but row is the same");
    }

    // Moving up
    else if (startingPos.iRow < finishingPos.iRow)
    {
      for (int i = startingPos.iRow + 1; i < finishingPos.iRow; i++)
      {
        if (true == isReachable(i, startingPos.iColumn, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
        }
      }
    }

    // Moving down
    else //if (startingPos.iColumn > finishingPos.iRow)
    {
      for (int i = startingPos.iRow - 1; i > finishingPos.iRow; i--)
      {
        if (true == isReachable(i, startingPos.iColumn, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
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
      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (true == isReachable(startingPos.iRow + i, startingPos.iColumn + i, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
        }
      }
    }

    // Moving up and left
    else if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
    {
      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (true == isReachable(startingPos.iRow + i, startingPos.iColumn - i, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
        }
      }
    }

    // Moving down and right
    else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
    {
      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (true == isReachable(startingPos.iRow - i, startingPos.iColumn + i, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
        }
      }
    }

    // Moving down and left
    else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
    {
      for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
      {
        if (true == isReachable(startingPos.iRow - i, startingPos.iColumn - i, getOpponentColor()))
        {
          // Some piece can block the way
          bBlocked = true;
        }
      }
    }

    else
    {
      //std::cout << "Error. Diagonal move not allowed\n";
      throw("Error. Diagonal move not allowed");
    }
  }
  break;
  }

  return bBlocked;
}

bool Logic::isCheckMate()
{
  bool bCheckmate = false;

  // 1. First of all, it the king in check?
  if (false == playerKingInCheck())
  {
    return false;
  }

  // 2. Can the king move the other square?
  Position king_moves[8] = { {  1, -1 },{  1, 0 },{  1,  1 }, { 0,  1 },
                                     { -1,  1 },{ -1, 0 },{ -1, -1 }, { 0, -1 } };

  Position king = findKing(getCurrentTurn());

  for (int i = 0; i < 8; i++)
  {
    int iRowToTest = king.iRow + king_moves[i].iRow;
    int iColumnToTest = king.iColumn + king_moves[i].iColumn;

    if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
    {
      // This square does not even exist, so no need to test
      continue;
    }

    if (Constants::EMPTY_SQUAREI != getPieceAtPositionI(iRowToTest, iColumnToTest))
    {
      // That square is not empty, so no need to test
      continue;
    }

    IntendedMove intended_move;
    //intended_move.piece = getPieceAtPosition(king.iRow, king.iColumn);
    intended_move.iPiece = getPieceAtPositionI(king.iRow, king.iColumn);
    intended_move.from.iRow = king.iRow;
    intended_move.from.iColumn = king.iColumn;
    intended_move.to.iRow = iRowToTest;
    intended_move.to.iColumn = iColumnToTest;

    // Now, for every possible move of the king, check if it would be in jeopardy
    // Since the move has already been made, current_game->getCurrentTurn() now will return 
    // the next player's color. And it is in fact this king that we want to check for jeopardy
    UnderAttack king_moved = isUnderAttack(iRowToTest, iColumnToTest, getCurrentTurn(), &intended_move);

    if (false == king_moved.bUnderAttack)
    {
      // This means there is at least one position when the king would not be in jeopardy, so that's not a checkmate
      return false;
    }
  }

  // 3. Can the attacker be taken or another piece get into the way? 
  UnderAttack king_attacked = isUnderAttack(king.iRow, king.iColumn, getCurrentTurn(), nullptr);
  if (1 == king_attacked.iNumAttackers)
  {
    // Can the attacker be taken?
    UnderAttack king_attacker = { 0 };
    king_attacker = isUnderAttack(king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn, getOpponentColor(), nullptr);

    if (true == king_attacker.bUnderAttack)
    {
      // This means that the attacker can be taken, so it's not a checkmate
      return false;
    }
    else
    {
      // Last resort: can any piece get in between the attacker and the king?
      int iPieceAttacker = getPieceAtPositionI(king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn);

      TypePiece typePiece = static_cast<TypePiece>(abs(iPieceAttacker));

      switch (typePiece)
      {
      case TypePiece::PAWN:
      case TypePiece::KNIGHT:
      {
        // If it's a pawn, there's no space in between the attacker and the king
        // If it's a knight, it doesn't matter because the knight can 'jump'
        // So, this is checkmate
        bCheckmate = true;
      }
      break;

      case TypePiece::BISHOP:
      {
        if (false == canBeBlocked(king_attacked.attacker[0].pos, king, static_cast<int>(Direction::DIAGONAL)))
        {
          // If no piece can get in the way, it's a checkmate
          bCheckmate = true;
        }
      }
      break;

      case TypePiece::ROOK:
      {
        if (false == canBeBlocked(king_attacked.attacker[0].pos, king, static_cast<int>(king_attacked.attacker[0].dir)))
        {
          // If no piece can get in the way, it's a checkmate
          bCheckmate = true;
        }
      }
      break;

      case TypePiece::QUEEN:
      {
        if (false == canBeBlocked(king_attacked.attacker[0].pos, king, static_cast<int>(king_attacked.attacker[0].dir)))
        {
          // If no piece can get in the way, it's a checkmate
          bCheckmate = true;
        }
      }
      break;


      default:
      {
        throw("!!!!Should not reach here. Invalid piece");
      }
      break;
      }
    }
  }
  else
  {
    // If there is more than one attacker and we reached this point, it's a checkmate
    bCheckmate = true;
  }

  // If the game has ended, store in the class variable
  m_bGameFinished = bCheckmate;

  return bCheckmate;
}

void Logic::callBackAddPiece(const std::function<void(int type, bool isWhite, const cocos2d::Size& futureCell)> addPiece)
{
  m_addPiece = addPiece;
}

void Logic::callBackDeletePiece(const std::function<void(const cocos2d::Size& presentCell)> deletePiece)
{
  m_deletePiece = deletePiece;
}

void Logic::callBackMovePiece(const std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> movePiece)
{
  m_movePiece = movePiece;
}

void Logic::callBackUndoLastMove(const std::function<void(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> undoLastMove)
{
  m_undoLastMove = undoLastMove;
}

/*void Logic::callBackPromotion(const std::function<int(const cocos2d::Size& futureCell)> promotion)
{
  m_promotion = promotion;
}*/

