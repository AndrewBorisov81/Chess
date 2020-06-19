#pragma once

#include "cocos2d.h"

class Grid : public cocos2d::Layer
{
public:
  Grid();
  Grid(float cellSize, int rows, int columns);
  ~Grid();

  virtual bool init();

  void drawGrid(float cellSize, int rows, int columns);
  void showGrid();
  void hideGrid();

  void drawLine(cocos2d::Vec2 begin, cocos2d::Vec2 end);

private:
  float m_cellSize{};
  int m_rows{};
  int m_columns{};
};