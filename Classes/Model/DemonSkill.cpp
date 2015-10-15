//
//  DemonSkill.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/7/24.
//
//

#include "DemonSkill.h"

//singleton stuff
static DemonSkill *s_SharedSkill = nullptr;
DemonSkill* DemonSkill::getInstance()
{
    if ( !s_SharedSkill)
    {
        s_SharedSkill = new DemonSkill();
    }
    return s_SharedSkill;
}

DemonSkill::DemonSkill()
{
    m_level = nullptr;
}

DemonSkill::~DemonSkill()
{
    
}

bool DemonSkill::initWithLevel(DemonLevel *level)
{
    if (m_level == nullptr)
    {
        m_level = level;
        return true;
    }
    return false;
}

#pragma mark - skills

void DemonSkill::treatSkill()
{
//    CCLOG("treatSkill");
//    int level = GameManager::getInstance()->getSkillLevel(0);
//    int treatHP = level * 5;//每一级多治疗5HP
//    //大树的生命值增加treatHP
//    m_level->m_tree->addHP(treatHP);
//    Point pos = Point(m_level->m_tree->getPositionX(),
//                      m_level->m_tree->getPositionY() - m_level->m_tree->getContentSize().height/3);
//    Point pos2 = Point(m_level->m_tree->getPositionX(),
//                      m_level->m_tree->getPositionY());
//    //效果1--要改，同治愈果实
//    auto cache = SpriteFrameCache::getInstance();
//    cache->addSpriteFramesWithFile("explodetreat.plist");
//    auto sp1  = Sprite::createWithSpriteFrameName("explodetreat_1.png");
//    sp1->setPosition(pos);
//    sp1->setScale(2);
//    m_level->addChild(sp1,10);
//    
//    Vector<SpriteFrame*> arr;
//    char buf[1024] = {0};
//    for (int i = 2; i <= 17; i++)
//    {
//        sprintf(buf, "explodetreat_%d.png",i);
//        auto frame = cache->getSpriteFrameByName(buf);
//        arr.pushBack(frame);
//    }
//    auto animation = Animation::createWithSpriteFrames(arr,0.05);
//    sp1->runAction(Sequence::create(Animate::create(animation),
//                                    CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp1)),nullptr));
//    
//    //效果2
//    auto sp2 = Sprite::createWithSpriteFrameName("explodetreat_18.png");
//    sp2->setPosition(pos2);
//    m_level->addChild(sp2,10);
//    
//    Vector<SpriteFrame*> arr2;
//    char buf2[1024] = {0};
//    for (int i = 19; i <= 27; i++)
//    {
//        sprintf(buf2, "explodetreat_%d.png",i);
//        auto frame = cache->getSpriteFrameByName(buf2);
//        arr2.pushBack(frame);
//    }
//    auto animation2 = Animation::createWithSpriteFrames(arr2,0.05);
//    sp2->runAction(Sequence::create(Animate::create(animation2),
//                                    CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp2)),nullptr));
//    
}

void DemonSkill::goldSkill()
{
    CCLOG("goldSkill");
    int level = GameManager::getInstance()->getSkillLevel(1);
    //效果同治愈果实
}

void DemonSkill::powerSkill()
{
//    CCLOG("powerSkill");
//    int level = GameManager::getInstance()->getSkillLevel(2);
//    //基础伤害每一级增加50
//    int damage = level * 50;
//    m_level->m_tree->addDamage(damage);
//    
//    //效果1
//    Point pos = Point(m_level->m_tree->getPositionX(),
//                       m_level->m_tree->getPositionY() - m_level->m_tree->getContentSize().height/3);
//    Point pos2 = Point(m_level->m_tree->getPositionX(),
//                       m_level->m_tree->getPositionY());
//    auto cache = SpriteFrameCache::getInstance();
//    cache->addSpriteFramesWithFile("explodepower.plist");
//    auto sp1  = Sprite::createWithSpriteFrameName("explodepower_1.png");
//    sp1->setPosition(pos);
//    m_level->addChild(sp1,10);
//    
//    float time = 0.1;
//    Vector<SpriteFrame*> arr;
//    char buf[1024] = {0};
//    for (int i = 2; i <= 6; i++)
//    {
//        sprintf(buf, "explodepower_%d.png",i);
//        auto frame = cache->getSpriteFrameByName(buf);
//        arr.pushBack(frame);
//    }
//    auto animation = Animation::createWithSpriteFrames(arr,time);
//    auto animate = Animate::create(animation);
//    auto mvTo = MoveTo::create(3*time, pos2);
//    auto spawn = Spawn::create(animate,mvTo, NULL);
//    sp1->runAction(Sequence::create(spawn,
//                                    CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp1)),nullptr));
//    //效果2,加攻击 向上的小箭头，配字pwoerup
    
}

