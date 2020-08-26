#pragma once

#include "cocos2d.h"

#include <functional>

typedef enum tagTouchAndDragState
{
  kGrabbed,
  kUngrabbed
} TouchAndDragState;

class GameLayer;
class Piece;
class Grid;
struct DataChess;

class TouchAndDragLayer : public cocos2d::Layer
{
  TouchAndDragState _state{ kUngrabbed };

public:
  TouchAndDragLayer(GameLayer* gameLayer, Grid* grid);
  virtual ~TouchAndDragLayer();

  virtual bool init();

  Piece* getPiece();

  cocos2d::EventListenerMouse* m_mouseListener;

 void onMouseDown(cocos2d::Event* event);
 void onMouseUp(cocos2d::Event* event);
 void onMouseMove(cocos2d::Event* event);

 void callBackUpdateBoardPiece(const std::function<void(Piece* piece, cocos2d::Size& oldPos, cocos2d::Size& newPos)>& callBack);

private:
  GameLayer* m_gameLayer{ nullptr };
  Grid* m_grid;

  Piece* m_currentDragPiece{ nullptr };
  cocos2d::Size m_currentPiecesize;
  //std::vector<std::vector<Piece*>> m_pieces;

  cocos2d::Size m_prevCellIJPiece;
  cocos2d::Size m_curCellIJPiece;

  std::function<void(Piece* piece, cocos2d::Size& oldPos, cocos2d::Size& newPos)> m_updateBoardPiece{ nullptr };

  //cocos2d::Label* labelTouchInfo;
};