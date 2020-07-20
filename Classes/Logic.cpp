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

  // -------------------------------------------------------------------
   // Game class
   // -------------------------------------------------------------------
   // White player always starts
  //m_currentTurn = static_cast<int>(Player::WHITE_PLAYER);
  m_currentTurn = 0;

  // Game on!
  m_bGameFinished = false;

  // Nothing has happend yet
  m_undo.bCapturedLastMove = false;
  m_undo.bCanUndo = false;
  m_undo.bCastlingKingSideAllowed = false;
  m_undo.bCastlingQueenSideAllowed = false;
  m_undo.en_passant.bApplied = false;
  m_undo.castling.bApplied = false;

  // Initial board settings
  //memcpy(board, initial_board, sizeof(char) * 8 * 8);

  // Castling is allowed (to each side) until the player moves the king or the rook
  m_bCastlingKingSideAllowed[static_cast<int>(Player::WHITE_PLAYER)] = true;
  m_bCastlingKingSideAllowed[static_cast<int>(Player::BLACK_PLAYER)] = true;

  m_bCastlingQueenSideAllowed[static_cast<int>(Player::WHITE_PLAYER)] = true;
  m_bCastlingQueenSideAllowed[static_cast<int>(Player::BLACK_PLAYER)] = true;

  return true;
}

