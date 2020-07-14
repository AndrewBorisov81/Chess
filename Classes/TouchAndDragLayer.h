#pragma once

#include "cocos2d.h"

#include <functional>

typedef enum tagTouchAndDragState
{
  kGrabbed,
  kUngrabbed
} TouchAndDragState;

class GameLayer;
class Figure;
class Grid;
struct DataChess;

class TouchAndDragLayer : public cocos2d::Layer
{
  TouchAndDragState _state{ kUngrabbed };

public:
  TouchAndDragLayer(GameLayer* gameLayer, Grid* grid);
  virtual ~TouchAndDragLayer();

  virtual bool init();

  Figure* getFigure();

  cocos2d::EventListenerMouse* m_mouseListener;

 void onMouseDown(cocos2d::Event* event);
 void onMouseUp(cocos2d::Event* event);
 void onMouseMove(cocos2d::Event* event);

 void callBackUpdateBoardFigures(const std::function<void(Figure* figure, cocos2d::Size& oldPos, cocos2d::Size& newPos)>& callBack);

private:
  GameLayer* m_gameLayer{ nullptr };
  Grid* m_grid;

  Figure* m_currentDragFigure{ nullptr };
  cocos2d::Size m_currentFigureSize;
  //std::vector<std::vector<Figure*>> m_figures;

  cocos2d::Size m_prevCellIJFigure;
  cocos2d::Size m_curCellIJFigure;

  std::function<void(Figure* figure, cocos2d::Size& oldPos, cocos2d::Size& newPos)> m_updateBoardFigures{ nullptr };

  //cocos2d::Label* labelTouchInfo;
};