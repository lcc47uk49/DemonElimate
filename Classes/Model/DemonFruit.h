//
//  DemonFruit.h
//  DemonElimate
//
//  Created by 凯 于 on 15-7-6.
//
//

#ifndef __DemonElimate__DemonFruit__
#define __DemonElimate__DemonFruit__

#include <stdio.h>
#include "cocos2d.h"
#include "../Controller/GameManager.h"
USING_NS_CC;
using namespace std;

class DemonFruit : public Sprite
{
public:
    DemonFruit();//构造
    ~DemonFruit();
    static DemonFruit* create(int row, int col, int fruitType);
    void highLight();
    void unHighLight();
    void Hints();//提示效果
    void explode();//消除
    void actionEndCallBack(Node* node);//动作完成后删除节点
    
    Sprite* m_spHighlight;
    CC_SYNTHESIZE(int, m_row, Row);
    CC_SYNTHESIZE(int, m_col, Col);
    CC_SYNTHESIZE(int, m_fruitType, FruitType);
    float m_swapDelayTime;//交换成功动作耗时0.3秒，交换成功后在消除，因此这里设置个延迟
//    CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);//是否需要消除，初始为false
//    CC_SYNTHESIZE(bool, m_canBeMove, CanBeMove);//是否可移动，初始化为true
};

#endif /* defined(__DemonElimate__DemonFruit__) */
