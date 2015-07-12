//
//  DemonSwap.h
//  DemonElimate
//
//  Created by 凯 于 on 15/7/8.
//
//

#ifndef __DemonElimate__DemonSwap__
#define __DemonElimate__DemonSwap__

#include <stdio.h>
#include "cocos2d.h"
#include "DemonFruit.h"
USING_NS_CC;
//记录交换,交换要被加入Vector中，因此定义为Node
class DemonSwap : public Node
{
public:
    DemonSwap();
    ~DemonSwap();
    static DemonSwap* create(DemonFruit* fruitA, DemonFruit* fruitB);
    bool isEqual(DemonSwap* swap);//判断两个交换是否为相同

    DemonFruit* m_fruitA;
    DemonFruit* m_fruitB;
};

#endif /* defined(__DemonElimate__DemonSwap__) */
