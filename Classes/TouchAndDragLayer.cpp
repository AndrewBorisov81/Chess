#include "TouchAndDragLayer.h"
#include "Figure.h"
#include "Constants.h"
#include "Grid.h"

USING_NS_CC;

TouchAndDragLayer::TouchAndDragLayer(int figures_board[8][8], std::vector<std::vector<Figure*>>& figures, Grid* grid)
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

  labelTouchInfo = Label::createWithSystemFont("Touch or clicksomewhere to begin", "Arial", 30);

  labelTouchInfo->setPosition(Vec2(
    Director::getInstance()->getVisibleSize().width / 2,
    Director::getInstance()->getVisibleSize().height / 2));

  this->addChild(labelTouchInfo);

  m_mouseListener = EventListenerMouse::create();
  m_mouseListener->onMouseMove = CC_CALLBACK_1(TouchAndDragLayer::onMouseMove, this);
  m_mouseListener->onMouseUp = CC_CALLBACK_1(TouchAndDragLayer::onMouseUp, this);
  m_mouseListener->onMouseDown = CC_CALLBACK_1(TouchAndDragLayer::onMouseDown, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(m_mouseListener, this);

  DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 35, 360, 20, true, 1, 1, Color4F::MAGENTA);
  this->addChild(drawnode);

  return true;
}


void TouchAndDragLayer::onMouseDown(Event* event)
{

  EventMouse* e = (EventMouse*)event;

  int x = event->getCurrentTarget()->convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY())).x;
  int y = event->getCurrentTarget()->convertToNodeSpace(Vec2(e->getCursorX(), e->getCursorY())).y;

  if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
  {
    Size resultClickCellSize;
    Vec2 clickPoint(x, y);

    m_grid->calculateCellByPoint(clickPoint, Constants::CELL_SIZE, resultClickCellSize);

    int cellI = resultClickCellSize.height;
    int cellJ = resultClickCellSize.width;

    Figure* clickFigure = m_figures[cellI][cellJ];
    m_currentDragFigure = clickFigure;

    /*m_figures[cellI][cellJ] = 0;
    m_currentTypeFigure = */

    _state = kGrabbed;
  }
}

void TouchAndDragLayer::onMouseUp(Event* event)
{
  // to illustrate the event....
  EventMouse* e = (EventMouse*)event;
  if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
  {
    _state = kUngrabbed;

    Size curFigureCellIJ;
    Vec2 curFigurePoint = m_currentDragFigure->getPosition();

    m_grid->calculateCellByPoint(curFigurePoint, Constants::CELL_SIZE, curFigureCellIJ);

    m_currentDragFigure->setPosition(m_grid->getPointByCell(curFigureCellIJ.width, curFigureCellIJ.height));

    m_currentDragFigure = nullptr;
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
      m_currentDragFigure->setPosition(Vec2(x, y));
    }
  }
}


/*bool TouchAndDragLayer::containsTouchLocation(cocos2d::Touch* touch)
{
  //return getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
  return true;
}*/

/*cocos2d::Rect TouchAndDragLayer::getRect()
{
  Sprite sp;
  auto s = (&sp)->getTexture()->getContentSize();
  return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}*/
