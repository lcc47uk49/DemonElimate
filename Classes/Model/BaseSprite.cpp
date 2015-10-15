//
//  BaseSprite.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/9/24.
//
//

#include "BaseSprite.h"

BaseSprite::BaseSprite()
{
    
}

BaseSprite::~BaseSprite()
{
    //析构的时候要release，因为人物死亡了就释放了，技能就不用，技能可以重复利用
    CC_SAFE_RELEASE_NULL(m_aIdleAction);
    CC_SAFE_RELEASE_NULL(m_aRunningAction);
    CC_SAFE_RELEASE_NULL(m_aAttackAction);
    CC_SAFE_RELEASE_NULL(m_aHurtAction);
    CC_SAFE_RELEASE_NULL(m_aDeadAction);
    CC_SAFE_RELEASE_NULL(m_aMagicAction);
}

BaseSprite* BaseSprite::create(_stSpriteConfig spConfig)
{
    BaseSprite* sprite = new BaseSprite();
    sprite->setAnchorPoint(Point(0.5,0.5));
    sprite->spConf = spConfig;//将人物的属性保存到类的成员中
    
    //初始化人物的当前能量值、生命值
    sprite->m_iCurLife = sprite->spConf.life;
    sprite->m_iCurPower = 0;
//    sprite->m_bIsMagic = false;
    
    sprite->setAllAction();//创建人物的所有动作并保存
    
    string spriteName = GameManager::getInstance()->getSpriteName(sprite->spConf.id);
    spriteName += "_run_001.png";//刚产生的时候是跑的动作
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteName);
    if (spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    else
    {
        delete sprite;
        sprite = nullptr;
        return nullptr;
    }
}

void BaseSprite::setAllAction()
{
    std::string str;
    str = GameManager::getInstance()->getStateName(this->spConf.id, SPRITESTATE_IDLE);
    m_aIdleAction = this->setAction(str.c_str(), spConf.stateNum.idleNum, 12, true, true);
    
    str = GameManager::getInstance()->getStateName(this->spConf.id, SPRITESTATE_RUN);
    m_aRunningAction = this->setAction(str.c_str(), spConf.stateNum.runNum, 12, true, true);
    
    str = GameManager::getInstance()->getStateName(this->spConf.id, SPRITESTATE_ATTACK);
    m_aAttackAction = this->setAction(str.c_str(), spConf.stateNum.attackNum, 12, false, true);
    
    m_aMagicAction = this->setAction(str.c_str(), spConf.stateNum.attackNum, 12, false, true);//释放技能也采用攻击的动作
    
    str = GameManager::getInstance()->getStateName(this->spConf.id, SPRITESTATE_HURT);
    m_aHurtAction = this->setAction(str.c_str(), spConf.stateNum.hurtNum, 12, false, true);
    
    str = GameManager::getInstance()->getStateName(this->spConf.id, SPRITESTATE_DEAD);
    m_aDeadAction = this->setAction(str.c_str(), spConf.stateNum.deadNum, 12, false, false);
}

Action* BaseSprite::setAction(const char* frameName, int count, int fps,
                              bool repeat, bool restore, int times)
{
    if (repeat)
    {
        auto tempAnimate = GameManager::getInstance()->setAnimate(frameName, count, fps, restore);
        auto repeat = RepeatForever::create(tempAnimate);
        CC_SAFE_RETAIN(repeat);
        return repeat;
    }
    //
    else
    {
        auto tempAnimate = GameManager::getInstance()->setAnimate(frameName, count, fps, restore);
        auto callfunc = CallFunc::create(CC_CALLBACK_0(BaseSprite::actionCallback, this));
        
        auto sequence = Sequence::create(tempAnimate, callfunc, NULL);
        CC_SAFE_RETAIN(sequence);//保存
        return sequence;
    }
}

void BaseSprite::startAction(SpriteState state)
{
    //执行动作前，先停止所有动作
    this->stopAllActions();
    //更新当前状态
    this->spriteState = state;
    
    switch (this->spriteState)
    {
        case SPRITESTATE_IDLE:
            this->runAction(m_aIdleAction);
            break;
            
        case SPRITESTATE_RUN:
            this->runAction(m_aRunningAction);
            break;
            
        case SPRITESTATE_ATTACK:
            this->runAction(m_aAttackAction);
            break;
            
        case SPRITESTATE_MAGIC:
            this->setScale(this->getScale()*1.1);
            this->runAction(m_aMagicAction);
//            this->m_bIsMagic = true;
            break;
            
        case SPRITESTATE_HURT:
            this->runAction(m_aHurtAction);
            break;
            
        case SPRITESTATE_DEAD:
            this->runAction(m_aDeadAction);
            break;
            
        default:
            break;
    }
}

void BaseSprite::actionCallback()
{
    SpriteState state = this->spriteState;
    
    switch (state)
    {
        case SPRITESTATE_IDLE:
            break;
        case SPRITESTATE_RUN:
            break;
        case SPRITESTATE_ATTACK://攻击动作结束后执行空闲动作
        {
            this->startAction(SPRITESTATE_IDLE);
        }
            break;
            
        case SPRITESTATE_MAGIC:
        {
            this->setScale(this->getScale() / 1.1);
//            this->m_bIsMagic = false;
            
            this->startAction(SPRITESTATE_IDLE);
        }
            break;
            
        case SPRITESTATE_HURT:
            break;
            
        case SPRITESTATE_DEAD:
        {
            auto fadeout = FadeOut::create(2.0f);
            auto seq = Sequence::create(fadeout, CallFunc::create(CC_CALLBACK_0(Sprite::setVisible, this,false)), NULL);
            this->runAction(seq);
            
            break;
        }
        default:
            break;
    }
}

void BaseSprite::move(MoveType moveType, Point endPoint, float dt)
{
    this->startAction(SPRITESTATE_RUN);
    auto callfunc = CallFunc::create(CC_CALLBACK_0(BaseSprite::moveCallback, this));
    
    if (moveType == MoveType_To)
    {
        auto moveAction = MoveTo::create(dt, endPoint);
        this->runAction(Sequence::create(moveAction, callfunc, NULL));
    }
    else
    {
        auto moveAction = MoveBy::create(dt, endPoint);
        this->runAction(Sequence::create(moveAction, callfunc, NULL));
    }
}

void BaseSprite::moveCallback()
{
    this->startAction(SPRITESTATE_IDLE);
}