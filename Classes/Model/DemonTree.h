//
//  DemonTree.h
//  DemonElimate
//
//  Created by 凯 于 on 15/8/2.
//
//

#ifndef __DemonElimate__DemonTree__
#define __DemonElimate__DemonTree__

#include <stdio.h>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class DemonTree : public Sprite
{
public:
    DemonTree();
    ~DemonTree();
    static DemonTree* create();
    void addHP(int value);//增加value生命值
    void minusHP(int value);//减少value生命值
    int getHP();//获取当前生命值
    void setTotalHP(int level);//根据等级设置总生命值
    
    void addDamage(int value);//增加基础伤害
private:
    int m_hp;//生命值
    int m_totalHP;
    int m_basicDemange;//基础伤害，技能伤害由 基础伤害、技能等级和系数决定
//    int m_defence;//护甲
    
    
    
};
#endif /* defined(__DemonElimate__DemonTree__) */
