#pragma once

#include "cocos2d.h"
#include "ui/UIImageView.h"

enum class TypeFigure
{
  PAWN = 1,
  ROOK = 2,
  KNIGHT = 3,
  HORSE = 4,
  QUEEN = 5,
  KING = 6
};

enum class Colour
{
  WHITE,
  BLACK
};

class Figure : public cocos2d::ui::ImageView
{
public:
  Figure();
  Figure(TypeFigure typeFigre, const std::string& fileName);
  ~Figure();

  static Figure* createFigure(TypeFigure typeFigure, const std::string& fileName);

  void setColour(Colour colour);
  void setTypeFigure(TypeFigure typeFigure);

  Colour getColour();
  TypeFigure getTypeFigure();

private:
  Colour m_colour{ Colour::WHITE };
  TypeFigure m_typeFigure{ TypeFigure::PAWN };

};

