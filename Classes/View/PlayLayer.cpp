//
//  PlayLayer.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/7/8.
//
//

#include "PlayLayer.h"

MessageQueue* PlayLayer::s_messageQ = new MessageQueue();



Scene* PlayLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = PlayLayer::create();
    scene->addChild(layer);
    
    return scene;
}

PlayLayer::PlayLayer()
{
    m_level = nullptr;
    m_spriteSystem = nullptr;
    m_bgFitNode = nullptr;
    m_srcFruit = nullptr;
    m_dstFruit = nullptr;
    m_touchEnable = false;
    m_isAnimation = true;
    m_prevTime = 0;
    m_aftTime = 0;
    m_basicScore = __BASIC_SCORE;
    m_comboNum = 0;
    m_elimateFruitType.clear();
    m_comboLabel = nullptr;
    m_skillElimateCount = 0;
//    m_isSkillElimate = false;
}

PlayLayer::~PlayLayer()
{
    
}

#pragma mark - Init Game

bool PlayLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    //随机种子
    srand((unsigned)time(NULL));
    //适配
    Size winSize = Director::getInstance()->getWinSize();
    m_bgFitNode = Node::create();
    m_bgFitNode->setContentSize(Size(__DEFAULT_WINWIDTH,__DEFAULT_WINHEIGHT));
    GameTools::getInstance()->scaleFitOutScreen(m_bgFitNode);
    this->addChild(m_bgFitNode,0);//底层
    
    //添加背景
    Sprite* spBackground = Sprite::create("background.png");
    m_bgFitNode->addChild(spBackground);
    spBackground->setPosition(Point(m_bgFitNode->getContentSize().width/2,m_bgFitNode->getContentSize().height/2));
    
   
    
    //创建关卡并适配 --锚点选为底边中心点，将此节点贴着底边放置
    m_level = DemonLevel::create("Level_0.json");
    m_level->setContentSize(Size(__DEFAULT_WINWIDTH,__DEFAULT_WINHEIGHT));
    GameTools::getInstance()->scaleFitInScreen(m_level);
    m_level->setPosition(Point(winSize.width/2,0));
    m_level->setAnchorPoint(Point(0.5,0));
    this->addChild(m_level,1);

    m_comboLabel = Label::createWithBMFont("font2.fnt", "");
    m_comboLabel->setScale(1.5);
    m_level->addChild(m_comboLabel);
    m_comboLabel->setPosition(m_level->getContentSize().width*0.8,m_level->getContentSize().height*0.7);
    
    
    //初始化DemonSkill中的m_level
    DemonSkill::getInstance()->initWithLevel(m_level);
    
    //开始游戏
    m_level->shuffle();
    this->scheduleOnce(schedule_selector(PlayLayer::callGameBegin), 4);
    this->schedule(schedule_selector(PlayLayer::handleMsg));//监听消息
    //倒计时动画
//    animateCountDown();
    
    m_spriteSystem = SpriteSystem::create();
    m_level->addChild(m_spriteSystem,1);
    
    //触摸绑定--单点触摸
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(PlayLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

#pragma mark - Touch

bool PlayLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    //每次触摸开始、结束或者移动都会更新m_prevTime
    m_prevTime = GameTools::getInstance()->getCurrentTime();
    m_srcFruit = nullptr;
    m_dstFruit = nullptr;
    if (m_touchEnable)
    {
        Point pos = m_level->convertTouchToNodeSpace(touch);
        m_srcFruit = m_level->getFruitofPos(pos);
        //高亮
    }
    return m_touchEnable;
}

void PlayLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    //每次触摸开始、结束或者移动都会更新m_prevTime
    m_prevTime = GameTools::getInstance()->getCurrentTime();
    if(!m_touchEnable || !m_srcFruit)
        return;
    
    int row = m_srcFruit->getRow();
    int col = m_srcFruit->getCol();
