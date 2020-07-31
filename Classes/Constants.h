#pragma once

#include <string>

namespace Constants
{
  // Images
  extern const std::string BOARD_PNG;
  // Figures
  extern constexpr auto BLACK_PAWN_PNG("blackPawn3.png");
  extern constexpr auto BLACK_ROOK_PNG("blackRook3.png");
  extern constexpr auto BLACK_KNIGHT_PNG("blackHorse3.png");
  extern constexpr auto BLACK_BISHOP_PNG("blackOfficer3.png");
  extern constexpr auto BLACK_QUEEN_PNG("blackQueen3.png");
  extern constexpr auto BLACK_KING_PNG("blackKing3.png");

  extern constexpr auto WHITE_PAWN_PNG("whitePawn3.png");
  extern constexpr auto WHITE_ROOK_PNG("whiteRook3.png");
  extern constexpr auto WHITE_KNIGHT_PNG("whiteHorse3.png");
  extern constexpr auto WHITE_BISHOP_PNG("whiteOfficer3.png");
  extern constexpr auto WHITE_QUEEN_PNG("whiteQueen3.png");
  extern constexpr auto WHITE_KING_PNG("whiteKing3.png");

  // Game elements setting
  extern constexpr auto CELL_SIZE(118.0f);
  extern constexpr auto ROWS(8);
  extern constexpr auto COLUMNS(8);
  extern constexpr auto EMPTY_SQUARE(nullptr);

  // This represents the pieces on the board.
  // Keep in mind that pieces[0][0] represents A1
  // pieces[1][1] represents B2 and so on.
  // Numbers with a minus sign are white
  constexpr int INITIAL_FIGURES_BOARD[8][8] =
                { 1, 2, 3, 4, 5, 3, 2, 1,
                  6, 6, 6, 6, 6, 6, 6, 6,
                  0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 
                 -6,-6,-6,-6,-6,-6,-6,-6,
                 -1,-2,-3,-4,-5,-3,-2,-1
                };

  // Debug
  // Castling
  /*constexpr int INITIAL_FIGURES_BOARD[8][8] =
  { 1, 0, 0, 4, 5, 0, 0, 1,
    6, 6, 6, 6, 6, 6, 6, 6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
   -6,-6,-6,-6,-6,-6,-6,-6,
   -1, 0, 0, 0,-5, 0, 0,-1
  };*/

  // Checkmate
  /*constexpr int INITIAL_FIGURES_BOARD[8][8] =
  { 1, 2, 3, 4, 5, 3, 2, 1,
    6, 6, 6, 6, 0, 6, 6, 6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 6, 0, 0, 0,
    0, 0, 0, 0,-6, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
   -6,-6,-6,-6,-5,-6,-6,-6,
   -1,-2,-3,-4, 0,-3,-2,-1
  };*/ 
}
