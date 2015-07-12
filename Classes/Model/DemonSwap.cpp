//
//  DemonSwap.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/7/8.
//
//

#include "DemonSwap.h"

DemonSwap::DemonSwap()
{
    m_fruitA = nullptr;
    m_fruitB = nullptr;
}

DemonSwap::~DemonSwap()
{
    
}

DemonSwap* DemonSwap::create(DemonFruit *fruitA, DemonFruit *fruitB)
{
    DemonSwap* swap = new DemonSwap();
    if (swap)
    {
        swap->m_fruitA = fruitA;
        swap->m_fruitB = fruitB;
        swap->autorelease();
        return swap;
    }
    else
    {
        delete swap;
        swap = nullptr;
        return nullptr;
    }
}

bool DemonSwap::isEqual(DemonSwap *swap)
{
    if ((this->m_fruitA == swap->m_fruitA
         && this->m_fruitB == swap->m_fruitB)
        ||
        (this->m_fruitB == swap->m_fruitA
         && this->m_fruitA == swap->m_fruitB)
        )
    {
        return true;
    }
    return false;
}