//    CCLOG("m_srcFruit.row = %d, col = %d, fruitType = %d,refCount = %d",row,col,m_srcFruit->getFruitType(),m_srcFruit->getReferenceCount());
    auto halfFruitWidth = m_level->m_fruitSize.width/2;
    auto halfFruitHeight = m_level->m_fruitSize.height/2;
    auto localPos = m_level->convertTouchToNodeSpace(touch);
    
    //上方区域
    auto upRect = Rect(m_srcFruit->getPositionX() - halfFruitWidth,
                       m_srcFruit->getPositionY() + halfFruitHeight + __FRUIT_GAP,
                       m_level->m_fruitSize.width,
                       m_level->m_fruitSize.height);
    if (upRect.containsPoint(localPos))
    {
        row++;
        if (row < __FRUIT_MATRIX_HEIGHT)
        {
            m_dstFruit = m_level->getFruitofRC(row, col);
        }
        if (m_dstFruit != nullptr)
        {
            DemonSwap* swap = DemonSwap::create(m_srcFruit, m_dstFruit);
            swap->retain();//引用+1，用完了在释放
            //这里swap没有addchild也没有被释放，是否因为其中的成员没有被释放？
            if(m_level->isPossibleSwap(swap))//判断是否为有效交换
            {
                m_level->performSwap(swap);//交换，并更新矩阵
                animateSwap(swap);//有效交换效果
            }
            else
            {
                animateInvalidSwap(swap);//无效交换效果
            }
        }
        return;
    }
    
        //下方区域
    auto downRect = Rect(m_srcFruit->getPositionX() - halfFruitWidth,
                         m_srcFruit->getPositionY() - halfFruitHeight*3 - __FRUIT_GAP,
                         m_level->m_fruitSize.width,
                         m_level->m_fruitSize.height);
    if (downRect.containsPoint(localPos))
    {
        row--;
        if (row >= 0)
        {
            m_dstFruit = m_level->getFruitofRC(row, col);
        }
        if (m_dstFruit != nullptr)
        {
            DemonSwap* swap = DemonSwap::create(m_srcFruit, m_dstFruit);
            swap->retain();//引用+1，用完了在释放
            if(m_level->isPossibleSwap(swap))//判断是否为有效交换
            {
                m_level->performSwap(swap);//交换
                animateSwap(swap);//有效交换效果
            }
            else
            {
                animateInvalidSwap(swap);//无效交换效果
            }
        }
        return;
    }
    
    //左方区域
    auto leftRect = Rect(m_srcFruit->getPositionX() - halfFruitWidth*3 - __FRUIT_GAP,
                         m_srcFruit->getPositionY() - halfFruitHeight,
                         m_level->m_fruitSize.width,
                         m_level->m_fruitSize.height);
    if (leftRect.containsPoint(localPos))
    {
        col--;
        if (col >= 0)
        {
            m_dstFruit = m_level->getFruitofRC(row, col);
        }
        if (m_dstFruit != nullptr)
        {
            DemonSwap* swap = DemonSwap::create(m_srcFruit, m_dstFruit);
            swap->retain();//引用+1，用完了在释放
            if(m_level->isPossibleSwap(swap))//判断是否为有效交换
            {
                m_level->performSwap(swap);//交换
                animateSwap(swap);//有效交换效果
            }
            else
            {
                animateInvalidSwap(swap);//无效交换效果
            }
        }
        return;
    }
    
    //右方区域
    auto rightRect = Rect(m_srcFruit->getPositionX() + halfFruitWidth + __FRUIT_GAP,
                          m_srcFruit->getPositionY() - halfFruitHeight,
                          m_level->m_fruitSize.width,
                          m_level->m_fruitSize.height);
    if (rightRect.containsPoint(localPos))
    {
        col++;
        if (col < __FRUIT_MATRIX_WIDTH)
        {
            m_dstFruit = m_level->getFruitofRC(row, col);
        }
        if (m_dstFruit != nullptr)
        {
            DemonSwap* swap = DemonSwap::create(m_srcFruit, m_dstFruit);
            swap->retain();//引用+1，用完了在释放
            if(m_level->isPossibleSwap(swap))//判断是否为有效交换
            {
                m_level->performSwap(swap);//交换
                animateSwap(swap);//有效交换效果
            }
            else
            {
                animateInvalidSwap(swap);//无效交换效果
            }
        }
        return;
    }

}

void PlayLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    //每次触摸开始、结束或者移动都会更新m_prevTime
    m_prevTime = GameTools::getInstance()->getCurrentTime();
}

#pragma mark - 定时器

void PlayLayer::update(float dt)
{
    //check if animationing
    if(m_isAnimation)
    {
        //初始化为false
        m_isAnimation = false;
        //如果有果实有动画效果则置为true
        for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
        {
            //标记，用于跳出两个循环
            int a = 0;
            for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
            {
                DemonFruit* fruit = m_level->getFruitofRC(row, col);
                if (fruit != nullptr && (fruit->getNumberOfRunningActions() > 0))
                {
                    m_isAnimation = true;
                    a = 1;
                    break;
                }
            }
            if (a == 1)
            {
                break;
            }
        }
    }
    m_touchEnable = !m_isAnimation;//有动画则不能触摸
    if (!m_isAnimation)//没有动画了 则进行消除
    {
        m_aftTime = GameTools::getInstance()->getCurrentTime();
        //获得消除链
        m_level->detectHorizontalMatches();
        m_level->detectVerticalMatches();//放在if中会被短路
        if (m_level->m_removeMatches.size() != 0)//如果当前有可消除(非技能)的
        {
            m_level->removeChians();//根据消除链更新矩阵（在矩阵中将要移除的置为null）
            calcuteScore();//计算分值
            animateExplode();//消除并清空消除链
            //消除后清空m_srcFruit和m_dstFruit
            m_srcFruit = nullptr;
            m_dstFruit = nullptr;
            m_level->fillHoles();//获得填充链，更新矩阵
            animateFall();//掉落，并填充
            //掉落后也要更新时间，防止连续掉落很久，给出提示
            m_prevTime = GameTools::getInstance()->getCurrentTime();
            if (m_level->detectPossibleSwaps() == 0)//消除后如果是死地图
            {
                //清空现有果实
                for (int row = 0; row < __FRUIT_MATRIX_HEIGHT; row++)
                {
                    for (int col = 0; col < __FRUIT_MATRIX_WIDTH; col++)
                    {
                        if (m_level->m_fruitMatrix[row][col] != nullptr)
                        {
                            m_level->m_fruitMatrix[row][col]->removeFromParent();
                        }
                    }
                }
                m_level->shuffle();//重新生成
            }
        }
        if (m_aftTime - m_prevTime >= __TIME_DIFFERENCE_COMBOS)//重置combo
        {
            m_comboNum = 0;
            m_comboLabel->setString("");
        }
        if (m_aftTime - m_prevTime >= __TIME_DIFFERENCE_HINT)//提示
        {
            m_level->giveHints();
            m_prevTime = GameTools::getInstance()->getCurrentTime();
        }
    }
//    //碰撞检测
//    collideWithTree();
//    collideWithEnemy();
//    //检查游戏是否结束
//    if (m_level->m_tree->getHP() <= 1)
//    {
//        CCLOG("GameOver!!");
//        for (auto obj : m_enemiesInLevel)
//        {
//            DemonEnemy* enemy = dynamic_cast<DemonEnemy*>(obj);
//            if (enemy != nullptr)
//            {
//                enemy->setState(3);
//            }
//        }
//        m_touchEnable = false;
//        this->unschedule(schedule_selector(PlayLayer::addEnemyTimer));
//        unscheduleUpdate();
//    }
}

