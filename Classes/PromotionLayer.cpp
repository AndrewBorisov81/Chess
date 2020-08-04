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

  m_imageFigureScale = 1.5;

  // Load the Sprite Sheet
  auto spritecache = SpriteFrameCache::getInstance();
  spritecache->addSpriteFramesWithFile("texture.plist");

  // Table image
  auto tableSprite = Sprite::createWithSpriteFrameName(Constants::PROMOTION_TABLE);
  m_table = tableSprite;
  Size tableSize = tableSprite->getContentSize();
  m_tableSize = tableSize;
  this->addChild(tableSprite, 1);

  // Create white figures
  createFigures(true, tableSize);

  // Create black figures
  createFigures(false, tableSize);

  hideFigures();

  show(false);

  return true;
}

cocos2d::Size PromotionLayer::getTableSize()
{
  return m_tableSize;
}

void PromotionLayer::show(bool isWhite)
{
  std::vector<Menu*> itemFigures;

  m_isWhite = isWhite;

  if (m_isWhite)
    itemFigures = m_whiteFigures;
  else
    itemFigures = m_blackFigures;

  for (int i = 0; i < itemFigures.size(); i++)
  {
    Menu* figures = itemFigures[i];
    figures->setOpacity(255);
  }

  m_table->setOpacity(255);
}

void PromotionLayer::hide()
{
  hideFigures();
  
  m_table->setOpacity(0);
}

void PromotionLayer::showFigures(bool isWhite)
{
  std::vector<Menu*> itemFigures;

  if (m_isWhite)
    itemFigures = m_whiteFigures;
  else
    itemFigures = m_blackFigures;

  for (int i = 0; i < itemFigures.size(); i++)
  {
    Menu* figures = itemFigures[i];
    figures->setOpacityModifyRGB(255);
  }
}

void PromotionLayer::hideFigures()
{
  for (int i = 0; i < m_whiteFigures.size(); i++)
  {
    Menu* figures = m_whiteFigures[i];
    figures->setOpacity(0);
  }

  for (int i = 0; i < m_blackFigures.size(); i++)
  {
    Menu* figures = m_blackFigures[i];
    figures->setOpacity(0);
  }
}

void PromotionLayer::figureCallback(cocos2d::Ref * pSender)
{
  
}

void PromotionLayer::createFigures(bool isWhite, Size tableSize)
{
  std::vector<Menu*> itemFigures;
  std::vector<Rect> rectFigures;

  float sumSizeWidthFigures{ 0 };

  if (isWhite && m_whiteFigures.size() > 0) return;
  if (!isWhite && m_blackFigures.size() > 0) return;

  for (int i = 1; i < 5; i++)
  {
    Rect curRect;

    Menu* figure = createFigure(i, isWhite, curRect);

    sumSizeWidthFigures += curRect.size.width;

    rectFigures.push_back(curRect);

    itemFigures.push_back(figure);
  }

  if (isWhite) m_whiteFigures = itemFigures;
  if (!isWhite) m_blackFigures = itemFigures;

  float deltaShift;
  Rect figureRect = rectFigures[0];
  deltaShift = 0.3 * figureRect.size.width;

  float initPosX = (tableSize.width - sumSizeWidthFigures)/2;
  Menu* f1 = itemFigures[0];
  f1->setPosition(Vec2(initPosX - tableSize.width/2, 0));

  // Set position
  for (int i = 0; i < itemFigures.size() - 1; i++)
  {
    Menu* prevFigure = itemFigures[i];
    Menu* curFigure = itemFigures[i + 1];

    Rect rectPrevFigure = rectFigures[i];

    Vec2 prevPosFigure = prevFigure->getPosition();

    curFigure->setPosition(prevPosFigure.x + rectPrevFigure.size.width + deltaShift, prevPosFigure.y);
  }
}

Menu* PromotionLayer::createFigure(int typeFigure, bool isWhite, Rect& figureRect)
{
  std::string pngName = (isWhite) ? Constants::WHITE_ROOK_PNG : Constants::BLACK_ROOK_PNG;

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

  figure->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pngName));
  figure->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pngName));
  figure->setScale(m_imageFigureScale);

  Rect rect = figure->rect();
  figureRect = rect;

  /*closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
    origin.y + closeItem->getContentSize().height / 2));*/

    // create menu, it's an autorelease object
  auto menu = Menu::create(figure, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 100);

  return menu;
}
