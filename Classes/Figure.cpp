#include "Figure.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Figure::Figure() :
  cocos2d::Sprite::Sprite()
{
}

Figure::Figure(const std::string& fileName) :
  cocos2d::Sprite::Sprite(),
  m_fileName(fileName)
{

}

void Figure::InitFigure()
{
  ImageView* imageView = ImageView::create(m_fileName, cocos2d::ui::ImageView::TextureResType::PLIST);
  m_imageView = imageView;
  m_imageView->setTouchEnabled(true);
  m_imageView->Node::setContentSize({ 250, 250 });
  m_imageView->addClickEventListener([=](Ref*) {
    m_imageView->setRotation(45);
  });
  this->addChild(imageView, 1);
}

Figure::~Figure()
{
}

Figure* Figure::createFigure(const std::string& fileName)
{
  Figure* pFigure = new(std::nothrow) Figure(fileName);

  if (pFigure && pFigure->init())
  {
    pFigure->InitFigure();
    pFigure->autorelease();
    return pFigure;
  }
  else
  {
    delete pFigure;
    pFigure = nullptr;
    return nullptr;
  }
  return pFigure;
}

cocos2d::ui::ImageView* Figure::getImageView()
{
  return m_imageView;
}


