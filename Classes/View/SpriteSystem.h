//
//  SpriteSystem.h
//  DemonElimate
//
//  Created by 凯 于 on 15/10/12.
//
//

#ifndef __DemonElimate__SpriteSystem__
#define __DemonElimate__SpriteSystem__

#include <stdio.h>
#include "cocos2d.h"
#include "../Model/BaseSprite.h"
#include "../Model/Magic.h"
USING_NS_CC;

//战斗系统类
class SpriteSystem : public Layer
{
public:
    SpriteSystem();
    ~SpriteSystem();
    virtual bool init();
    virtual void update(float dt);//更新
    CREATE_FUNC(SpriteSystem);
    
    void enemyAttack(float dt);//怪物攻击使用定时器
    void heroAttack(BaseSprite* hero);//英雄攻击是消除果实时触发
    void showLifeTop(BaseSprite* sp);//显示人物头顶的生命值（敌人）
    void showLifeButton(BaseSprite* sp);//显示技能按钮上的生命值（英雄）
    void showPowerButton(BaseSprite* sprite);//显示技能按钮上的英雄的能量值
public:
    Vector<BaseSprite*> m_vHeros;//英雄们
    Vector<BaseSprite*> m_vEnemys;//敌人们
    Vector<Magic*> m_vMagics;//魔法们
    
private:
    void readBSJson(_stSpriteConfig (&spConf)[6]);//读取人物属性存入spConf[6]--BaseSprite中只是读取名字创建图片
    void readMagicJson(_stMagicConfig (&magConf)[9]);//读取技能属性
    void setMagicPanel();//设置技能按钮
    void itemCallback(Ref* pSender, int Num);//技能按钮回调
    void initBSandMagic();//初始化图片
    void setAttackTarget();//设置攻击目标
    int gameResult();//判断是否通关 0没完成，1胜利，2失败
private:
    _stSpriteConfig m_bsConf[6];//6个人物的属性结构体
    _stMagicConfig m_magConf[9];//9个技能的属性结构体
    
    float m_fAttackElapseTime[3];//记录敌人攻击后流失的时间
    float m_fBeginElaseTime;//游戏开始后流失的时间，当>4秒后(前4秒，人物在运动)，开始战斗
    float m_fOverElaspeTime;//记录了战斗结束后流失的时间，用于结束游戏跳转
    
    
    MenuItemSprite* m_itItem[3];//3个技能按钮
    ProgressTimer* m_ptLife[3];//生命值进度条
    ProgressTimer* m_ptPower[3];//能力值进度条
    ProgressTimer* m_ptLifeTop[3];//敌人生命值进度条
};

#endif /* defined(__DemonElimate__SpriteSystem__) */
