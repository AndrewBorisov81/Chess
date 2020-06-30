#include "Board.h"
#include "Constants.h"
#include "Grid.h"
#include "Figure.h"

USING_NS_CC;

Board::Board()
{
}

Board::~Board()
{

}

bool Board::init()
{
  if (!CCLayer::init()) {

    return false;
  }

  // Load the Sprite Sheet
  auto spritecache = SpriteFrameCache::getInstance();
  spritecache->addSpriteFramesWithFile("texture.plist");

  // Block
  auto boardSprite = Sprite::createWithSpriteFrameName(Constants::BOARD_PNG);
  Size boardSize = boardSprite->getContentSize();
  m_boardSize = boardSize;
  this->addChild(boardSprite, 1);

  DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 50, 360, 20, true, 1, 1, Color4F::RED);
  boardSprite->addChild(drawnode);

  return true;
}

void Board::loadAllFigures(std::vector<std::vector<Figure*>>& figures)
{
  for (int i = 0; i < figures.size(); i++)
  {
    std::vector<Figure*> row = figures[i];
    for (int j = 0; j < row.size(); j++)
    {
      if (row[j] != 0)
      {
        Figure* pFigure = row[j];
        m_grid->addChild(pFigure, 303);
        pFigure->setPosition(Vec2(m_grid->getPointByCell(j, i)));
      }
    }
  }
}

void Board::addFigure()
{
}

void Board::removeFigure()
{
}

void Board::addGrid(Grid* grid)
{
  m_grid = grid;
}