void PlayLayer::addEnemyTimer(float dt)
{
////    CCLOG("add enemy!!");
//    //1.取出向量中的第一个
//    DemonEnemy* enemy = m_level->m_enemies.front();
//    if(enemy == nullptr) return;
//    //2.添加到m_level中并记录已经添加的敌人
//    m_level->addChild(enemy,10);
//    m_enemiesInLevel.pushBack(enemy);// 加入 “在m_level中的敌人” 的向量
//    enemy->setPosition(Point(m_level->getContentSize().width,m_level->getContentSize().height*0.9));
//    //3.从m_enemies中移除该敌人
//    m_level->m_enemies.erase(m_level->m_enemies.begin());
////    CCLOG("m_enemies.size = %ld",m_level->m_enemies.size());
}

#pragma mark - Functions

void PlayLayer::calcuteScore()
{
    //计算分值
    for (auto chain : m_level->m_removeMatches)//每一组消除组合判断分值
    {
        long score = m_level->giveScore(m_basicScore, chain->getSize());
        //取中间坐标
        DemonFruit* firstFruit = chain->getFirstFruit();
        DemonFruit* lastFruit = chain->getLastFruit();
        
        Point centerPos = Point(0.5*(firstFruit->getPositionX()+lastFruit->getPositionX()),0.5*(firstFruit->getPositionY()+lastFruit->getPositionY()));
        
        animateScore(score,centerPos);//得分动画
        m_basicScore *= 2;//如果连消，则每次基本分值增加一倍
    }
}

void PlayLayer::giveSkill()
{
    long size = m_elimateFruitType.size();
    int fruitType1, fruitType2, fruitType3;
    if (size >= 3)
    {
        fruitType1 = m_elimateFruitType.back();//最后一个果实类型
        fruitType2 = m_elimateFruitType.at(m_elimateFruitType.size()-2);//倒数第二个果实类型
        fruitType3 = m_elimateFruitType.at(m_elimateFruitType.size()-3);//倒数第三个果实类型
    }
    else if(size == 2)
    {
        fruitType1 = m_elimateFruitType.back();//最后一个果实类型
        fruitType2 = m_elimateFruitType.at(m_elimateFruitType.size()-2);//倒数第二个果实类型
        fruitType3 = -1;//倒数第三个为空
    }
    else if(size == 1)
    {
        fruitType1 = m_elimateFruitType.back();//最后一个果实类型
        fruitType2 = -1;
        fruitType3 = -1;//两个为空
    }
    
    if(fruitType1 == fruitType2 && fruitType2 == fruitType3)//释放大招
    {
        //判断技能是否已经解锁
        int bigSkillType = fruitType1 + __SKILL_DIFFERENCE;
        bool isUnlock = GameManager::getInstance()->isSkillUnlock(bigSkillType);
        if (isUnlock)//解锁了则释放大招
        {
            CCLOG("Big Skill %d",bigSkillType);
            m_elimateFruitType.clear();//释放完大招，清空消除类型链
        }
        else
        {
//            CCLOG("Big Skill %d is locked!",bigSkillType);
            m_elimateFruitType.clear();
            m_elimateFruitType.push_back(fruitType1);//清空消除链，并且记录最后一个消除的果实的类型
        }
        
    }
    else if (fruitType1 == fruitType2)
    {
        bool isUnlock = GameManager::getInstance()->isSkillUnlock(fruitType1);
        if (isUnlock)
        {
            CCLOG("Skill %d",fruitType1);//释放小技能，不用消除链
            animateSkill(fruitType1);
        }
    }
    else if(fruitType1 != fruitType2)
    {
        //清空消除链，并且记录最后一个果实类型
        m_elimateFruitType.clear();
        m_elimateFruitType.push_back(fruitType1);
    }
}

