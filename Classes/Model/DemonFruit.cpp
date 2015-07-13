//
//  DemonFruit.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15-7-6.
//
//

#include "DemonFruit.h"

DemonFruit::DemonFruit()
{
    m_row = -1;
    m_col = -1;
    m_fruitType = -1;
    m_spHighlight = nullptr;
    m_swapDelayTime = 0.2;
//    m_isNeedRemove = false;
//    m_canBeMove = true;
}

DemonFruit::~DemonFruit()
{
    
}

DemonFruit* DemonFruit::create(int row, int col, int fruitType)
{
    DemonFruit* fruit = new DemonFruit();
    if (fruit)
    {
        fruit->m_row = row;
        fruit->m_col = col;
        fruit->m_fruitType = fruitType;
        vector<string> fruitsNameArr = GameManager::getInstance()->getusedFruitsName();
        fruit->initWithSpriteFrameName(fruitsNameArr[fruitType]);
        fruit->autorelease();
        
        //将宽度缩放到64，高等比例缩小
        Size contentSize = fruit->getContentSize();
        float scale = 64 / contentSize.width;
        fruit->setScale(scale);
        
        fruit->m_spHighlight = Sprite::createWithSpriteFrameName(GameManager::getInstance()->getHighLightFruitName(fruit->m_fruitType));
        fruit->addChild(fruit->m_spHighlight);
        fruit->m_spHighlight->setPosition(fruit->getContentSize().width/2,fruit->getContentSize().height/2);
        fruit->m_spHighlight->setVisible(false);
        return fruit;
    }
    else
    {
        delete fruit;
        fruit = nullptr;
        return nullptr;
    }
}

void DemonFruit::highLight()
{
    if (m_spHighlight != nullptr)
    {
        m_spHighlight->setVisible(true);

    }
}

void DemonFruit::unHighLight()
{
    if (m_spHighlight != nullptr)
    {
        m_spHighlight->setVisible(false);
    }
}

void DemonFruit::Hints()
{
    if (m_spHighlight != nullptr)
    {
        m_spHighlight->runAction(Blink::create(1.5, 3));
    }
    
}

void DemonFruit::explode()
{
    float time = 0.2;
    Point pos = this->getPosition();
    int index = this->getFruitType();
    this->runAction(Sequence::create(DelayTime::create(m_swapDelayTime),
                                     ScaleTo::create(time, 0.0),
                                     CallFuncN::create(CC_CALLBACK_1(DemonFruit::actionEndCallBack, this)),
                                     NULL));
}

void DemonFruit::actionEndCallBack(Node* node)
{
    if (node != nullptr)
    {
        node->removeFromParent();
        node = nullptr;
    }
}
