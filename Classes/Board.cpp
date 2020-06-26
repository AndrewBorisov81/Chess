#include "Board.h"
#include "Constants.h"

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

void Board::loadAllFigures()
{
}

void Board::addFigure()
{
}

void Board::removeFigure()
{
}
