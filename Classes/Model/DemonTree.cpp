//
//  DemonTree.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/8/2.
//
//

#include "DemonTree.h"

DemonTree::DemonTree()
{
    m_hp = 10;
    m_totalHP = 10;
    m_basicDemange = 50;
}

DemonTree::~DemonTree()
{
    
}

DemonTree* DemonTree::create()
{
    DemonTree* tree = new DemonTree();
    if (tree)
    {
        auto frame = Sprite::create("tree.png");
        tree->initWithSpriteFrame(frame->getSpriteFrame());
        tree->autorelease();
        tree->initTree();
        return tree;
    }
    else
    {
        delete tree;
        tree = nullptr;
        return nullptr;
    }
}

void DemonTree::initTree()
{
    m_hp = 10;
    //生命值图标
    char buf[1024];
    sprintf(buf, "BossHP%02d.png",m_hp);
    m_spriteHP = Sprite::createWithSpriteFrameName(buf);
    m_spriteHP->setAnchorPoint(Point(0.5,0));
    m_spriteHP->setPosition(this->getContentSize().width/2,this->getContentSize().height*1.2);
    m_spriteHP->setScale(2);
    this->addChild(m_spriteHP);
    
}

//更新生命值图标
void DemonTree::updateHPSprite()
{
    char buf[1024];
    sprintf(buf, "BossHP%02d.png",m_hp);
    auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf);
    m_spriteHP->setSpriteFrame(frame);
}

void DemonTree::addHP(int value)
{
    m_hp += value;
    if(m_hp >= 10) m_hp = 10;
    updateHPSprite();
}

void DemonTree::minusHP(int value)
{
    m_hp -= value;
    if(m_hp <= 1)
    {
       m_hp = 1;
    }
    updateHPSprite();
}

int DemonTree::getHP()
{
    return m_hp;
}

void DemonTree::setTotalHP(int level)
{
    m_totalHP = level * 10;//每一级增加10生命值
    updateHPSprite();
}

void DemonTree::addDamage(int value)
{
    m_basicDemange += value;
}
