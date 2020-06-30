#include "Figure.h"
#include "Constants.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Figure::Figure() :
  cocos2d::ui::ImageView()
{
}

Figure::Figure(int type, bool isWhite) 
  :
  m_isWhite(isWhite),
  cocos2d::ui::ImageView()
{
  
}

Figure::~Figure()
{
}

Figure* Figure::createFigure(int type, bool isWhite, std::string fileName)
{
  Figure* pFigure = new(std::nothrow) Figure(type, isWhite);

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

bool Figure::isWhite()
{
  return m_isWhite;
}

void Figure::setType(TypeFigure type)
{
  m_type = type;
}

void Figure::setIsWhite(bool isWhite)
{
  m_isWhite = isWhite;
}

TypeFigure Figure::getType()
{
  return m_type;
}


