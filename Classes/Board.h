#pragma once

#include "cocos2d.h"

class Figure;
class Grid;

class Board : public cocos2d::Layer
{
public:
  Board();
  virtual ~Board();

  virtual bool init();

  void loadAllFigures(std::vector<std::vector<Figure*>>& figures);
  void addFigure();
  void removeFigure();
  void addGrid(Grid* grid);

  cocos2d::Size m_boardSize;

private:
  Grid* m_grid{ nullptr };
  std::vector<std::vector<Figure*>> m_figures;
};
