//
//  DemonChain.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/7/8.
//
//

#include "DemonChain.h"

DemonChain::DemonChain()
{
    m_fruitsChain.clear();
}

DemonChain::~DemonChain()
{
    
}

void DemonChain::addFruit(DemonFruit *fruit)
{
    if (fruit != nullptr)
    {
        m_fruitsChain.push_back(fruit);
    }
}

long DemonChain::getSize()
{
    return m_fruitsChain.size();
}

DemonFruit* DemonChain::getFirstFruit()
{
    
    return *m_fruitsChain.begin();//begin是迭代器
}

DemonFruit* DemonChain::getLastFruit()
{
    return m_fruitsChain.back();//back是引用
}