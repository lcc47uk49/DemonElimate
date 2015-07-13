//
//  PlayLayer.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/7/8.
//
//

#include "PlayLayer.h"

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
    m_bgFitNode = nullptr;
    m_srcFruit = nullptr;
    m_dstFruit = nullptr;
    m_touchEnable = false;
    m_isAnimation = true;
    m_prevTime = 0;
    m_aftTime = 0;
    m_basicScore = __BASIC_SCORE;
}

PlayLayer::~PlayLayer()
{
    
}

#pragma mark - 初始化游戏

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

    //开始游戏
    m_level->shuffle();
    m_prevTime = GameTools::getInstance()->getCurrentTime();//获取当前时间
    
    //更新
    scheduleUpdate();
    
    //触摸绑定--单点触摸
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(PlayLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

#pragma mark - 触摸

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
        size_t horizonSize = m_level->detectHorizontalMatches();
        size_t verticalSize = m_level->detectVerticalMatches();//放在if中会被短路
        if (horizonSize != 0 || verticalSize != 0)//如果当前有可消除的
        {
            m_level->removeChians();//获得消除链，更新矩阵
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
            
            animateExplode();//消除
            //消除后清空m_srcFruit和m_dstFruit
            m_srcFruit = nullptr;
            m_dstFruit = nullptr;
            m_level->fillHoles();//获得填充链，更新矩阵
            animateFall();//掉落，并填充
            //掉落后也要更新时间，防止连续掉落很久，给出提示
            m_prevTime = GameTools::getInstance()->getCurrentTime();
            if (m_level->detectPossibleSwaps() == 0)//消除后如果是死地图
            {
                //清空现由果实
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
        if (m_aftTime - m_prevTime >= __TIME_DIFFERENCE_HINT)//提示
        {
            m_level->giveHints();
            m_prevTime = GameTools::getInstance()->getCurrentTime();
        }
    }
}

void PlayLayer::animateSwap(DemonSwap *swap)
{
    m_touchEnable = false;
    m_isAnimation = true;
    m_basicScore = __BASIC_SCORE;//重置基本分数
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
        fruit->setVisible(true);
        Point startPos = fruit->getPosition();
        Point endPos = m_level->getPosOfItem(fruit->getRow(), fruit->getCol());
        float swapDelay = 0.15;//交换动画用时
        float explodDelay = 0.2;//消除用时
        float duration = ((startPos.y - endPos.y)/(2.5*startPos.y)+0.05* fruit->getRow());
        auto easeout = EaseSineIn::create(MoveTo::create(duration, endPos));
        //        fruit->runAction(Sequence::create(DelayTime::create(delay),MoveTo::create(duration, endPos), NULL));
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
        for (; it != chain->m_fruitsChain.end(); it++)
        {
            DemonFruit* fruit = *it;
            Point pos = fruit->getPosition();
            int fruitType = fruit->getFruitType();
            fruit->explode();
            //消除特效根据果实的不同而不同
            animateExplodeEffect(fruitType,pos);
        }
    }
    m_level->m_removeMatches.clear();
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
    addChild(circleSprite, 10);
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
    particleStars->setScale(0.3);
    addChild(particleStars, 20);
}

#pragma mark - callbacks

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