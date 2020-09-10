#include "Grid.h"
#include "Piece.h"

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

  /*DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 40, 360, 20, true, 1, 1, Color4F::GREEN);
  this->addChild(drawnode, 1000);*/

  //Draw grid
  //drawGrid(m_cellSize, m_rows, m_columns);

  /*auto touchListener = EventListenerTouchOneByOne::create();

  touchListener->onTouchBegan = CC_CALLBACK_2(Grid::onTouchBegan, this);
  touchListener->onTouchEnded = CC_CALLBACK_2(Grid::onTouchEnded, this);
  touchListener->onTouchMoved = CC_CALLBACK_2(Grid::onTouchMoved, this);
  touchListener->onTouchCancelled = CC_CALLBACK_2(Grid::onTouchCancelled, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

  this->addChild(labelTouchInfo);*/

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

  this->addChild(drawNode, 1000);
}

void Grid::showGrid()
{
}

void Grid::hideGrid()
{
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

Piece* Grid::getCurrentPiece()
{
  return m_currentPiece;
}

void Grid::setCurrentPiece(Piece* piece)
{
  m_currentPiece = piece;
}

void Grid::resetCurrentPiece()
{
  m_currentPiece = nullptr;
}
