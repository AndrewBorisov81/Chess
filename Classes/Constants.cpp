#include "Constants.h"

namespace Constants
{
  // Imgages
  const std::string BOARD_PNG = "chessBoard3.png";
  const std::string PROMOTION_TABLE = "promotionTable.png";

  // Buttons
  const std::string UNDO_BUTTON = "undoButtonNormal.png";
  const std::string UNDO_BUTTON_SELECTED = "undoButtonSelected.png";

  // Piece
  const std::string BLACK_PAWN_PNG = "blackPawn3.png";
  const std::string BLACK_ROOK_PNG = "blackRook3.png";
  const std::string BLACK_KNIGHT_PNG = "blackHorse3.png";
  const std::string BLACK_BISHOP_PNG = "blackOfficer3.png";
  const std::string BLACK_QUEEN_PNG = "blackQueen3.png";
  const std::string BLACK_KING_PNG = "blackKing3.png";

  const std::string WHITE_PAWN_PNG = "whitePawn3.png";
  const std::string WHITE_ROOK_PNG = "whiteRook3.png";
  const std::string WHITE_KNIGHT_PNG = "whiteHorse3.png";
  const std::string WHITE_BISHOP_PNG = "whiteOfficer3.png";
  const std::string WHITE_QUEEN_PNG = "whiteQueen3.png";
  const std::string WHITE_KING_PNG = "whiteKing3.png";

  // Prompts
  const std::string GREEN_RECT_PNG = "greenRect.png";
  const std::string RED_RECT_PNG = "redRect.png";
  const std::string GREEN_CIRCLE_PNG = "greenCircle.png";

  // Game elements setting
  const float CELL_SIZE(118.0f);
  const int ROWS(8);
  const int COLUMNS(8);
  int EMPTY_SQUAREI(0);

  // This represents the pieces on the board.
 // Keep in mind that pieces[0][0] represents A1
 // pieces[1][1] represents B2 and so on.
 // Numbers with a minus sign are white
 /*const int INITIAL_PIECE_BOARD[8][8] =
               { 1, 2, 3, 4, 5, 3, 2, 1,
                 6, 6, 6, 6, 6, 6, 6, 6,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                -6,-6,-6,-6,-6,-6,-6,-6,
                -1,-2,-3,-4,-5,-3,-2,-1
               };*/

               // Debug
               // Castling
               /*const int INITIAL_PIECE_BOARD[8][8] =
               { 1, 0, 0, 4, 5, 0, 0, 1,
                 6, 6, 6, 6, 6, 6, 6, 6,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                -6,-6,-6,-6,-6,-6,-6,-6,
                -1, 0, 0, 0,-5, 0, 0,-1
               };*/

               // Castling2
               /*const int INITIAL_PIECE_BOARD[8][8] =
               { 1, 0, 0, 0, 5, 0, 0, 1,
                 6, 0, 6, 6, 6, 6, 6, 6,
                -3, 6, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                -6,-6,-6,-6,-6,-6,-6,-6,
                -1, 0, 0, 0,-5, 0, 0,-1
               };*/
                
               // Castling3
               /*const int INITIAL_PIECE_BOARD[8][8] =
               { 1, 0, 0, 0, 5, 0, 0, 1,
                 6, 0, 6, 6, 6, 6, 0, 6,
                -3, 6, 0, 0, 0, 0, 6,-3,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                -6,-6,-6,-6,-6,-6,-6,-6,
                -1, 0, 0, 0,-5, 0, 0,-1
               };*/  

               // Castling4
               /*const int INITIAL_PIECE_BOARD[8][8] =
                                         { 1, 0, 0, 0, 5, 0, 0, 1,
                                           6, 0, 6, 6, 6, 6, 0, 6,
                                           3, 6, 0, 0, 0, 0, 6, 3,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0,-6, 0, 0, 0,
                                          -6,-6,-6,-6, 0,-6,-6,-6,
                                          -1, 0, 0, 0,-5, 0, 0,-1
                                         };*/
                            // Castling5
                            /*const int INITIAL_PIECE_BOARD[8][8] =
                            { 1, 0, 0, 0, 5, 0, 0, 1,
                              6, 0, 6, 6, 6, 6, 0, 6,
                              0, 6, 0, 0, 0, 0, 6, 3,
                              0, 0, 3, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0,
                             -6,-6,-6,-6,-6, 0,-6,-6,
                             -1, 0, 0, 0,-5, 0, 0,-1
                            };*/

               // Checkmate
               /*const int INITIAL_PIECE_BOARD[8][8] =
               { 1, 2, 3, 4, 5, 3, 2, 1,
                 6, 6, 6, 6, 0, 6, 6, 6,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 6, 0, 0, 0,
                 0, 0, 0, 0,-6, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                -6,-6,-6,-6,-5,-6,-6,-6,
                -1,-2,-3,-4, 0,-3,-2,-1
               };*/

               // Debug promotion
  /*const int INITIAL_PIECE_BOARD[8][8] =
  { 0, 0, 0, 0, 5, 3, 2, 1,
    0, 0, 0, 6, 6, 6, 6, 6,
   -6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,-6,-6,-6,-6,-6,-6,-6,
   -1,-2,-3,-4,-5,-3,-2,-1
  };*/

  // Debug king attack
  /*const int INITIAL_PIECE_BOARD[8][8] =
  { 0, 0, 0, 0, 5, 3, 2, 1,
    0, 0, 0, 6, 6, 6, 6, 6,
   -6, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,-3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,-6,-6,-6,-6,-6,-6,-6,
   -1,-2,-3,-4,-5, 0,-2,-1
  };*/

  // Debug king under attack
const int INITIAL_PIECE_BOARD[8][8] =
{ 0, 0, 0, 0, 5, 3, 2, 1,
  0, 0, 0, 6, 6, 6, 6, 6,
 -6, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
 -3, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0,-6,-6,-6,-6,-6,-6,-6,
 -1,-2,-3,-4,-5,-3,-2,-1
};
}