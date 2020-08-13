#pragma once

#include "cocos2d.h"
#include "ui/UIImageView.h"

enum class TypeFigure
{
  ROOK = 1,
  KNIGHT = 2,
  BISHOP = 3,
  QUEEN = 4,
  KING = 5,
  PAWN = 6
};

class Figure : public cocos2d::ui::ImageView
{
public:
  Figure();
  Figure(int type, bool isWhite);
  Figure(int type, bool isWhite, const std::string& fileName);
  ~Figure();

  // Copy constructor
  Figure(const Figure& figure);

  static Figure* createFigure(int type, bool isWhite, std::string fileName);

  TypeFigure getType();
  bool isWhite();

  void setType(TypeFigure type);
  void setIsWhite(bool isWhite);

  cocos2d::Rect getRect();

  Figure* cloneFigure();

private:
  TypeFigure m_type{ TypeFigure::PAWN };
  bool m_isWhite{ true };

  std::string m_fileName;
};