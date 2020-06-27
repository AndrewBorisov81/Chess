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

enum class ColourFigure
{
  WHITE,
  BLACK
};

class Figure : public cocos2d::ui::ImageView
{
public:
  Figure();
  Figure(TypeFigure typeFigre, ColourFigure, const std::string& fileName);
  ~Figure();

  static Figure* createFigure(TypeFigure type, ColourFigure colour, const std::string& fileName);

  ColourFigure getColour();
  TypeFigure getType();

private:
  ColourFigure m_colour{ ColourFigure::WHITE };
  TypeFigure m_type{ TypeFigure::PAWN };

};

