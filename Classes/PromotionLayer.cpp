#include "PromotionLayer.h"
#include "Constants.h"

USING_NS_CC;

PromotionLayer::PromotionLayer()
{
}

PromotionLayer::~PromotionLayer()
{
}


bool PromotionLayer::init()
{
  if (!CCLayer::init()) {

    return false;
  }

  // Table image
  auto tableSprite = Sprite::createWithSpriteFrameName(Constants::PROMOTION_TABLE);
  Size tableSize = tableSprite->getContentSize();
  m_tableSize = tableSize;
  this->addChild(tableSprite, 1);

  createFigures(1, true);

  return true;
}

cocos2d::Size PromotionLayer::getTableSize()
{
  return m_tableSize;
}

void PromotionLayer::show()
{
}

void PromotionLayer::hide()
{
}

void PromotionLayer::figureCallback(cocos2d::Ref * pSender)
{
  bool stop = true;
}

void PromotionLayer::createFigures(int typeFigure, bool isWhite)
{
  std::string pngName = (isWhite)? Constants::WHITE_ROOK_PNG : Constants::BLACK_ROOK_PNG;

  // 2 - KNIGHT; 3 - BISHOP; 4 - QWEEN 
  switch (typeFigure) {
    case 2:
      pngName = (isWhite) ? Constants::WHITE_KNIGHT_PNG : Constants::BLACK_KNIGHT_PNG;
      break;

    case 3:
      pngName = (isWhite) ? Constants::WHITE_BISHOP_PNG : Constants::BLACK_KING_PNG;
      break;

    case 4:
      pngName = (isWhite) ? Constants::WHITE_QUEEN_PNG : Constants::BLACK_QUEEN_PNG;
      break;
  }

  auto figure = MenuItemImage::create(
    pngName,
    pngName,
    CC_CALLBACK_1(PromotionLayer::figureCallback, this));

  figure->setScale(1.5);

  /*closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
    origin.y + closeItem->getContentSize().height / 2));*/

    // create menu, it's an autorelease object
  auto menu = Menu::create(figure, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 100);
}