void DemonSkill::fireSkill(DemonChain* chain)//根据技能等级选择若干个果实，1-2x2,2-2x3,3-3x3,4-3x4,5-4x4，首先选择左下角的
{
    CCLOG("fireSkill");
    int level = GameManager::getInstance()->getSkillLevel(3);
    Point pos1,pos2;
    int randRow = rand() % (__FRUIT_MATRIX_HEIGHT - (level+1)/2);
    int randCol = rand() % (__FRUIT_MATRIX_WIDTH - 1 - level/2);
    pos1 = m_level->getPosOfItem(randRow, randCol);//左下角的点
    pos2 = m_level->getPosOfItem(randRow + (level+1)/2, randCol + 1 + level/2);//右上角的点
    for (int row = randRow; row <= randRow + (level+1)/2; row++)
    {
        for (int col = randCol; col <= randCol + 1 + level/2; col++)
        {
            if (m_level->m_fruitMatrix[row][col] != nullptr)
            {
                DemonFruit* fruit = m_level->m_fruitMatrix[row][col];
                m_level->m_fruitMatrix[row][col] = nullptr;
                m_level->m_iMatrix[row][col] = -1;//标记下，在回调中更新m_fruitMatrix
                chain->addFruit(fruit);
            }
        }
    }
    //爆破动画
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("explosion.plist");
    auto sp1 = Sprite::createWithSpriteFrameName("explosion_09.png");
    sp1->setPosition((pos1+pos2)/2);
    m_level->m_clippingNode->addChild(sp1,10);
    float scalex = 2.5;
    float scaley = 2.5;
    switch (level)
    {
        case 1:
        {
            scalex = 2.5;
            scaley = 2.5;
        }
            break;
        case 2:
        {
            scalex = 3.25;
            scaley = 2.5;
        }
            break;
        case 3:
        {
            scalex = 3.25;
            scaley = 3.25;
        }
            break;
        case 4:
        {
            scalex = 4;
            scaley = 3.25;
        }
            break;
        case 5:
        {
            scalex = 4;
            scaley = 4;
        }
            break;
        default:
            break;
    }
    sp1->setScaleX(scalex);
    sp1->setScaleY(scaley);
    
    Vector<SpriteFrame*> arr;
    char buf[1024] = {0};
    for (int i = 10; i <= 15; i++)
    {
        sprintf(buf, "explosion_%02d.png",i);
        auto frame = cache->getSpriteFrameByName(buf);
        arr.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(arr,0.05);
    sp1->runAction(Sequence::create(Animate::create(animation),
                                    CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp1)),nullptr));
}

void DemonSkill::iceSkill(DemonChain* chain)//根据技能等级随机选择若干个果实引爆 1-4,2-6,3-9,4-12,5-16
{
    CCLOG("iceSkill");
    int level = GameManager::getInstance()->getSkillLevel(4);
    int num = 0;
    switch (level)
    {
        case 1:
            num = 4;
            break;
        case 2:
            num = 6;
            break;
        case 3:
            num = 9;
            break;
        case 4:
            num = 12;
            break;
        case 5:
            num = 16;
            break;
        default:
            break;
    }
    vector<Point> pointVector;
    pointVector.clear();
    
    //随机选择9个不同且不为空的果实
    for (int i = 0; i < num; i++)
    {
        int randRow,randCol;
        DemonFruit* fruit;
        //1.判断是否重复
        do {
            randRow = rand() % __FRUIT_MATRIX_HEIGHT;
            randCol = rand() % __FRUIT_MATRIX_WIDTH;
        } while (GameTools::getInstance()->isContain(pointVector, Point(randRow,randCol)));
        //2.判断是否为空
        fruit = m_level->m_fruitMatrix[randRow][randCol];
        if (fruit != nullptr)
        {
            pointVector.push_back(Point(randRow,randCol));//不重复且不为空，则加入向量
            //更新矩阵
            m_level->m_fruitMatrix[randRow][randCol] = nullptr;
            m_level->m_iMatrix[randRow][randCol] = -1;
        }
        else
        {
            i--;//如果不重复但是选择到了空的，则需要多选择一次，保证最终选择9个
            continue;
        }
        //3.加入冰霜消除链中
        chain->addFruit(fruit);
    }
    
    //爆破动画
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("explodeice.plist");
    
    for (vector<Point>::iterator it = pointVector.begin(); it != pointVector.end(); it++)
    {
        auto sp1 = Sprite::createWithSpriteFrameName("explodeice_1.png");
        Point point = *it;
        Point pos = m_level->getPosOfItem(point.x, point.y);
        sp1->setPosition(pos);
        sp1->setScale(0.5);
        m_level->m_clippingNode->addChild(sp1,10);
        
        Vector<SpriteFrame*> arr;
        char buf[1024] = {0};
        for (int i = 2; i <= 10; i++)
        {
            sprintf(buf, "explodeice_%d.png",i);
            auto frame = cache->getSpriteFrameByName(buf);
            arr.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(arr,0.04);
        sp1->runAction(Sequence::create(Animate::create(animation),
                                        CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp1)),nullptr));
    }
    

}

