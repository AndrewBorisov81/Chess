#include "Figure.h"

USING_NS_CC;

Figure::Figure()
{
}

Figure::Figure(const std::string& fileName)
{
  
}

Figure::~Figure()
{
}

Figure* Figure::createFigure(const std::string& fileName)
{
  Figure* pFigure = new(std::nothrow) Figure(fileName);

  if (pFigure && pFigure->initWithSpriteFrameName(fileName))
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

