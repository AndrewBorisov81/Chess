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

  void showPiece(bool isWhite);
  void hidePiece();

  // a selector callback
  void pieceCallback(cocos2d::Ref* pSender);

  void callBackClickPiece(const std::function<void(int)>& callBack);
  void callBackHide(const std::function<void(int)>& callBack);

  void createPiece(bool isWhite, cocos2d::Size tableSize);
  cocos2d::Menu* createPiece(int typePiece, bool isWhite, cocos2d::Rect& pieceRect);

private:
  bool m_isShow = false;

  cocos2d::Sprite* m_table{ nullptr };
  cocos2d::Size m_tableSize;
  float m_imagePiececale{ 0 };

  int m_tagPiece{ 1 };

  std::vector<cocos2d::Menu*> m_whitePiece;
  std::vector<cocos2d::Menu*> m_blackPiece;

  bool m_isWhite{ true };

  std::function<void(int)> m_clickPieceCallBack{ nullptr };
  std::function<void(int)> m_hideCallBack{ nullptr };
};
