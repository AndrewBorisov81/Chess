#pragma once

#include "cocos2d.h"

#include <functional>

class HudLayer: public cocos2d::Layer
{
public:
  HudLayer();
  virtual ~HudLayer();

  virtual bool init();

  // a selector callback
  void menuCloseCallback(cocos2d::Ref* pSender);

  void callBackUndoLastMove(const std::function<void()>& callBack);

private:
  std::function<void()> m_undoLastMove{ nullptr };
};
