#pragma once

enum class GameMode
{
  HOME,
  PLAY,
  END,
  PLAY_AGAIN
};

enum class GameState
{
  STARTING,
  PREPARING,
  READY,
  PAUSE,
  FINISH,
  END
};

enum class ZOrderGame
{
  BOARD = 101,
  FIGURE = 202
};

#include "cocos2d.h"

class GameLayer : public cocos2d::Layer
{
public:

  GameLayer();
  virtual ~GameLayer();

  virtual bool init();
};
