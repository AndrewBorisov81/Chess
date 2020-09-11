#pragma once

#include "cocos2d.h"

#include "Grid.h"

#include <functional>

typedef enum tagTouchAndDragState
{
  kGrabbed,
  kUngrabbed
} TouchAndDragState;

class GameLayer;
class Piece;
struct DataChess;

class TouchAndDragLayer : public Grid
{
  TouchAndDragState _state{ kUngrabbed };

public:
  TouchAndDragLayer(float cellSize, int rows, int columns);

  virtual ~TouchAndDragLayer();

  virtual bool init();

  Piece* getPiece();

  cocos2d::EventListenerMouse* m_mouseListener;

 void onMouseDown(cocos2d::Event* event);
 void onMouseUp(cocos2d::Event* event);
 void onMouseMove(cocos2d::Event* event);

 void callBackHaveMovedPiece(const std::function<void(Piece* piece, cocos2d::Size& oldPos, cocos2d::Size& newPos)>& callBack);
 void callBackGetPieceFromCell(const std::function<Piece*(cocos2d::Size& clickCell)>& callBack);

private:
  Piece* m_currentDragPiece{ nullptr };

  cocos2d::Size m_currentPiecesize;

  cocos2d::Size m_prevCellIJPiece;
  cocos2d::Size m_curCellIJPiece;

  std::function<void(Piece* piece, cocos2d::Size& oldPos, cocos2d::Size& newPos)> m_haveMovedPieceCallBack{ nullptr };
  std::function<Piece*(cocos2d::Size& clickCell)> m_getPieceFromCellCallBack{ nullptr };

  //cocos2d::Label* labelTouchInfo;
};