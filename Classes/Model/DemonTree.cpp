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
    m_hp = 50;
    m_totalHP = 50;
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
        tree->m_hp = 50;
        auto frame = Sprite::create("tree.png");
        tree->initWithSpriteFrame(frame->getSpriteFrame());
        tree->autorelease();
        return tree;
    }
    else
    {
        delete tree;
        tree = nullptr;
        return nullptr;
    }
}

void DemonTree::addHP(int value)
{
    m_hp += value;
}

void DemonTree::minusHP(int value)
{
    m_hp -= value;
}

int DemonTree::getHP()
{
    return m_hp;
}

void DemonTree::setTotalHP(int level)
{
    m_totalHP = level * 50;//每一级增加50生命值
}

void DemonTree::addDamage(int value)
{
    m_basicDemange += value;
}
