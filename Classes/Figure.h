#pragma once

#include "cocos2d.h"
#include "ui/UIImageView.h"

enum class TypeFigure
{
  ROOK = 1,
  HORSE = 2,
  KNIGHT = 3,
  QUEEN = 4,
  KING = 5,
  PAWN = 6
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
  Figure(TypeFigure type, ColourFigure colour, const std::string& fileName);
  //Figure(TypeFigure type, bool isWhite, const std::string& fileName);
  ~Figure();

  static Figure* createFigure(int type, bool isWhite, const std::string& fileName);
  //static Figure* createFigure(TypeFigure type, ColourFigure colour, const std::string& fileName);

  ColourFigure getColour();
  TypeFigure getType();

private:
  ColourFigure m_colour{ ColourFigure::WHITE };
  TypeFigure m_type{ TypeFigure::PAWN };

};