//---------------------------------------------------------------------------------------
// Commands
// Functions to handle the commands of the program
// New game, Move, Undo, Save game, Load game, etc
//---------------------------------------------------------------------------------------
void Logic::moveFigure(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion)
{
  // Get the piece to be moved
  //char chPiece = getPieceAtPosition(present);
  Figure* figure = getFigureAtPosition(present.iRow, present.iColumn);

  // Is the destination square occupied?
  //char chCapturedPiece = getPieceAtPosition(future);
  Figure* capturedFigure = getFigureAtPosition(future.iRow, future.iColumn);

  // So, was a piece captured in this move?
  //if (0x20 != chCapturedPiece)
  if(Constants::EMPTY_SQUARE != capturedFigure)
  {
    //if (WHITE_PIECE == getPieceColor(chCapturedPiece))
    if(capturedFigure->isWhite())
    {
      // A white piece was captured
      white_captured.push_back(capturedFigure);
    }
    else
    {
      // A black piece was captured
      black_captured.push_back(capturedFigure);
    }

    // Set Undo structure. If a piece was captured, then no "en passant" move performed
    m_undo.bCapturedLastMove = true;

    // Reset m_undo.castling
    memset(&m_undo.en_passant, 0, sizeof(EnPassant));
  }
  else if (true == S_enPassant->bApplied)
  {
    //char chCapturedEP = getPieceAtPosition(S_enPassant->PawnCaptured.iRow, S_enPassant->PawnCaptured.iColumn);
    Figure* capturedEPFigure = getFigureAtPosition(S_enPassant->PawnCaptured.iRow, S_enPassant->PawnCaptured.iColumn);

    //if (WHITE_PIECE == getPieceColor(chCapturedEP))
    if(capturedEPFigure->isWhite())
    {
      // A white piece was captured
      white_captured.push_back(capturedEPFigure);
    }
    else
    {
      // A black piece was captured
      black_captured.push_back(capturedEPFigure);
    }

    // Now, remove the captured pawn
    //board[S_enPassant->PawnCaptured.iRow][S_enPassant->PawnCaptured.iColumn] = EMPTY_SQUARE;
    m_gameLayer->removeFigureBoard(Size(S_enPassant->PawnCaptured.iRow, S_enPassant->PawnCaptured.iColumn));

    // Set Undo structure as piece was captured and "en passant" move was performed
    m_undo.bCapturedLastMove = true;
    memcpy(&m_undo.en_passant, S_enPassant, sizeof(EnPassant));
  }
  else
  {
    m_undo.bCapturedLastMove = false;

    // Reset m_undo.castling
    memset(&m_undo.en_passant, 0, sizeof(EnPassant));
  }

  // Remove piece from present position
  //board[present.iRow][present.iColumn] = EMPTY_SQUARE;
  m_gameLayer->removeFigureBoard(Size(present.iRow, present.iColumn));

  // Move piece to new position
  if (true == S_promotion->bApplied)
  {
    //board[future.iRow][future.iColumn] = S_promotion->chAfter;
    m_gameLayer->setFigureToNewPos(S_promotion->figureBefore, Size(future.iRow, future.iColumn));

    // Set Undo structure as a promotion occured
    memcpy(&m_undo.promotion, S_promotion, sizeof(Promotion));
  }
  else
  {
    //board[future.iRow][future.iColumn] = chPiece;
    m_gameLayer->setFigureToNewPos(figure, Size(future.iRow, future.iColumn));

    // Reset m_undo.promotion
    memset(&m_undo.promotion, 0, sizeof(Promotion));
  }

  // Was it a castling move?
  if (S_castling->bApplied == true)
  {
    // The king was already move, but we still have to move the rook to 'jump' the king
    //char chPiece = getPieceAtPosition(S_castling->rook_before.iRow, S_castling->rook_before.iColumn);
    Figure* figure = getFigureAtPosition(S_castling->rook_before.iRow, S_castling->rook_before.iColumn);

    // Remove the rook from present position
    //board[S_castling->rook_before.iRow][S_castling->rook_before.iColumn] = EMPTY_SQUARE;
    m_gameLayer->removeFigureBoard(Size(S_castling->rook_before.iRow, S_castling->rook_before.iColumn));

    // 'Jump' into to new position
    //board[S_castling->rook_after.iRow][S_castling->rook_after.iColumn] = chPiece;
    m_gameLayer->setFigureToNewPos(figure, Size(S_castling->rook_after.iRow, S_castling->rook_after.iColumn));

    // Write this information to the m_undo struct
    memcpy(&m_undo.castling, S_castling, sizeof(Castling));

    // Save the 'CastlingAllowed' information in case the move is undone
    m_undo.bCastlingKingSideAllowed = m_bCastlingKingSideAllowed[getCurrentTurn()];
    m_undo.bCastlingQueenSideAllowed = m_bCastlingQueenSideAllowed[getCurrentTurn()];
  }
  else
  {
    // Reset m_undo.castling
    memset(&m_undo.castling, 0, sizeof(Castling));
  }

  // Castling requirements
  //if ('K' == toupper(chPiece))
  if(TypeFigure::KING == figure->getType())
  {
    // After the king has moved once, no more castling allowed
    m_bCastlingKingSideAllowed[getCurrentTurn()] = false;
    m_bCastlingQueenSideAllowed[getCurrentTurn()] = false;
  }
  //else if ('R' == toupper(chPiece))
  else if (TypeFigure::ROOK == figure->getType())
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
  m_undo.bCanUndo = true;
}