void PlayLayer::collideWithTree()
{
//    Rect rectOfTree = m_level->m_tree->boundingBox();
//    
//    for (auto obj : m_enemiesInLevel)
//    {
//        DemonEnemy* enemy = dynamic_cast<DemonEnemy*>(obj);
//        if(enemy->boundingBox().intersectsRect(rectOfTree))
//        {
//            enemy->setState(2);//更改状态为攻击
//            int aggressivity = enemy->getAggressivity();//获取攻击力
////            enemy->attack();//攻击后消失
//            enemy->dead();
//            m_level->m_tree->minusHP(aggressivity);//大树掉血
//            m_enemiesInLevel.eraseObject(enemy);//从m_enemiesInLevel中移除
//        }
//    }
////    CCLOG("m_enemiesInLevel.size = %ld",m_enemiesInLevel.size());
}

void PlayLayer::collideWithEnemy()
{
//    Vector<Sprite*> vecBullet = m_level->m_bulletLayer->GetBullet();
//    for (int i = 0; i < vecBullet.size(); i++)
//    {
//        Sprite* sp = vecBullet.at(i);
//        //将子弹坐标转换到m_level下
//        Point pos = m_level->convertToNodeSpace(sp->getPosition());
//        for (int j = 0; j < m_enemiesInLevel.size(); j++)
//        {
//            DemonEnemy* enemy = m_enemiesInLevel.at(j);
//            if (enemy->boundingBox().containsPoint(pos))
//            {
//                //碰撞
//                m_level->m_bulletLayer->removeBullet(sp);
////                enemy->dead();
////                m_enemiesInLevel.eraseObject(enemy);
//            }
//        }
//    }
}

#pragma mark - Animations

void PlayLayer::animateSwap(DemonSwap *swap)
{
    m_touchEnable = false;
    m_isAnimation = true;
    m_basicScore = __BASIC_SCORE;//重置基本分数
    m_comboNum = 0;//重置连击数
    m_comboLabel->setString("");//将标签内容置为空
    //玩家先选择的放在上面
    swap->m_fruitA->setLocalZOrder(2);
    swap->m_fruitB->setLocalZOrder(1);
    float time = 0.2;
    Point posA = swap->m_fruitA->getPosition();
    Point posB = swap->m_fruitB->getPosition();
    

    swap->m_fruitA->runAction(MoveTo::create(time, posB));
    
    auto callRelease = CallFunc::create(CC_CALLBACK_0(PlayLayer::callNActionEndRelease, this,swap));
    auto seq = Sequence::create(MoveTo::create(time, posA), callRelease,NULL);//B动作结束释放，A不调用，防止重复释放
    swap->m_fruitB->runAction(seq);
    //音效
    
}

void PlayLayer::animateInvalidSwap(DemonSwap *swap)
{
    m_touchEnable = false;
    m_isAnimation = true;
    m_basicScore = __BASIC_SCORE;//重置基本分数
    m_comboNum = 0;//重置连击数
    m_comboLabel->setString("");//将标签内容置为空
    //玩家先选择的在上面
    swap->m_fruitA->setLocalZOrder(2);
    swap->m_fruitB->setLocalZOrder(1);
    float time1 = 0.2;
    float time2 = 0.2;
    Point posA = swap->m_fruitA->getPosition();
    Point posAA = m_level->getPosOfItem(swap->m_fruitA->getRow(), swap->m_fruitA->getCol());
//    CCLOG("posA = (%f,%f), posAA = (%f,%f)",posA.x,posA.y,posAA.x,posAA.y);
    Point posB = swap->m_fruitB->getPosition();
    
    auto callA = CallFunc::create(CC_CALLBACK_0(PlayLayer::callNDBackSetZOrder, this,swap->m_fruitA, 1));//换回来的时候玩家选择的在下面
    
    Sequence* seqA = Sequence::create(MoveTo::create(time1, posB),callA,MoveTo::create(time2, posA),NULL);
    swap->m_fruitA->runAction(seqA);
    
    
    
    auto callB = CallFunc::create(CC_CALLBACK_0(PlayLayer::callNDBackSetZOrder, this,swap->m_fruitB, 2));
    
    auto callRelease = CallFunc::create(CC_CALLBACK_0(PlayLayer::callNActionEndRelease, this,swap));
    Sequence* seqB = Sequence::create(MoveTo::create(time1, posA),callB,MoveTo::create(time2, posB),callRelease,NULL);//B动作结束调用，A不调用防止重复释放
    
    swap->m_fruitB->runAction(seqB);
    //音效
}

