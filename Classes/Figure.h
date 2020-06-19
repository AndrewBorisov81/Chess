#pragma once

#include "cocos2d.h"

enum class TypeFigure
{
  PAWN = 1,
  ROOK = 2,
  KNIGHT = 3,
  BISHOP = 4,
  QUEEN = 5,
  KING = 6
};

class Figure : public cocos2d::Sprite
{
public:
  Figure();
  Figure(const std::string& fileName);
  ~Figure();

  static Figure* createFigure(const std::string& fileName);

private:

};
