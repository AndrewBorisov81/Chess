#pragma once

#include "cocos2d.h"

class Figure;

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

  void calculateCellByPoint(const cocos2d::Vec2& point, float cellSize, cocos2d::Size& cellIJ);

  cocos2d::Vec2 getPointByCell(int i, int j);

  cocos2d::Vec2 getDelta();
  cocos2d::Vec2 getLocation();
  Figure* getCurrentFigure();
  void setCurrentFigure(Figure* figure);
  void resetCurrentFigure();

private:
  float m_cellSize{};
  int m_rows{};
  int m_columns{};

  cocos2d::Size m_currentClickCell{};
  Figure* m_currentFigure{ nullptr };

  cocos2d::Vec2 m_delta{};
  cocos2d::Vec2 m_location{};

  cocos2d::Vec2 m_prevPosFigure{};
  cocos2d::Vec2 m_curPosFigure{};
};