void PlayLayer::animateFall()
{
    m_touchEnable = false;
    m_isAnimation = true;
    
    Size size = m_level->getContentSize();
    for (auto fruit : m_level->m_fallFruits)//掉落已存在的
    {
        //动画
        
        
        Point startPos = fruit->getPosition();
        Point endPos = m_level->getPosOfItem(fruit->getRow(), fruit->getCol());
        float swapDelay = 0.15;//交换动画用时
        float duration = ((startPos.y - endPos.y)/(2.5*startPos.y)+0.05* fruit->getRow());
        auto easeout = EaseSineIn::create(MoveTo::create(duration, endPos));
        fruit->runAction(Sequence::create(DelayTime::create(swapDelay),easeout, NULL));
    }
    m_level->m_fallFruits.clear();
}

void PlayLayer::animateExplode()
{
    m_touchEnable = false;
    m_isAnimation = true;
    for (auto chain : m_level->m_removeMatches)
    {
        vector<DemonFruit*>::iterator it = chain->m_fruitsChain.begin();
        int i = 0;//标记，每个链只执行一次
        for (; it != chain->m_fruitsChain.end(); it++)
        {
            DemonFruit* fruit = *it;
            if(fruit != nullptr)
            {
                int row = fruit->getRow();
                int col = fruit->getCol();//获得要消除的果实的行列
                
                if (m_level->m_iMatrix[row][col] == -2)
                {
                    //留给技能消除，这里不消除
                    continue;
                }
//                CCLOG("Elimate [%d,%d]",row,col);
                Point pos = fruit->getPosition();
                int fruitType = fruit->getFruitType();
                //消除增加能量
                if(fruitType < 3)
                {
                    int var = m_spriteSystem->m_vHeros.at(fruitType)->getCurPower();
                    m_spriteSystem->m_vHeros.at(fruitType)->setCurPower(var+1);
                    if (i == 0)//攻击动作,每个链只执行一次
                    {
                        m_spriteSystem->heroAttack(m_spriteSystem->m_vHeros.at(fruitType));
                    }
                }
                
                
                fruit->explode();
                //消除特效根据果实的不同而不同
//                animateExplodeEffect(fruitType,pos);
                if (i == 0)
                {
                    m_comboNum++;//每次消除连击数+1，但是每次交换会重置为0
                    m_elimateFruitType.push_back(fruitType);//记录消除的果实的种类
//                    CCLOG("cuurrent FruitType is %d",fruitType);
                    //判断是否需要释放技能，释放什么技能
                    giveSkill();
                    if(m_comboNum >= 2)//两连击以上
                    {
                        animateCombos();//连击效果
                    }
                }
                i++;
            }
            
        }
    }
    m_level->m_removeMatches.clear();
}

void PlayLayer::animateSkillExplode(Vector<DemonChain*>& chains)
{
    m_touchEnable = false;
    m_isAnimation = true;
    for (auto chain : chains)
    {
        vector<DemonFruit*>::iterator it = chain->m_fruitsChain.begin();
        for (; it != chain->m_fruitsChain.end(); it++)
        {
            DemonFruit* fruit = *it;
            if (fruit != nullptr)
            {
                Point pos = fruit->getPosition();
                int fruitType = fruit->getFruitType();
                CCLOG("skillElimate [%d,%d]",fruit->getRow(),fruit->getCol());
                fruit->explode();
                //消除特效根据果实的不同而不同
                animateExplodeEffect(fruitType,pos);
                m_skillElimateCount++;//技能消除计数+1
                if(m_skillElimateCount >= 100)
                {
                    m_skillElimateCount = 100;
                    //发送信号，可以释放必杀
                }
            }
        }
        //每一个技能消除链进行记分
        m_comboNum++;
        if(m_comboNum >= 2)//两连击以上
        {
            animateCombos();//连击效果
        }
    }
    chains.clear();
}

