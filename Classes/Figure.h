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

class Figure : public cocos2d::ui::ImageView
{
public:
  Figure();
  Figure(int type, bool isWhite);
  ~Figure();

  static Figure* createFigure(int type, bool isWhite, std::string fileName);

  TypeFigure getType();
  bool isWhite();

  void setType(TypeFigure type);
  void setIsWhite(bool isWhite);

private:
  TypeFigure m_type{ TypeFigure::PAWN };
  bool m_isWhite{ true };

};