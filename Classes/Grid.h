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

private:
  float m_cellSize{};
  int m_rows{};
  int m_columns{};
};