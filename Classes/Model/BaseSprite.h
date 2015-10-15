//
//  BaseSprite.h
//  DemonElimate
//
//  Created by 凯 于 on 15/9/24.
//
//

#ifndef __DemonElimate__BaseSprite__
#define __DemonElimate__BaseSprite__

#include <stdio.h>
#include <string>
#include "../Configer.h"
#include "cocos2d.h"
#include "../Controller/GameManager.h"
USING_NS_CC;
using namespace std;

//人物类，数据转模型
class BaseSprite : public Sprite
{
public:
    BaseSprite();
    ~BaseSprite();
    
    //根据人物的属性创建人物图片
    static BaseSprite* create(_stSpriteConfig spConfig);
    //定义空闲动作，并保存，不用每次都创建了
    CC_SYNTHESIZE_RETAIN(Action*, m_aIdleAction, IdleAction);
    CC_SYNTHESIZE_RETAIN(Action*, m_aRunningAction, RunningAction);
    CC_SYNTHESIZE_RETAIN(Action*, m_aAttackAction, AttackAction);
    CC_SYNTHESIZE_RETAIN(Action*, m_aHurtAction, HurtAction);
    CC_SYNTHESIZE_RETAIN(Action*, m_aDeadAction, DeadAction);
    CC_SYNTHESIZE_RETAIN(Action*, m_aMagicAction, MagicAction);
    
    CC_SYNTHESIZE(_stSpriteConfig, spConf, SpConf);//人物的属性
    CC_SYNTHESIZE(BaseSprite*, targetSp, TargetSp);//攻击的目标
    CC_SYNTHESIZE(BaseSprite*, magicTargetSp, MagicTargetSP);//大招的目标
    
    CC_SYNTHESIZE(int, m_iCurPower, CurPower);//当前能量
    CC_SYNTHESIZE(int, m_iCurLife, CurLife);//当前的生命
//    CC_SYNTHESIZE(bool, m_bIsMagic, IsMagic);//是否释放魔法
    CC_SYNTHESIZE(SpriteState, spriteState, SpriteState);//当前状态
    
    void startAction(SpriteState state);//根据人物的状态执行对应的动作
    void move(MoveType moveType,Point endPoint,float dt);//移动
    
private:
    //创建动作，帧名，帧数，间隔，是否重复(永久循环)，是否回到起始帧，重复次数
    Action* setAction(const char* frameName, int count, int fps,
                      bool repeat, bool restore, int times=1);
    void setAllAction();//生成所有的动作，并存入对象的属性
    void actionCallback();//动作执行完成后的回调
    void moveCallback();//移动后的回调
};


#endif /* defined(__DemonElimate__BaseSprite__) */
