#include "Grid.h"
#include "Figure.h"

#include "base/CCEventListener.h"
#include "base/CCEvent.h"
#include "math.h"

#include <string>

USING_NS_CC;

Grid::Grid()
{
}

Grid::Grid(float cellSize, int rows, int columns)
  :m_cellSize(cellSize), m_rows(rows), m_columns(columns)
{

}

Grid::~Grid()
{

}

bool Grid::init()
{
  if (!CCLayer::init()) {

    return false;
  }

  DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 40, 360, 20, true, 1, 1, Color4F::GREEN);
  this->addChild(drawnode);

  //drawGrid(m_cellSize, m_rows, m_columns);

  Label* labelTouchInfo = Label::createWithSystemFont("Touch or clicksomewhere to begin", "Arial", 30);
  m_labelTouchInfo = labelTouchInfo;

  labelTouchInfo->setPosition(Vec2(
    Director::getInstance()->getVisibleSize().width / 2,
    Director::getInstance()->getVisibleSize().height / 2));

  auto touchListener = EventListenerTouchOneByOne::create();

  touchListener->onTouchBegan = CC_CALLBACK_2(Grid::onTouchBegan, this);
  touchListener->onTouchEnded = CC_CALLBACK_2(Grid::onTouchEnded, this);
  touchListener->onTouchMoved = CC_CALLBACK_2(Grid::onTouchMoved, this);
  touchListener->onTouchCancelled = CC_CALLBACK_2(Grid::onTouchCancelled, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

  this->addChild(labelTouchInfo);

  this->scheduleUpdate();

  return true;
}

void Grid::drawGrid(float cellSize, int rows, int columns)
{
  auto drawNode = DrawNode::create();

  for (int i = 0; i < rows + 1; i++)
  {
    drawNode->drawLine(Vec2(0, i * cellSize), Vec2(columns * cellSize, i * cellSize), Color4F::GREEN);
  }

  for (int j = 0; j < columns + 1; j++)
  {
    drawNode->drawLine(Vec2(j * cellSize, 0), Vec2(j * cellSize, rows * cellSize), Color4F::GREEN);
  }

  this->addChild(drawNode, 1);
}

void Grid::showGrid()
{
}

void Grid::hideGrid()
{
}

bool Grid::onTouchBegan(Touch* touch, Event* event)
{
  if (m_currentFigure) {
    m_prevPosFigure = m_currentFigure->getPosition();
  }
  float x = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).x;
  float y = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).y;
  m_labelTouchInfo->setPosition(Vec2(x,y));

  Size cellIJ{};
  float cellSize{ m_cellSize };

  calculateCellByPoint(Vec2(x, y), cellSize, cellIJ);

  //m_labelTouchInfo->setString("You Touched Here" + std::to_string(int(cellIJ.width)) + std::to_string(int(cellIJ.height)));
  return true;
}

void Grid::onTouchEnded(Touch* touch, Event* event)
{
  m_delta.x = 0;
  m_delta.y = 0;
  m_location.x = 0;
  m_location.y = 0;
  cocos2d::log("touch ended");
  resetCurrentFigure();
}

void Grid::onTouchMoved(Touch* touch, Event* event)
{
  if (m_currentFigure) {
    m_currentFigure->setPosition(m_currentFigure->getPosition() + touch->getDelta());
  }


  /*m_delta.x = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).x;
  m_delta.y = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).y;
  cocos2d::log("touch moved");*/
  //m_labelTouchInfo->setString(std::to_string(int(m_location.x))+ " , " + std::to_string(int(m_location.y)));
  /*m_labelTouchInfo->setString(std::to_string(int(m_delta.x))+ " , " + std::to_string(int(m_delta.y)));
  if (m_currentFigure) {
    m_curPosFigure.x = m_prevPosFigure.x + m_delta.x;
    m_curPosFigure.y = m_prevPosFigure.y + m_delta.y;
    m_currentFigure->setPosition(m_curPosFigure);
  }*/
}

void Grid::onTouchCancelled(Touch* touch, Event* event)
{
  cocos2d::log("touch cancelled");
}

void Grid::calculateCellByPoint(const cocos2d::Vec2& point, float cellSize, cocos2d::Size& cellIJ)
{
  int i{}, j{};

  i = floor(point.x / cellSize);
  j = floor(point.y / cellSize);

  cellIJ.width = i;
  cellIJ.height = j;
}

cocos2d::Vec2 Grid::getPointByCell(int i, int j)
{
  float x = i * m_cellSize + 0.5 * m_cellSize;
  float y = j * m_cellSize + 0.5 * m_cellSize;

  return cocos2d::Vec2(x, y);
}

cocos2d::Vec2 Grid::getDelta()
{
  return m_delta;
}

cocos2d::Vec2 Grid::getLocation()
{
  return m_location;
}

Figure* Grid::getCurrentFigure()
{
  return m_currentFigure;
}

void Grid::setCurrentFigure(Figure* figure)
{
  m_currentFigure = figure;
}

void Grid::resetCurrentFigure()
{
  m_currentFigure = nullptr;
}
