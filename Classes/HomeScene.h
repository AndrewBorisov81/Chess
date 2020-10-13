#pragma once

#include "cocos2d.h"

class GameLayer;
class HudLayer;

USING_NS_CC;

enum class ZOrderLayer
{
  HOME = 888,
  GAME = 999,
  HUD = 1001
};

class HomeScene : public cocos2d::Scene
{
public:
  HomeScene();

  virtual bool init();

  //void update(float) override;

  static HomeScene* createScene();

private:
  GameLayer* m_gameLayer{ nullptr };
  HudLayer* m_hudLayer{ nullptr };

  GameLayer* createGameLayer();
};

