#include "Figure.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Figure::Figure() :
  cocos2d::ui::ImageView()
{
}

Figure::Figure(TypeFigure type, ColourFigure colour, const std::string& fileName) :
  m_type(type),
  m_colour(colour),
  cocos2d::ui::ImageView()
{

}


Figure::~Figure()
{
}

Figure* Figure::createFigure(TypeFigure type, ColourFigure colour, const std::string& fileName)
{
  Figure* pFigure = new(std::nothrow) Figure(type, colour, fileName);

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

ColourFigure Figure::getColour()
{
  return m_colour;
}

TypeFigure Figure::getType()
{
  return m_type;
}


