//
//  DemonChain.h
//  DemonElimate
//
//  Created by 凯 于 on 15/7/8.
//
//

#ifndef __DemonElimate__DemonChain__
#define __DemonElimate__DemonChain__

#include <stdio.h>
#include "cocos2d.h"
#include "DemonFruit.h"
USING_NS_CC;

typedef enum ChainType
{
    ChainTypeHorizontal,
    ChainTypeVertical,
    
    // Note: add any other shapes you want to detect to this list.
    //ChainTypeL,
    //ChainTypeT,
}ChainType;

//记录消除链,消除链要被加入到Vector中，所以定义成Node，消除链使用vector而不是Vector保存果实，方便移除
class DemonChain : public Node
{
public:
    DemonChain();
    ~DemonChain();
    void addFruit(DemonFruit* fruit);
    CREATE_FUNC(DemonChain);
    long getSize();
    DemonFruit* getFirstFruit();//获得链中的第一个果实
    DemonFruit* getLastFruit();//获得链中的最后一个果实
    
    
    
    ChainType m_chainType;
    vector<DemonFruit*> m_fruitsChain;
    int m_score;
};

#endif /* defined(__DemonElimate__DemonChain__) */
