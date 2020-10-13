#pragma once

#include "PromptElement.h"
#include "Constants.h"

USING_NS_CC;

PromptElement::PromptElement()
{

}

PromptElement::PromptElement(TypePrompt typePrompt)
  :m_typePrompt(typePrompt)
{

}

PromptElement::~PromptElement()
{
}

bool PromptElement::init()
{
  if (!Node::init())
  {
    return false;
  }

  if (m_typePrompt == TypePrompt::PROMPT_RECT)
  {
    // Create Rects
    auto redRect = Sprite::createWithSpriteFrameName(Constants::RED_RECT_PNG);
    this->addChild(redRect, 1);
    m_redRect = redRect;

    auto greenRect = Sprite::createWithSpriteFrameName(Constants::GREEN_RECT_PNG);
    this->addChild(greenRect, 1);
    m_greenRect = greenRect;
  }
  else
  {
    // Create Circle
    auto circle = Sprite::createWithSpriteFrameName(Constants::GREEN_CIRCLE_PNG);
    this->addChild(circle, 1);
    m_circle = circle;
  }

  return true;
}

PromptElement* PromptElement::createPromptElement(TypePrompt typePrompt)
{
  PromptElement* pPromptElement = new(std::nothrow) PromptElement(typePrompt);

  if (pPromptElement && pPromptElement->init())
  {
    pPromptElement->autorelease();
    return pPromptElement;
  }
  else
  {
    delete pPromptElement;
    pPromptElement = nullptr;
    return nullptr;
  }
  return pPromptElement;
}

void PromptElement::show()
{
  if (m_typePrompt == TypePrompt::PROMPT_RECT)
  {
    if (m_greenRect && m_colorPrompt == ColorPrompt::GREEN)
      m_greenRect->setOpacity(255);

    if (m_redRect && m_colorPrompt == ColorPrompt::RED)
      m_redRect->setOpacity(255);
  }
  else
  {
    if (m_circle)
      m_circle->setOpacity(255);
  }
}

void PromptElement::hide()
{
  if (m_greenRect)
    m_greenRect->setOpacity(0);

  if (m_redRect)
    m_redRect->setOpacity(0);

  if (m_circle)
    m_circle->setOpacity(0);
}

void PromptElement::setColor(ColorPrompt colorPrompt)
{
  m_colorPrompt = colorPrompt;

  if (m_typePrompt == TypePrompt::PROMPT_RECT)
  {
    if (colorPrompt == ColorPrompt::GREEN)
    {
      m_greenRect->setOpacity(255);
      m_redRect->setOpacity(0);
    }
    else
    {
      m_greenRect->setOpacity(0);
      m_redRect->setOpacity(255);
    }
  }
}

void PromptElement::setCellPos(const cocos2d::Size & cellPos)
{
  m_cellPos = cellPos;
}

cocos2d::Size PromptElement::getCellPos()
{
  return m_cellPos;
}

TypePrompt PromptElement::getTypePrompt()
{
  return TypePrompt::PROMPT_CIRCLE;
}

void PromptElement::setTypePrompt(TypePrompt typePrompt)
{

}


