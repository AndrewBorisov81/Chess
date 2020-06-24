#include "Figure.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Figure::Figure() :
  cocos2d::ui::ImageView()
{
}

Figure::Figure(const std::string& fileName) :
  cocos2d::ui::ImageView()
{

}


Figure::~Figure()
{
}

Figure* Figure::createFigure(const std::string& fileName)
{
  Figure* pFigure = new(std::nothrow) Figure(fileName);

  if (pFigure && pFigure->init(fileName, cocos2d::ui::ImageView::TextureResType::PLIST))
  {
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


