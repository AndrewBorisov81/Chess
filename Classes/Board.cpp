#include "Board.h"

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



  return true;
}