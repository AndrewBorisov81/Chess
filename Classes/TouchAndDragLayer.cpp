#include "TouchAndDragLayer.h"
#include "Figure.h"
#include "Constants.h"
#include "Grid.h"

USING_NS_CC;

TouchAndDragLayer::TouchAndDragLayer(std::vector<std::vector<Figure*>>& figures, Grid* grid)
  : m_figures(figures),
    m_grid(grid)
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

Figure* TouchAndDragLayer::getFigure()
{
  return  m_currentDragFigure;
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
      Figure* pClickFigure = m_figures[cellI][cellJ];
      if (pClickFigure)
      {
        m_currentDragFigure = pClickFigure;
        m_currentFigureSize = m_currentDragFigure->getContentSize();

        m_figures[cellI][cellJ] = 0;

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

    if (m_currentDragFigure)
    {
      Size curFigureCellIJ;
      Vec2 curFigurePoint = m_currentDragFigure->getPosition();

      m_grid->calculateCellByPoint(curFigurePoint, Constants::CELL_SIZE, curFigureCellIJ);

      int cellI = curFigureCellIJ.height;
      int cellJ = curFigureCellIJ.width;

      m_currentDragFigure->setPosition(m_grid->getPointByCell(curFigureCellIJ.width, curFigureCellIJ.height));

      m_figures[cellI][cellJ] = m_currentDragFigure;

      m_currentDragFigure = nullptr;
      m_currentFigureSize = Size::ZERO;
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

    if (m_currentDragFigure)
    {
      if(x >= 0 && y >= 0 && y < Constants::ROWS * Constants::CELL_SIZE && x < Constants::ROWS * Constants::CELL_SIZE)
          m_currentDragFigure->setPosition(Vec2(x, y));
    }
  }
}