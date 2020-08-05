#pragma once

#include <cocos2d.h>

#include <vector>

#include <functional>

class PromotionLayer : public cocos2d::Layer
{
public:
  PromotionLayer();
  ~PromotionLayer();

  virtual bool init();

  cocos2d::Size getTableSize();

  void show(bool isWhite);
  void hide();

  void showFigures(bool isWhite);
  void hideFigures();

  // a selector callback
  void figureCallback(cocos2d::Ref* pSender);

  void callBackClickFigure(const std::function<void(int)>& callBack);
  //void callBackHide(const std::function<void(int)>& callBack);

  void createFigures(bool isWhite, cocos2d::Size tableSize); 
  cocos2d::Menu* createFigure(int typeFigure, bool isWhite, cocos2d::Rect& figureRect);

private:
  cocos2d::Sprite* m_table{ nullptr };
  cocos2d::Size m_tableSize;
  float m_imageFigureScale{ 0 };

  int m_tagFigure{ 1 };

  std::vector<cocos2d::Menu*> m_whiteFigures;
  std::vector<cocos2d::Menu*> m_blackFigures;

  bool m_isWhite{ true };

  std::function<void(int)> m_clickFigure{ nullptr };
  //std::function<void(int)> m_hide{ nullptr };
};