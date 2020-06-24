#include "Figure.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Figure::Figure() :
  cocos2d::ui::ImageView()
{
}

Figure::Figure(const std::string& fileName) :
  cocos2d::ui::ImageView(),
  m_fileName(fileName)
{

}

void Figure::InitFigure()
{
  this->setTouchEnabled(true);
  this->Node::setContentSize({ 250, 250 });
  this->addClickEventListener([=](Ref*) {
    this->setRotation(45);
  });
}

Figure::~Figure()
{
}

Figure* Figure::createFigure(const std::string& fileName)
{
  Figure* pFigure = new(std::nothrow) Figure(fileName);

  if (pFigure && pFigure->init(fileName, cocos2d::ui::ImageView::TextureResType::PLIST))
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


