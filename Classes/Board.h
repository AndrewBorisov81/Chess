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

  virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);

  void loadAllFigures(std::vector<std::vector<Figure*>>& figures, int zOrder);
  void addFigure();
  void removeFigure();
  void addGrid(Grid* grid);

  cocos2d::Size m_boardSize;

private:
  Grid* m_grid{ nullptr };
  std::vector<std::vector<Figure*>> m_figures;

  cocos2d::Label* m_labelTouchInfo;
};
