#include "Grid.h"

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

  drawGrid(m_cellSize, m_rows, m_columns);

  return true;
}

void Grid::drawGrid(float cellSize, int rows, int columns)
{
  auto drawNode = DrawNode::create();

  for (int i = 0; i < 1; i++)
  {
    drawNode->drawLine(Vec2(0, i * cellSize), Vec2(columns * cellSize, i * cellSize), Color4F::GREEN);
  }

  /*for (int j = 0; j < columns; j++)
  {
    drawNode->drawLine(Vec2(j * columns * cellSize, 0), Vec2(j * columns * cellSize, j * rows), Color4F::GREEN);
  }*/

  this->addChild(drawNode, 1);
}

void Grid::drawLine(Vec2 begin, Vec2 end)
{
  /*auto drawNode = DrawNode::create();
  drawNode->drawLine(begin, end, Color4F::YELLOW);
  this->addChild(drawNode, 1);*/
}

void Grid::showGrid()
{
}

void Grid::hideGrid()
{
}
