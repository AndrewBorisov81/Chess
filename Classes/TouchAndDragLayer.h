#pragma once

#include "cocos2d.h"

typedef enum tagTouchAndDragState
{
  kGrabbed,
  kUngrabbed
} TouchAndDragState;

class GameLayer;
class Figure;
class Grid;

class TouchAndDragLayer : public cocos2d::Layer
{
  TouchAndDragState _state{ kUngrabbed };

public:
  TouchAndDragLayer(std::vector<std::vector<Figure*>>& figures, GameLayer* gameLayer, Grid* grid);
  virtual ~TouchAndDragLayer();

  virtual bool init();

  Figure* getFigure();

  cocos2d::EventListenerMouse* m_mouseListener;

 void onMouseDown(cocos2d::Event* event);
 void onMouseUp(cocos2d::Event* event);
 void onMouseMove(cocos2d::Event* event);

private:
  GameLayer* m_gameLayer{ nullptr };
  Grid* m_grid;

  Figure* m_currentDragFigure{ nullptr };
  cocos2d::Size m_currentFigureSize;
  std::vector<std::vector<Figure*>> m_figures;

  cocos2d::Size m_prevCellIJFigure;
  cocos2d::Size m_curCellIJFigure;

  //cocos2d::Label* labelTouchInfo;
};