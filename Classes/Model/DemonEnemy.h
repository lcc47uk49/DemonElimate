//
//  DemonEnemy.h
//  DemonElimate
//
//  Created by 凯 于 on 15/8/8.
//
//怪物类

#ifndef __DemonElimate__DemonEnemy__
#define __DemonElimate__DemonEnemy__

#include <stdio.h>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class DemonEnemy : public Sprite
{
public:
    DemonEnemy();
    ~DemonEnemy();
    static DemonEnemy* create(int index, int level);//根据类型、等级创建敌人
    void init(int index, int level);
    virtual void update(float dt);
public:
    //怪物动作
    void attack();//进攻 大树掉血
    void dead();//死亡
    void taunt();//嘲讽
    void skill();//释放技能 影响消除
    void hurt();//受到攻击 自己掉血
//    void run();//行走的动作在playLayer的update中完成还是做一个动作？
    CC_SYNTHESIZE(int, m_hp, HP);//生命值
    CC_SYNTHESIZE(float, m_speed, Speed);//速度
    CC_SYNTHESIZE(int, m_level, Level);//等级
    CC_SYNTHESIZE(int, m_index, Index);//种类
    CC_SYNTHESIZE(int, m_aggressivity, Aggressivity);//攻击力
    CC_SYNTHESIZE(int, m_state, State);//状态 1.run 2.attact 3.游戏结束做出嘲讽的动作 4.被减速 5.被定身
};
#endif /* defined(__DemonElimate__DemonEnemy__) */
