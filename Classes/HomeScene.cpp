#include "HomeScene.h"
#include "GameLayer.h"
#include "HudLayer.h"

HomeScene::HomeScene()
{

}

bool HomeScene::init()
{
  // 1. super init first
  if (!Scene::init())
  {
    return false;
  }

  GameLayer* pGameLayer = createGameLayer();
  if (pGameLayer)
  {
    m_gameLayer = pGameLayer;
    this->addChild(pGameLayer, static_cast<int>(ZOrderLayer::GAME));
  }

  HudLayer* pHudLayer = createHudLayer();
  if (pHudLayer)
  {
    m_hudLayer = pHudLayer;
    this->addChild(pHudLayer, static_cast<int>(ZOrderLayer::HUD));
  }

  this->scheduleUpdate();

  return true;
}


void HomeScene::update(float delta) {
  if(m_gameLayer)
    m_gameLayer->update(delta);
}

HomeScene* HomeScene::createScene()
{
  HomeScene* pRet = new(std::nothrow) HomeScene();
  if (pRet && pRet->init())
  {
    pRet->autorelease();
    return pRet;
  }
  else
  {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

HudLayer* HomeScene::createHudLayer()
{
  HudLayer* pHudLayer = new(std::nothrow) HudLayer();
  if (pHudLayer && pHudLayer->init())
  {
    pHudLayer->autorelease();
    return pHudLayer;
  }
  else
  {
    delete pHudLayer;
    pHudLayer = nullptr;
    return nullptr;
  }
}

GameLayer* HomeScene::createGameLayer()
{
  GameLayer* pGameLayer = new(std::nothrow) GameLayer();
  if (pGameLayer && pGameLayer->init())
  {
    pGameLayer->autorelease();
    return pGameLayer;
  }
  else
  {
    delete pGameLayer;
    pGameLayer = nullptr;
    return nullptr;
  }
}