void PlayLayer::animateScore(long score,Point pos)
{
    char strScore[1024];
    sprintf(strScore, "%ld",score);
//    CCLOG("score = %s", strScore);
    auto label = LabelTTF::create(strScore, "Arial", 50);
    m_level->addChild(label);
    label->setPosition(pos);
    label->setColor(Color3B(255,0,0));
    auto call = CallFunc::create(CC_CALLBACK_0(PlayLayer::callNRemoveScoreLabel, this,label));
    float time = 1.5;
    Point endPos = Point(pos.x,pos.y + m_level->m_fruitSize.height);
    auto spawn = Spawn::create(FadeOut::create(time),MoveTo::create(time, endPos), NULL);
    auto seq = Sequence::create(EaseOut::create(spawn,time),call, NULL);
    label->runAction(seq);
    
}

void PlayLayer::animateExplodeEffect(int fruitType, Point pos)
{
    float time = 0.4;
    // 2. action for circle
    auto circleSprite = Sprite::create("circle.png");
    m_level->addChild(circleSprite, 10);
    circleSprite->setPosition(pos);
    circleSprite->setScale(0);// start size
    circleSprite->runAction(Sequence::create(ScaleTo::create(time, 1.0),
                                             CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
                                             NULL));
    
    // 3. particle effect
    auto particleStars = ParticleSystemQuad::create("stars.plist");
    particleStars->setAutoRemoveOnFinish(true);
    particleStars->setBlendAdditive(false);
    particleStars->setPosition(pos);
    particleStars->setScale(0.8);
    m_level->addChild(particleStars, 20);
}

void PlayLayer::animateCombos()
{
//    CCLOG("m_comboNum = %d",m_comboNum);
    char buf[1024];
    sprintf(buf, "%d Combos!",m_comboNum);
    m_comboLabel->setString(buf);
    int clolrBlue = 255 - (m_comboNum-2)*15;//颜色随着连击数从黄变红
    if (clolrBlue < 0)
    {
        clolrBlue = 0;
    }
    m_comboLabel->setColor(Color3B(255, clolrBlue, 0));
    if (m_comboLabel->getNumberOfRunningActions() == 0)
    {
        m_comboLabel->runAction(Sequence::create(ScaleTo::create(0.1, 1.7),ScaleTo::create(0.1, 1.5), NULL));
    }
}

void PlayLayer::animateSkill(int fruitType)
{
    GameManager::getInstance()->setSkillLevel(fruitType, 5);
    
    switch (fruitType)
    {
        case 0://治愈果实技能效果
        {
            //技能效果
//            DemonSkill::getInstance()->treatSkill();
        }
            break;
        case 1://金币果实技能效果
        {
            //技能效果
//            DemonSkill::getInstance()->goldSkill();
        }
            break;
        case 2://攻击力提升果实技能效果
        {
            //技能效果
//            DemonSkill::getInstance()->powerSkill();
        }
            break;
        case 3://火焰果实技能效果
        {
            //技能效果
            DemonChain* chain = DemonChain::create();
            chain->m_chainType = ChainTypeFire;
            DemonSkill::getInstance()->fireSkill(chain);
            
            //消除效果
            Vector<DemonChain*> fireChain;
            fireChain.pushBack(chain);
            animateSkillExplode(fireChain);
        }
            break;
        case 4://冰霜果实技能效果
        {
            //技能效果
            DemonChain* chain = DemonChain::create();
            chain->m_chainType = ChainTypeIce;
            DemonSkill::getInstance()->iceSkill(chain);
            
            //消除效果
            Vector<DemonChain*> iceChain;
            iceChain.pushBack(chain);
            animateSkillExplode(iceChain);
        }
            break;
        case 5://炫光果实技能效果
        {
            //技能效果
            DemonChain* chain = DemonChain::create();
            chain->m_chainType = ChainTypeLight;
            DemonSkill::getInstance()->lightSkill(chain);
            //消除效果
            Vector<DemonChain*> lightChain;
            lightChain.pushBack(chain);
            animateSkillExplode(lightChain);
        }
            break;
        default:
            break;
    }
}

