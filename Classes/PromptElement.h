#pragma once

#include "cocos2d.h"

class PromptLogicHelper;

enum class TypePrompt
{
  PROMPT_RECT,
  PROMPT_CIRCLE
};

enum class ColorPrompt
{
  GREEN,
  RED
};

class PromptElement: public cocos2d::Node
{
public:
  PromptElement();
  PromptElement(TypePrompt typePrompt);
  virtual ~PromptElement();

  virtual bool init();

  static PromptElement* createPromptElement(TypePrompt typePrompt);

  void show();
  void hide();

  void setColor(ColorPrompt colorPrompt);

  void setCellPos(const cocos2d::Size& cellPos);
  cocos2d::Size getCellPos();

  TypePrompt getTypePrompt();
  void setTypePrompt(TypePrompt typeElement);

private:
  TypePrompt m_typePrompt{ TypePrompt::PROMPT_RECT };
  
  ColorPrompt m_colorPrompt{ ColorPrompt::GREEN };

  cocos2d::Sprite* m_greenRect{ nullptr };
  cocos2d::Sprite* m_redRect{ nullptr };
  cocos2d::Sprite* m_circle{ nullptr };

  cocos2d::Size m_cellPos{ -1, -1 };
};
