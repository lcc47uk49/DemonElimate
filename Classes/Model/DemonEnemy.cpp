//
//  DemonEnemy.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/8/8.
//
//

#include "DemonEnemy.h"

DemonEnemy::DemonEnemy()
{
    m_level = 1;
    m_index = 1;
    m_hp = 50;
    m_aggressivity = 1;
    m_speed = 1;
    m_state = 1;
}

DemonEnemy::~DemonEnemy()
{
    
}

DemonEnemy* DemonEnemy::create(int index, int level)
{
    DemonEnemy* enemy = new DemonEnemy();
    if (enemy)
    {
        //view
        char buf[1024];
        sprintf(buf, "%d_%d_r.png",index,level);
        enemy->initWithSpriteFrameName(buf);
        enemy->autorelease();
        enemy->setFlippedX(true);
        //data
        enemy->init(index, level);
        
        return enemy;
    }
    else
    {
        delete enemy;
        enemy = nullptr;
        return nullptr;
    }
}

void DemonEnemy::init(int index, int level)
{
    m_index = index;
    m_level = level;
    //创建出来后就开始移动
    scheduleUpdate();
}

#pragma mark - 定时器

void DemonEnemy::update(float dt)
{
    if (m_state == 1)
    {
        this->setPosition(Point(this->getPositionX() - 10, this->getPositionY()));
    }
    else if(m_state == 3)
    {
        unscheduleUpdate();
        taunt();
    }
    if (m_hp <= 0)
    {
        dead();
    }
}

void DemonEnemy::attack()
{
    char buf[1024];
    Vector<SpriteFrame*> arr;
    for(int i = 1; i <= 5; i++)
    {
        sprintf(buf, "air%02d.png",i+10);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf);
        arr.pushBack(frame);
    }

    auto animation = Animation::createWithSpriteFrames(arr,0.1);
    this->runAction(Sequence::create(Animate::create(animation),
                                     CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, this)),
                                     nullptr));
}

void DemonEnemy::dead()
{
    char buf[1024];
    Vector<SpriteFrame*> arr;
    for(int i = 1; i <= 5; i++)
    {
        sprintf(buf, "air%02d.png",i+20);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf);
        arr.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(arr,0.1);
    this->runAction(Sequence::create(Animate::create(animation),
                                     CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, this)),
                                     nullptr));
}

//嘲讽
void DemonEnemy::taunt()
{
    char buf1[1024],buf2[1024];
    Vector<SpriteFrame*> arr;
    sprintf(buf1, "%d_%d_a.png",m_index,m_level);
    sprintf(buf2, "%d_%d_r.png",m_index,m_level);
    auto frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf1);
    arr.pushBack(frame1);
    auto frame2 = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf2);
    arr.pushBack(frame2);
    auto animation = Animation::createWithSpriteFrames(arr,0.3);
    this->runAction(Repeat::create(Animate::create(animation), 5));
}

void DemonEnemy::skill()
{
    
}

void DemonEnemy::hurt()
{
    
}