#pragma once

#include <string>

class Figure;

namespace Constants
{
  // Images
  extern const std::string BOARD_PNG;
  extern const std::string PROMOTION_TABLE;

  // Button
  extern const std::string UNDO_BUTTON_NORMAL;
  extern const std::string UNDO_BUTTON_SELECTED;

  // Figures
  extern const std::string BLACK_PAWN_PNG;
  extern const std::string BLACK_ROOK_PNG;
  extern const std::string BLACK_KNIGHT_PNG;
  extern const std::string BLACK_BISHOP_PNG;
  extern const std::string BLACK_QUEEN_PNG;
  extern const std::string BLACK_KING_PNG;

  extern const std::string WHITE_PAWN_PNG;
  extern const std::string WHITE_ROOK_PNG;
  extern const std::string WHITE_KNIGHT_PNG;
  extern const std::string WHITE_BISHOP_PNG;
  extern const std::string WHITE_QUEEN_PNG;
  extern const std::string WHITE_KING_PNG;

  // Game elements setting
  extern const float CELL_SIZE;
  extern const int ROWS;
  extern const int COLUMNS;
  extern Figure* EMPTY_SQUARE;

  // This represents the pieces on the board.
  // Keep in mind that pieces[0][0] represents A1
  // pieces[1][1] represents B2 and so on.
  // Numbers with a minus sign are white
  //*extern const int INITIAL_FIGURES_BOARD[8][8];

  // Debug
  // Castling
  //extern const int INITIAL_FIGURES_BOARD[8][8];

  // Checkmate
  //extern const int INITIAL_FIGURES_BOARD[8][8];

  // Debug promotion
  extern const int INITIAL_FIGURES_BOARD[8][8];
}
