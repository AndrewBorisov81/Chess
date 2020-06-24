#pragma once

#include "cocos2d.h"
#include "ui/UIImageView.h"

enum class TypeFigure
{
  PAWN = 1,
  ROOK = 2,
  KNIGHT = 3,
  BISHOP = 4,
  QUEEN = 5,
  KING = 6
};

class Figure : public cocos2d::ui::ImageView
{
public:
  Figure();
  Figure(const std::string& fileName);
  ~Figure();

  void InitFigure();

  static Figure* createFigure(const std::string& fileName);

private:
  cocos2d::ui::ImageView* m_imageView{ nullptr };
  std::string m_fileName{};

};