void Logic::undoLastMove()
{
  std::string last_move = getLastMove();

  // Parse the line
  Position from;
  Position to;
  parseMove(last_move, &from, &to);

  // Since we want to undo a move, we will be moving the piece from (iToRow, iToColumn) to (iFromRow, iFromColumn)
  //char chPiece = getPieceAtPosition(to.iRow, to.iColumn);
  Figure* figure = getFigureAtPosition(to.iRow, to.iColumn);

  // Moving it back
  // If there was a castling
  if (true == m_undo.promotion.bApplied)
  {
    //board[from.iRow][from.iColumn] = m_undo.promotion.chBefore;
    m_gameLayer->setFigureToNewPos(m_undo.promotion.figureBefore, Size(from.iRow, from.iColumn));
  }
  else
  {
    //board[from.iRow][from.iColumn] = chPiece;
    m_gameLayer->setFigureToNewPos(figure, Size(from.iRow, from.iColumn));
  }

  // Change turns
  changeTurns();

  // If a piece was captured, move it back to the board
  if (m_undo.bCapturedLastMove)
  {
    // Let's retrieve the last captured piece
    //char chCaptured;
    Figure* capturedFigure;

    // Since we already changed turns back, it means we should we pop a piece from the oponents vector
    if (static_cast<int>(Player::WHITE_PLAYER) == m_currentTurn)
    {
      //chCaptured = black_captured.back();
      capturedFigure = black_captured.back();
      black_captured.pop_back();
    }
    else
    {
      //chCaptured = white_captured.back();
      capturedFigure = white_captured.back();
      white_captured.pop_back();
    }

    // Move the captured piece back. Was this an "en passant" move?
    if (m_undo.en_passant.bApplied)
    {
      // Move the captured piece back
      //board[m_undo.en_passant.PawnCaptured.iRow][m_undo.en_passant.PawnCaptured.iColumn] = chCaptured;
      m_gameLayer->setFigureToNewPos(capturedFigure, Size(m_undo.en_passant.PawnCaptured.iRow, m_undo.en_passant.PawnCaptured.iColumn));

      // Remove the attacker
      //board[to.iRow][to.iColumn] = EMPTY_SQUARE;
      m_gameLayer->removeFigureBoard(Size(to.iRow, to.iColumn));
    }
    else
    {
      //board[to.iRow][to.iColumn] = chCaptured;
      m_gameLayer->setFigureToNewPos(figure, Size(to.iRow, to.iColumn));
    }
  }
  else
  {
    //board[to.iRow][to.iColumn] = EMPTY_SQUARE;
    m_gameLayer->removeFigureBoard(Size(to.iRow, to.iColumn));
  }

  // If there was a castling
  if (m_undo.castling.bApplied)
  {
    //char chRook = getPieceAtPosition(m_undo.castling.rook_after.iRow, m_undo.castling.rook_after.iColumn);
    Figure* rookFigure = getFigureAtPosition(m_undo.castling.rook_after.iRow, m_undo.castling.rook_after.iColumn);

    // Remove the rook from present position
    //board[m_undo.castling.rook_after.iRow][m_undo.castling.rook_after.iColumn] = EMPTY_SQUARE;
    m_gameLayer->removeFigureBoard(Size(m_undo.castling.rook_after.iRow, m_undo.castling.rook_after.iColumn));

    // 'Jump' into to new position
    //board[m_undo.castling.rook_before.iRow][m_undo.castling.rook_before.iColumn] = chRook;
    m_gameLayer->setFigureToNewPos(rookFigure, Size(m_undo.castling.rook_before.iRow, m_undo.castling.rook_before.iColumn));

    // Restore the values of castling allowed or not
    m_bCastlingKingSideAllowed[getCurrentTurn()] = m_undo.bCastlingKingSideAllowed;
    m_bCastlingQueenSideAllowed[getCurrentTurn()] = m_undo.bCastlingQueenSideAllowed;
  }

  // Clean m_undo struct
  m_undo.bCanUndo = false;
  m_undo.bCapturedLastMove = false;
  m_undo.en_passant.bApplied = false;
  m_undo.castling.bApplied = false;
  m_undo.promotion.bApplied = false;

  // If it was a checkmate, toggle back to game not finished
  m_bGameFinished = false;

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
  return m_undo.bCanUndo;
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
  //Figure* figure = m_figures[i][j];
  DataChess& dataChess = m_gameLayer->getDataChess();
  return dataChess.figures[i][j];
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

bool Logic::isReachable(int iRow, int iColumn, int iColor)
{
  bool bReachable = false;

  // a) Direction: HORIZONTAL
  {
    // Check all the way to the right
    for (int i = iColumn + 1; i < 8; i++)
    {
      //char chPieceFound = getPieceAtPosition(iRow, i);
      Figure* figureFound = getFigureAtPosition(iRow, i);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      //if (iColor == getPieceColor(chPieceFound))
      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::ROOK))
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
      //char chPieceFound = getPieceAtPosition(iRow, i);
      Figure* figureFound = getFigureAtPosition(iRow, i);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::ROOK))
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
      //char chPieceFound = getPieceAtPosition(i, iColumn);
      Figure* figureFound = getFigureAtPosition(i, iColumn);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::PAWN) &&
        (foundFigureColor == static_cast<int>(FigureColor::BLACK_FIGURE)) &&
        (i == iRow + 1))
      {
        // There is a pawn one square up, so the square is reachable
        bReachable = true;
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::ROOK))
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
      //char chPieceFound = getPieceAtPosition(i, iColumn);
      Figure* figureFound = getFigureAtPosition(i, iColumn);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::PAWN) &&
        (foundFigureColor == static_cast<int>(FigureColor::WHITE_FIGURE)) &&
        (i == iRow - 1))
      {
        // There is a pawn one square down, so the square is reachable
        bReachable = true;
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::ROOK))
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
      //char chPieceFound = getPieceAtPosition(i, j);
      Figure* figureFound = getFigureAtPosition(i, j);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::BISHOP))
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
      //char chPieceFound = getPieceAtPosition(i, j);
      Figure* figureFound = getFigureAtPosition(i, j);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::BISHOP))
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
      //char chPieceFound = getPieceAtPosition(i, j);
      Figure* figureFound = getFigureAtPosition(i, j);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::BISHOP))
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
      //char chPieceFound = getPieceAtPosition(i, j);
      Figure* figureFound = getFigureAtPosition(i, j);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        break;
      }
      else if ((figureFound->getType() == TypeFigure::QUEEN) ||
        (figureFound->getType() == TypeFigure::BISHOP))
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

      //char chPieceFound = getPieceAtPosition(iRowToTest, iColumnToTest);
      Figure* figureFound = getFigureAtPosition(iRowToTest, iColumnToTest);
      if (Constants::EMPTY_SQUARE == figureFound)
      {
        // This square is empty, move on
        continue;
      }

      int foundFigureColor = (figureFound->isWhite()) ? static_cast<int>(FigureColor::WHITE_FIGURE) : static_cast<int>(FigureColor::BLACK_FIGURE);
      if (iColor == foundFigureColor)
      {
        // This is a piece of the same color
        continue;
      }
      else if (figureFound->getType() == TypeFigure::KNIGHT)
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
      if(figure && figure->getType() == TypeFigure::KING && figure->isWhite() == isWhiteKing)
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
  int colorKing = getCurrentTurn();
  return isKingInCheck(static_cast<int>(getCurrentTurn()), intended_move);
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

    if (Constants::EMPTY_SQUARE != getFigureAtPosition(iRowToTest, iColumnToTest))
    {
      // That square is not empty, so no need to test
      continue;
    }

    IntendedMove intended_move;
    intended_move.figure = getFigureAtPosition(king.iRow, king.iColumn);
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
      //char chAttacker = getPieceAtPosition(king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn);
      Figure* figureAttacker = getFigureAtPosition(king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn);

      TypeFigure typeFigure{ TypeFigure::PAWN };
      switch (typeFigure)
      {
      case TypeFigure::PAWN:
      case TypeFigure::KNIGHT:
      {
        // If it's a pawn, there's no space in between the attacker and the king
        // If it's a knight, it doesn't matter because the knight can 'jump'
        // So, this is checkmate
        bCheckmate = true;
      }
      break;

      case TypeFigure::BISHOP:
      {
        if (false == canBeBlocked(king_attacked.attacker[0].pos, king, static_cast<int>(Direction::DIAGONAL)))
        {
          // If no piece can get in the way, it's a checkmate
          bCheckmate = true;
        }
      }
      break;

      case TypeFigure::ROOK:
      {
        if (false == canBeBlocked(king_attacked.attacker[0].pos, king, static_cast<int>(king_attacked.attacker[0].dir)))
        {
          // If no piece can get in the way, it's a checkmate
          bCheckmate = true;
        }
      }
      break;

      case TypeFigure::QUEEN:
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

