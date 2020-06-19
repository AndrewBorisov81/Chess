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

  virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

private:
  float m_cellSize{};
  int m_rows{};
  int m_columns{};

  cocos2d::Label* m_labelTouchInfo;
};