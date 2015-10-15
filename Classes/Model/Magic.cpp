//
//  Magic.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/10/12.
//
//

#include "Magic.h"
#include "../Controller/GameManager.h"

Magic::Magic()
{
    m_aMagAction = nullptr;//要初始化，因为setMagic进行判空了
}

Magic::~Magic()
{
    CC_SAFE_RELEASE_NULL(m_aMagAction);
}

Magic* Magic::create(_stMagicConfig stMagConf)
{
    Magic* sp = new Magic();
    sp->m_stMagConf = stMagConf;
    string spName = GameManager::getInstance()->getMagicName(sp->m_stMagConf.id);
    spName += "1.png";//获取某个技能所有图片中的第一个图片的全名
    auto spFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spName);
    if (spFrame && sp->initWithSpriteFrame(spFrame))
    {
        sp->autorelease();
        sp->setMagic();
        return sp;
    }
    else
    {
        delete  sp;
        sp = nullptr;
        return nullptr;
    }
}

void Magic::setMagic()
{
    //动画
    if (m_aMagAction == nullptr)
    {
        string str;
        str = m_stMagConf.name;
        str += "%d.png";
        
        Vector<SpriteFrame*> vec;
        for (int i = 1; i <= m_stMagConf.count; i++)
        {
            const char* name = String::createWithFormat(str.c_str(),i)->getCString();
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
            vec.pushBack(frame);
        }
        Animation* animation = Animation::createWithSpriteFrames(vec,0.08);
        Animate* animate = Animate::create(animation);
        
        setaMagAction(animate);//这里有retain
    }
}

void Magic::startMagic(BaseSprite* attack, BaseSprite* hurt, float dt)
{
    Point attackPos = attack->getPosition();
    Point hurtPos = hurt->getPosition();
    if (dt != 0)//远程攻击 有弹道
    {
        this->setPosition(attackPos);
        auto mv = MoveTo::create(dt, hurtPos);
        auto seq = Sequence::create(mv,CallFunc::create(CC_CALLBACK_0(Magic::startMagicCallback, this,attack, hurt)), NULL);
        this->runAction(seq);
        this->runAction(RepeatForever::create(m_aMagAction));
    }
    else//没有弹道，直接出现在敌人脚下,动画不循环
    {
        this->setPosition(hurtPos);
        auto seq = Sequence::create(m_aMagAction,CallFunc::create(CC_CALLBACK_0(Magic::startMagicCallback, this,attack, hurt)), NULL);
        this->runAction(seq);
    }
}

void Magic::startMagicCallback(BaseSprite* attack, BaseSprite* hurt)
{
    //更新生命值
    //魔法攻击不随机
    int magicAttack = this->m_stMagConf.attack;
//    int randAttack = attack->getSpConf().attackMin + rand()%(attack->getSpConf().attackMax - attack->getSpConf().attackMin);
    if (this->m_stMagConf.id == 8)
    {
        //加血
        attack->getMagicTargetSP()->setCurLife(attack->getMagicTargetSP()->getCurLife() + magicAttack);//可以在json中用正负攻击力表示加血扣血
    }
    else
    {
        //扣血
        attack->getMagicTargetSP()->setCurLife(attack->getMagicTargetSP()->getCurLife() - magicAttack);
    }
    this->stopAllActions();
    this->removeFromParent();
}