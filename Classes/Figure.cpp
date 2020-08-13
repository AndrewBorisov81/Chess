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

Figure::Figure(int type, bool isWhite, const std::string& fileName)
  :
  m_isWhite(isWhite),
  m_fileName(fileName),
  cocos2d::ui::ImageView()
{

}

Figure::~Figure()
{
}

/*Figure::Figure(const Figure& figure)
{
  this->init(figure._textureFile, cocos2d::ui::ImageView::TextureResType::PLIST);
  this->m_type = figure.m_type;
  this->m_isWhite = figure.m_isWhite;
  this->autorelease();
}*/

Figure::Figure(const Figure& figure)
{
  this->init(figure.m_fileName, cocos2d::ui::ImageView::TextureResType::PLIST);
  this->m_type = figure.m_type;
  this->m_isWhite = figure.m_isWhite;
  //this->autorelease();
}

Figure* Figure::createFigure(int type, bool isWhite, std::string fileName)
{
  //Figure* pFigure = new(std::nothrow) Figure(type, isWhite);
  Figure* pFigure = new(std::nothrow) Figure(type, isWhite, fileName);

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

cocos2d::Rect Figure::getRect()
{
  auto s = this->getContentSize();
  return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

Figure* Figure::cloneFigure()
{
  return new Figure(*this);
}

TypeFigure Figure::getType()
{
  return m_type;
}


