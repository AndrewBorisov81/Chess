#pragma once

#include "cocos2d.h"
#include "ui/UIImageView.h"

enum class PieceColor
{
  WHITE_PIECE = 0,
  BLACK_PIECE = 1
};

enum class TypePiece
{
  ROOK = 1,
  KNIGHT = 2,
  BISHOP = 3,
  QUEEN = 4,
  KING = 5,
  PAWN = 6
};

class Piece : public cocos2d::ui::ImageView
{
public:
  Piece();
  Piece(int type, bool isWhite);
  Piece(int type, bool isWhite, const std::string& fileName);
  ~Piece();

  static bool isWhite(int iPiece);

  static bool isBlack(int iPiece);

  static PieceColor getColor(int iPiece);

  static TypePiece getTypeP(int iPiece);

  static Piece* createPiece(int type, bool isWhite, std::string fileName);

  TypePiece getType();
  bool isWhite();

  void setType(TypePiece type);
  void setIsWhite(bool isWhite);

  void setCell(const cocos2d::Size& cellRowColumn);
  cocos2d::Size getCell();

  cocos2d::Rect getRect();

private:
  TypePiece m_type{ TypePiece::PAWN };

  bool m_isWhite{ true };

  std::string m_fileName;

  cocos2d::Size m_cell;
};