void DemonSkill::lightSkill(DemonChain* chain)//根据技能等级随机选择若干行或者列果实引爆
{
    CCLOG("lightSkill");
    int level = GameManager::getInstance()->getSkillLevel(5);
    Point pos1 = Point(0,0);
    Point pos2= Point(0,0);
    Point pos3= Point(0,0);
    Point pos4 = Point(0,0);//技能效果坐标
    switch (level)
    {
        case 1:
        {
            //随机选择一行的4个消除，随机选择最左边的果实
            int randRow = rand() % __FRUIT_MATRIX_HEIGHT;
            int randColBegin = rand() % (__FRUIT_MATRIX_WIDTH - 3);
            pos1 = m_level->getPosOfItem(randRow, randColBegin);
            pos2 = m_level->getPosOfItem(randRow, randColBegin + 3);//记录技能效果的起始终止坐标
            for (int i = 0; i < 4; i++)
            {
                if (m_level->m_fruitMatrix[randRow][randColBegin + i] != nullptr)
                {
                    DemonFruit* fruit = m_level->m_fruitMatrix[randRow][randColBegin + i];
                    m_level->m_fruitMatrix[randRow][randColBegin + i] = nullptr;
                    m_level->m_iMatrix[randRow][randColBegin + i] = -1;
                    chain->addFruit(fruit);
                }
            }
        }
            break;
        case 2:
        {
            //随机选择一行的6个消除
            int randRow = rand() % __FRUIT_MATRIX_HEIGHT;
            int randColBegin = rand() % (__FRUIT_MATRIX_WIDTH - 5);
            pos1 = m_level->getPosOfItem(randRow, randColBegin);
            pos2 = m_level->getPosOfItem(randRow, randColBegin + 5);
            for (int i = 0; i < 6; i++)
            {
                if (m_level->m_fruitMatrix[randRow][randColBegin + i] != nullptr)
                {
                    DemonFruit* fruit = m_level->m_fruitMatrix[randRow][randColBegin + i];
                    m_level->m_fruitMatrix[randRow][randColBegin + i] = nullptr;
                    m_level->m_iMatrix[randRow][randColBegin + i] = -1;
                    chain->addFruit(fruit);
                }
            }
        }
            break;
        case 3:
        {
            //随机选择一行消除
            int randRow = rand() % __FRUIT_MATRIX_HEIGHT;
            int randColBegin = 0;
            pos1 = m_level->getPosOfItem(randRow, randColBegin);
            pos2 = m_level->getPosOfItem(randRow, randColBegin + 8);
            for (int i = 0; i < __FRUIT_MATRIX_WIDTH; i++)
            {
                if (m_level->m_fruitMatrix[randRow][randColBegin + i] != nullptr)
                {
                    DemonFruit* fruit = m_level->m_fruitMatrix[randRow][randColBegin + i];
                    m_level->m_fruitMatrix[randRow][randColBegin + i] = nullptr;
                    m_level->m_iMatrix[randRow][randColBegin + i] = -1;
                    chain->addFruit(fruit);
                }
            }
        }
            break;
        case 4:
        {
            //随机选择一行消除并在此行上随机选择一列，消除5个
            int randRow = rand() % (__FRUIT_MATRIX_HEIGHT - 4) + 2;//选择2~6行
            int randColBegin = 0;
            pos1 = m_level->getPosOfItem(randRow, randColBegin);
            pos2 = m_level->getPosOfItem(randRow, randColBegin + 8);
            for (int i = 0; i < __FRUIT_MATRIX_WIDTH; i++)//行
            {
                if (m_level->m_fruitMatrix[randRow][randColBegin + i] != nullptr)
                {
                    DemonFruit* fruit = m_level->m_fruitMatrix[randRow][randColBegin + i];
                    m_level->m_fruitMatrix[randRow][randColBegin + i] = nullptr;
                    m_level->m_iMatrix[randRow][randColBegin + i] = -1;
                    chain->addFruit(fruit);
                }
            }
            int randNo = rand() % __FRUIT_MATRIX_WIDTH;//随机选一列
            pos3 = m_level->getPosOfItem(randRow - 2, randNo);
            pos4 = m_level->getPosOfItem(randRow + 2, randNo);
            for (int i = -2; i <= 2; i++)
            {
                if (m_level->m_fruitMatrix[randRow+i][randNo] != nullptr)
                {
                    DemonFruit* fruit = m_level->m_fruitMatrix[randRow+i][randNo];
                    m_level->m_fruitMatrix[randRow+i][randNo] = nullptr;
                    m_level->m_iMatrix[randRow+i][randNo] = -1;
                    chain->addFruit(fruit);
                }
            }
        }
            break;
        case 5:
        {
            //随机选择一行、一列消除
            int randRow = rand() % __FRUIT_MATRIX_HEIGHT ;
            int randCol = rand() % __FRUIT_MATRIX_WIDTH;
            pos1 = m_level->getPosOfItem(randRow, 0);
            pos2 = m_level->getPosOfItem(randRow, 8);
            pos3 = m_level->getPosOfItem(0, randCol);
            pos4 = m_level->getPosOfItem(8, randCol);
            for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)//行
            {
                DemonFruit* fruit = m_level->m_fruitMatrix[randRow][col];
                if (fruit != nullptr)
                {
                    chain->addFruit(fruit);
                    //更新矩阵
                    m_level->m_fruitMatrix[randRow][col] = nullptr;
                    m_level->m_iMatrix[randRow][col] = -1;
                }
            }
            for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)//列
            {
                DemonFruit* fruit = m_level->m_fruitMatrix[row][randCol];
                if (fruit != nullptr)
                {
                    chain->addFruit(fruit);
                    //更新矩阵
                    m_level->m_fruitMatrix[row][randCol] = nullptr;
                    m_level->m_iMatrix[row][randCol] = -1;
                }
            }
        }
            break;
        default:
            break;
    }
    if(pos1 != Point(0,0) && pos2 != Point(0,0))
    {
        Size size = Director::getInstance()->getWinSize();
        float scaleX = 4 ;
        float scaleY = 0.4 ;
        float time = 0.2;
        float speed = 0.2;
        switch (level)
        {
            case 1:
            {
                time = 0.1;
                speed = 0.1;
            }
                break;
            case 2:
            {
                time = 0.1;
                speed = 0.2;
            }
                break;
            default:
            {
                time = 0.2;
                speed = 0.3;
            }
                break;
        }
        
        
        auto colorSpriteRight = Sprite::create("colorHRight.png");
        colorSpriteRight->setScaleY(0.8);
        m_level->m_clippingNode->addChild(colorSpriteRight, 10);
        colorSpriteRight->setPosition((pos1+pos2)/2);
        colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                     MoveTo::create(speed, pos2),
                                                     CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
                                                     NULL));
        
        auto colorSpriteLeft = Sprite::create("colorHLeft.png");
        colorSpriteLeft->setScaleY(0.8);
        m_level->m_clippingNode->addChild(colorSpriteLeft, 10);
        colorSpriteLeft->setPosition((pos1+pos2)/2);
        colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                    MoveTo::create(speed, pos1),
                                                    CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
                                                    NULL));
    }
    if(pos3 != Point(0,0) && pos4 != Point(0,0))
    {
        Size size = Director::getInstance()->getWinSize();
        float scaleX = 0.4 ;
        float scaleY = 4 ;
        float time = 0.2;
        float speed = 0.2;
        switch (level)
        {
            case 4:
            {
                time = 0.1;
                speed = 0.2;
            }
                break;
            case 5:
            {
                time = 0.2;
                speed = 0.3;
            }
                break;
            default:
                break;
        }

        
        auto colorSpriteDown = Sprite::create("colorVDown.png");
        colorSpriteDown->setScaleX(0.8);
        m_level->m_clippingNode->addChild(colorSpriteDown, 10);
        colorSpriteDown->setPosition((pos3+pos4)/2);
        colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                     MoveTo::create(speed, pos4),
                                                     CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
                                                     NULL));
        
        auto colorSpriteUp = Sprite::create("colorVUp.png");
        colorSpriteUp->setScaleX(0.8);
        m_level->m_clippingNode->addChild(colorSpriteUp, 10);
        colorSpriteUp->setPosition((pos3+pos4)/2);
        colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
                                                    MoveTo::create(speed, pos3),
                                                    CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
                                                    NULL));
    }
}

