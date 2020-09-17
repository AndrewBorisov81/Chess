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

  m_imagePiececale = 1.5;

  // Load the Sprite Sheet
  auto spritecache = SpriteFrameCache::getInstance();
  spritecache->addSpriteFramesWithFile("texture.plist");

  // Table image
  auto tableSprite = Sprite::createWithSpriteFrameName(Constants::PROMOTION_TABLE);
  m_table = tableSprite;
  Size tableSize = tableSprite->getContentSize();
  m_tableSize = tableSize;
  this->addChild(tableSprite, 1);

  // Create white piece
  createPiece(true, tableSize);

  // Create black piece
  createPiece(false, tableSize);

  hide();

  return true;
}

cocos2d::Size PromotionLayer::getTableSize()
{
  return m_tableSize;
}

void PromotionLayer::show(bool isWhite)
{
  std::vector<Menu*> itemPiece;

  m_isWhite = isWhite;

  if (m_isWhite)
    itemPiece = m_whitePiece;
  else
    itemPiece = m_blackPiece;

  for (int i = 0; i < itemPiece.size(); i++)
  {
    Menu* piece = itemPiece[i];
    piece->setOpacity(255);
  }

  m_table->setOpacity(255);

  m_isShow = true;
}

void PromotionLayer::hide()
{
  hidePiece();
  
  m_table->setOpacity(0);

  if (m_hideCallBack)
  {
    int tagPiece = m_tagPiece;
    m_hideCallBack(tagPiece);
  }

  m_isShow = false;
}

void PromotionLayer::showPiece(bool isWhite)
{
  std::vector<Menu*> itemPiece;

  if (m_isWhite)
    itemPiece = m_whitePiece;
  else
    itemPiece = m_blackPiece;

  for (unsigned int i = 0; i < itemPiece.size(); i++)
  {
    Menu* piece = itemPiece[i];
    piece->setOpacity(255);
  }
}

void PromotionLayer::hidePiece()
{
  for (unsigned int i = 0; i < m_whitePiece.size(); i++)
  {
    Menu* piece = m_whitePiece[i];
    piece->setOpacity(0);
  }

  for (unsigned int i = 0; i < m_blackPiece.size(); i++)
  {
    Menu* piece = m_blackPiece[i];
    piece->setOpacity(0);
  }
}

void PromotionLayer::pieceCallback(cocos2d::Ref * pSender)
{
  if (m_isShow)
  {
    auto menu = static_cast<Menu*>(static_cast<Node*>(pSender)->getParent());
    int tagPiece = menu->getTag();
    m_tagPiece = tagPiece;
    if(m_clickPieceCallBack)
      m_clickPieceCallBack(tagPiece);
  }
}

void PromotionLayer::callBackClickPiece(const std::function<void(int)>& callBack)
{
  m_clickPieceCallBack = callBack;
}

void PromotionLayer::callBackHide(const std::function<void(int)>& callBack)
{
  m_hideCallBack = callBack;
}

void PromotionLayer::createPiece(bool isWhite, Size tableSize)
{
  std::vector<Menu*> itemPiece;
  std::vector<Rect> rectPiece;

  float sumSizeWidthPiece{ 0 };

  if (isWhite && m_whitePiece.size() > 0) return;
  if (!isWhite && m_blackPiece.size() > 0) return;

  for (int i = 1; i < 5; i++)
  {
    Rect curRect;

    Menu* piece = createPiece(i, isWhite, curRect);
    
    piece->setTag(i);

    sumSizeWidthPiece += curRect.size.width;

    rectPiece.push_back(curRect);

    itemPiece.push_back(piece);
  }

  if (isWhite) m_whitePiece = itemPiece;
  if (!isWhite) m_blackPiece = itemPiece;

  float deltaShift;
  Rect pieceRect = rectPiece[0];
  deltaShift = 0.3 * pieceRect.size.width;

  float initPosX = (tableSize.width - sumSizeWidthPiece)/2;
  Menu* f1 = itemPiece[0];
  f1->setPosition(Vec2(initPosX - tableSize.width/2, 0));

  // Set position
  for (unsigned int i = 0; i < itemPiece.size() - 1; i++)
  {
    Menu* prevPiece = itemPiece[i];
    Menu* curPiece = itemPiece[i + 1];

    Rect rectPrevPiece = rectPiece[i];

    Vec2 prevPosPiece = prevPiece->getPosition();

    curPiece->setPosition(prevPosPiece.x + rectPrevPiece.size.width + deltaShift, prevPosPiece.y);
  }
}

Menu* PromotionLayer::createPiece(int typePiece, bool isWhite, Rect& pieceRect)
{
  std::string pngName = (isWhite) ? Constants::WHITE_ROOK_PNG : Constants::BLACK_ROOK_PNG;

  // 2 - KNIGHT; 3 - BISHOP; 4 - QWEEN 
  switch (typePiece) {
  case 2:
    pngName = (isWhite) ? Constants::WHITE_KNIGHT_PNG : Constants::BLACK_KNIGHT_PNG;
    break;

  case 3:
    pngName = (isWhite) ? Constants::WHITE_BISHOP_PNG : Constants::BLACK_BISHOP_PNG;
    break;

  case 4:
    pngName = (isWhite) ? Constants::WHITE_QUEEN_PNG : Constants::BLACK_QUEEN_PNG;
    break;
  }

  auto piece = MenuItemImage::create(
    pngName,
    pngName,
    CC_CALLBACK_1(PromotionLayer::pieceCallback, this));

  piece->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pngName));
  piece->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pngName));
  piece->setScale(m_imagePiececale);

  Rect rect = piece->rect();
  pieceRect = rect;

  /*closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
    origin.y + closeItem->getContentSize().height / 2));*/

    // create menu, it's an autorelease object
  auto menu = Menu::create(piece, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 100);

  return menu;
}
