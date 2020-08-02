#pragma once

#include <cocos2d.h>

class PromotionLayer : public cocos2d::Layer
{
public:
  PromotionLayer();
  ~PromotionLayer();

  virtual bool init();

  cocos2d::Size getTableSize();

  void show();
  void hide();

  // a selector callback
  void figureCallback(cocos2d::Ref* pSender);

  void createFigures(int TypeFigure, bool isWhite); 

private:
  cocos2d::Size m_tableSize;
};
