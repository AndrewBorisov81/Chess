#include "HudLayer.h"
#include "Constants.h"

USING_NS_CC;

HudLayer::HudLayer()
{
}

HudLayer::~HudLayer()
{
}

bool HudLayer::init()
{
  if (!Layer::init()) {

    return false;
  }

  Size winSize = Director::getInstance()->getWinSize();
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto closeItem = MenuItemImage::create(
    "undoButtonNormal.png",
    "undoButtonSelected.png",
    CC_CALLBACK_1(HudLayer::menuCloseCallback, this));

  closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
    origin.y + closeItem->getContentSize().height / 2));

  // create menu, it's an autorelease object
  auto menu = Menu::create(closeItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  return true;
}

void HudLayer::menuCloseCallback(Ref* pSender)
{
  if (m_undoLastMove)
    m_undoLastMove();
}

void HudLayer::callBackUndoLastMove(const std::function<void()>& callBack)
{
  m_undoLastMove = callBack;
}
