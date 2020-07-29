#include "HomeScene.h"
#include "GameLayer.h"

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