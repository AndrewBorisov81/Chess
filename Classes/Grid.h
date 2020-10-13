#pragma once

#include "cocos2d.h"

class Piece;

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
  Piece* getCurrentPiece();
  void setCurrentPiece(Piece* piece);
  void resetCurrentPiece();

private:
  float m_cellSize{};
  int m_rows{};
  int m_columns{};

  cocos2d::Size m_currentClickCell{};
  Piece* m_currentPiece{ nullptr };

  cocos2d::Vec2 m_delta{};
  cocos2d::Vec2 m_location{};

  cocos2d::Vec2 m_prevPosPiece{};
  cocos2d::Vec2 m_curPosPiece{};
};
