#pragma once

#include "cocos2d.h"

typedef enum tagTouchAndDragState
{
  kGrabbed,
  kUngrabbed
} TouchAndDragState;

class Figure;
class Grid;

class TouchAndDragLayer : public cocos2d::Layer
{
  TouchAndDragState _state{ kUngrabbed };

public:
  TouchAndDragLayer(std::vector<std::vector<Figure*>>& figures, Grid* grid);
  virtual ~TouchAndDragLayer();

  virtual bool init();

  cocos2d::EventListenerMouse* m_mouseListener;

 void onMouseDown(cocos2d::Event* event);
 void onMouseUp(cocos2d::Event* event);
 void onMouseMove(cocos2d::Event* event);

private:
  std::vector<std::vector<Figure*>> m_figures;
  Grid* m_grid;

  Figure* m_currentDragFigure{ nullptr };

  //cocos2d::Label* labelTouchInfo;
};