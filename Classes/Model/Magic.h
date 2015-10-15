//
//  Magic.h
//  DemonElimate
//
//  Created by 凯 于 on 15/10/12.
//
//

#ifndef __DemonElimate__Magic__
#define __DemonElimate__Magic__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseSprite.h"
#include "../Configer.h"
USING_NS_CC;

//远程攻击、魔法技能类
class Magic : public Sprite
{
public:
    Magic();
    ~Magic();
    static Magic* create(_stMagicConfig stMagConf);
    CC_SYNTHESIZE(_stMagicConfig, m_stMagConf, stMagConf);
    CC_SYNTHESIZE_RETAIN(Animate*, m_aMagAction, aMagAction);
    void startMagic(BaseSprite* attack, BaseSprite* hurt, float dt);//开始释放魔法的动作，接口
private:
    void setMagic();//设置动画
    void startMagicCallback(BaseSprite* attack, BaseSprite* hurt);//动作完成回调，隐藏魔法
};

#endif /* defined(__DemonElimate__Magic__) */
