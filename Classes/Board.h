#pragma once

#include "cocos2d.h"

class Board : public cocos2d::Layer
{
public:
  Board();
  virtual ~Board();

  virtual bool init();
};
