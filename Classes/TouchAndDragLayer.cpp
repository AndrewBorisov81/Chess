#include "TouchAndDragLayer.h"
#include "GameLayer.h"
#include "Piece.h"
#include "Constants.h"
#include "Grid.h"

#include <vector>

USING_NS_CC;

TouchAndDragLayer::TouchAndDragLayer(Grid* grid)
  : m_grid(grid)
{

}

TouchAndDragLayer::~TouchAndDragLayer()
{

}

bool TouchAndDragLayer::init()
{
  if (!CCLayer::init()) {

    return false;
  }

  /*labelTouchInfo = Label::createWithSystemFont("Touch or clicksomewhere to begin", "Arial", 30);

  labelTouchInfo->setPosition(Vec2(
    Director::getInstance()->getVisibleSize().width / 2,
    Director::getInstance()->getVisibleSize().height / 2));

  this->addChild(labelTouchInfo);*/

  m_mouseListener = EventListenerMouse::create();
  m_mouseListener->onMouseMove = CC_CALLBACK_1(TouchAndDragLayer::onMouseMove, this);
  m_mouseListener->onMouseUp = CC_CALLBACK_1(TouchAndDragLayer::onMouseUp, this);
  m_mouseListener->onMouseDown = CC_CALLBACK_1(TouchAndDragLayer::onMouseDown, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(m_mouseListener, this);

  /*DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 35, 360, 20, true, 1, 1, Color4F::MAGENTA);
  this->addChild(drawnode);*/

  return true;
}

Piece* TouchAndDragLayer::getPiece()
{
  return  m_currentDragPiece;
}


void TouchAndDragLayer::onMouseDown(Event* event)
{

  EventMouse* e = (EventMouse*)event;

  int x = event->getCurrentTarget()->convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY())).x;
  int y = event->getCurrentTarget()->convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY())).y;

  if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
  {
    Size calculateClickCell;
    Vec2 clickPoint(x, y);

    m_grid->calculateCellByPoint(clickPoint, Constants::CELL_SIZE, calculateClickCell);

    int cellI = calculateClickCell.height;
    int cellJ = calculateClickCell.width;

    if (cellI >= 0 && cellJ >= 0 && cellI < Constants::COLUMNS && cellJ < Constants::ROWS)
    {
      Piece* pClickPiece{ nullptr };

      if(m_getPieceFromCell)
        pClickPiece = m_getPieceFromCell(Size(cellI, cellJ));

      if (pClickPiece)
      {
        m_currentDragPiece = pClickPiece;
        m_currentPiecesize = m_currentDragPiece->getContentSize();

        m_prevCellIJPiece.width = cellI;
        m_prevCellIJPiece.height = cellJ;

        _state = kGrabbed;
      }
    }
  }
}

void TouchAndDragLayer::onMouseUp(Event* event)
{
  // to illustrate the event....
  EventMouse* e = (EventMouse*)event;
  if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
  {
    _state = kUngrabbed;

    if (m_currentDragPiece)
    {
      Size curPieceCellIJ;
      Vec2 curPiecePoint = m_currentDragPiece->getPosition();

      m_grid->calculateCellByPoint(curPiecePoint, Constants::CELL_SIZE, curPieceCellIJ);

      int cellI = curPieceCellIJ.height;
      int cellJ = curPieceCellIJ.width;

      m_currentDragPiece->setPosition(m_grid->getPointByCell(curPieceCellIJ.width, curPieceCellIJ.height));

      m_curCellIJPiece.width = cellI;
      m_curCellIJPiece.height = cellJ;

      Size prev(m_prevCellIJPiece.width, m_prevCellIJPiece.height);
      Size cur(m_curCellIJPiece.width, m_curCellIJPiece.height);

      Piece* pClickPiece = m_getPieceFromCell(Size(m_prevCellIJPiece.width, m_prevCellIJPiece.height));
      //pClickPiece->setCell(Size(m_curCellIJPiece.width, m_curCellIJPiece.height));

      m_updateBoardPiece(m_currentDragPiece, prev, cur);

      m_currentDragPiece = nullptr;
      m_currentPiecesize = Size::ZERO;
    }
  }
}

void TouchAndDragLayer::onMouseMove(Event* event)
{
  if (_state == kGrabbed)
  {
    EventMouse* e = (EventMouse*)event;

    int x = event->getCurrentTarget()->convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY())).x;
    int y = event->getCurrentTarget()->convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY())).y;

    if (m_currentDragPiece)
    {
      if(x >= 0 && y >= 0 && y < Constants::ROWS * Constants::CELL_SIZE && x < Constants::ROWS * Constants::CELL_SIZE)
        m_currentDragPiece->setPosition(Vec2(x, y));
    }
  }
}

void TouchAndDragLayer::callBackUpdateBoardPiece(const std::function<void(Piece* piece, Size& oldPos, Size& newPos)>& callBack)
{
  m_updateBoardPiece = callBack;
}

void TouchAndDragLayer::callBackGetPieceFromCell(const std::function<Piece*(cocos2d::Size& clickCell)>& callBack)
{
  m_getPieceFromCell = callBack;
}