//倒计时动画
void PlayLayer::animateCountDown()
{
//    //开始倒计时动画效果
//    auto countGo = Sprite::createWithSpriteFrameName("countdown_13.png");
//    m_level->addChild(countGo,9);
//    countGo->setAnchorPoint(Point(0.5,1));
//    countGo->setPosition(Point(m_level->getContentSize().width* 0.5,m_level->getContentSize().height));
//    
//    auto countDown = Sprite::createWithSpriteFrameName("countdown_11.png");
//    m_level->addChild(countDown,10);
//    countDown->setAnchorPoint(Point(0.5,1));
//    countDown->setPosition(Point(m_level->getContentSize().width* 0.5,m_level->getContentSize().height));
//    auto countNum = Sprite::createWithSpriteFrameName("countdown_01.png");
//    countDown->addChild(countNum);
//    countNum->setPosition(Point(countDown->getContentSize().width/2,countDown->getContentSize().height/2));
//    auto countCircle = Sprite::createWithSpriteFrameName("countdown_12.png");
//    countCircle->setPosition(Point(countDown->getContentSize().width/2,countDown->getContentSize().height/2));
//    countDown->addChild(countCircle);
//    
//    Vector<SpriteFrame*> arr;
//    char buf[1024] = {0};
//    for (int i = 1; i <= 3; i++)
//    {
//        sprintf(buf, "countdown_%02d.png",i);
//        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf);
//        arr.pushBack(frame);
//    }
//    auto animation = Animation::createWithSpriteFrames(arr,1);
//    countNum->runAction(Sequence::create(Animate::create(animation),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, countDown)),nullptr));
//    countCircle->runAction(Sequence::create(RotateTo::create(3, -1080),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, countCircle)),nullptr));
//    
//    auto callBegin = CallFunc::create(CC_CALLBACK_0(PlayLayer::callGameBegin, this));//游戏开始允许触摸，添加怪物，在Go消失之后
//    countGo->runAction(Sequence::create(DelayTime::create(4),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, countGo)),callBegin,nullptr));
}

#pragma mark - Callbacks

void PlayLayer::callNDBackSetZOrder(Sprite* sp,int zOrder)
{
    sp->setLocalZOrder(zOrder);
}

void PlayLayer::callNActionEndRelease(Node* node)
{
    node->release();
}

void PlayLayer::callNRemoveScoreLabel(Node* node)
{
    if (node != nullptr)
    {
        node->removeFromParent();
        node = nullptr;
    }
}

//游戏开始
void PlayLayer::callGameBegin(float dt)
{
    //开始计时
    m_prevTime = GameTools::getInstance()->getCurrentTime();//获取当前时间
    //更新--允许触摸
    scheduleUpdate();
}

#pragma mark - 消息处理
void PlayLayer::handleMsg(float dt)
{
    if(PlayLayer::s_messageQ->size() != 0)
    {
        Message* m = NULL;
        while ((m = PlayLayer::s_messageQ->pop()) != NULL)
        {
            switch (m->getMesType())
            {
                case Message_GameSuccess_:
                    unscheduleUpdate();
                    break;
                case Message_GameFailed_:
                    unscheduleUpdate();
                    break;
            }
        }
    }
}