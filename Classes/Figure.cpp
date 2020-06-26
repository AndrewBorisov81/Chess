#include "Figure.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Figure::Figure() :
  cocos2d::ui::ImageView()
{
}

Figure::Figure(TypeFigure typeFigure, const std::string& fileName) :
  m_typeFigure(typeFigure),
  cocos2d::ui::ImageView()
{

}


Figure::~Figure()
{
}

Figure* Figure::createFigure(TypeFigure typeFigure, const std::string& fileName)
{
  Figure* pFigure = new(std::nothrow) Figure(typeFigure, fileName);

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

void Figure::setColour(Colour colour)
{
  m_colour = colour;
}

void Figure::setTypeFigure(TypeFigure typeFigure)
{
  m_typeFigure = typeFigure;
}

Colour Figure::getColour()
{
  return Colour();
}

TypeFigure Figure::getTypeFigure()
{
  return TypeFigure();
}


