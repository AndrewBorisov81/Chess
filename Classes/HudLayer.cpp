#include "HudLayer.h"

USING_NS_CC;

HudLayer::HudLayer()
{
}

HudLayer::~HudLayer()
{
}

bool HudLayer::init()
{
  if (!CCLayer::init()) {

    return false;
  }

  Size winSize = Director::getInstance()->getWinSize();
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // add a "close" icon to exit the progress. it's an autorelease object
  auto closeItem = MenuItemImage::create(
    "CloseNormal.png",
    "CloseSelected.png",
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
  if(m_undoLastMove)
    m_undoLastMove();

  //Close the cocos2d-x game scene and quit the application
  //Director::getInstance()->end();

/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif*/

  /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

  //EventCustom customEndEvent("game_scene_close_event");
  //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void HudLayer::callBackUndoLastMove(const std::function<void()>& callBack)
{
  m_undoLastMove = callBack;
}
