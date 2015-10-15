//
//  SpriteSystem.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/10/12.
//
//

#include "SpriteSystem.h"
#include "OverScene.h"
#include "PlayLayer.h"
#include "../Message.h"
SpriteSystem::SpriteSystem()
{
    
}

SpriteSystem::~SpriteSystem()
{
    
}

bool SpriteSystem::init()
{
    if (! Layer::init())
    {
        return false;
    }
    
    //初始化时间
    m_fBeginElaseTime = 0;
    m_fOverElaspeTime = 0;
    //敌人的攻击时间
    for (uint i = 0; i < 3;  i++)
    {
        m_fAttackElapseTime[i] = 0;
    }
    
    readBSJson(m_bsConf);//读取人物数据
    readMagicJson(m_magConf);//读取技能数据
    initBSandMagic();//初始化人物和技能，并开启update
    setMagicPanel();//添加技能按钮

    return true;
}

void SpriteSystem::readBSJson(_stSpriteConfig (&spConf)[6])
{
    rapidjson::Document doc;
    std::string str = FileUtils::getInstance()->getStringFromFile(s_spConfJson);
    doc.Parse<0>(str.c_str());
    for(unsigned int i = 0; i < 6; i++)
    {
        rapidjson::Value &val = doc[(rapidjson::SizeType)i];
        
        if (val.HasMember("id")&&val.HasMember("name")&&val.HasMember("type")&&val.HasMember("life")&&val.HasMember("attackMax")&&val.HasMember("attackMin")&&val.HasMember("attackSpeed")&&val.HasMember("power")&& val.HasMember("magicGroup"))
        {
            spConf[i].id = (SpriteId)val["id"].GetInt();
            spConf[i].name = val["name"].GetString();
            spConf[i].type = (SpriteType)val["type"].GetInt();
            spConf[i].life = val["life"].GetInt();
            spConf[i].attackMax = val["attackMax"].GetInt();
            spConf[i].attackMin = val["attackMin"].GetInt();
            spConf[i].attackSpeed = val["attackSpeed"].GetDouble();
            spConf[i].power = val["power"].GetInt();
        }
        
        //状态对应的图片数
        rapidjson::Value& num = val["stateNum"][(rapidjson::SizeType)0];
        spConf[i].stateNum.idleNum = num["idleNum"].GetInt();
        spConf[i].stateNum.runNum = num["runNum"].GetInt();
        spConf[i].stateNum.attackNum = num["attackNum"].GetInt();
        spConf[i].stateNum.hurtNum = num["hurtNum"].GetInt();
        spConf[i].stateNum.deadNum = num["deadNum"].GetInt();
        
        //技能
        rapidjson::Value& group = val["magicGroup"][(rapidjson::SizeType)0];
        spConf[i].magicGroup.magic1 = (MagicId)group["magic1"].GetInt();
        spConf[i].magicGroup.magic2 = (MagicId)group["magic2"].GetInt();
    }
}

void SpriteSystem::readMagicJson(_stMagicConfig (&magConf)[9])
{
    rapidjson::Document doc;
    std::string str = FileUtils::getInstance()->getStringFromFile(s_spMagJson);
    doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());
    
    for (unsigned int i = 0; i < 9; i++)
    {
        rapidjson::Value &val =doc[(rapidjson::SizeType)(i)];
        
        if (val.HasMember("id")&&val.HasMember("name")&&val.HasMember("count"))
        {
            magConf[i].id = (MagicId)val["id"].GetInt();
            magConf[i].name = val["name"].GetString();
            magConf[i].count = val["count"].GetInt();
            magConf[i].attack = val["attack"].GetInt();
        }
    }
}


void SpriteSystem::initBSandMagic()
{
    //初始化人物
    struct _stHeroConf
    {
        float spScale[3];//缩放
        Point spPosStart[3];//起始位置
        Point spPosEnd[3];//结束位置
        bool  spFlip[3];//是否翻转，英雄翻转，敌人不翻转，面对面
        
        _stHeroConf()
        {
            spScale[0] = 1.5f; spScale[1] = 1.5f; spScale[2] = 1.5f;
            spFlip[0] = true; spFlip[1] = true; spFlip[2] = true;
            spPosStart[0] = PointS_1; spPosStart[1] = PointS_2; spPosStart[2] = PointS_3;
            spPosEnd[0] = PointE_1; spPosEnd[1] = PointE_2; spPosEnd[2] = PointE_3;
        }
    }_stHeroConf;
    
    struct _stEnemyConf
    {
        float spScale[3];//缩放
        Point spPosStart[3];//起始位置
        Point spPosEnd[3];//结束位置
        bool  spFlip[3];//是否翻转，英雄翻转，敌人不翻转，面对面
        
        _stEnemyConf()
        {
            spScale[0] = 1.5f; spScale[1] = 1.5f; spScale[2] = 1.5f;
            spFlip[0] = false; spFlip[1] = false; spFlip[2] = false;
            spPosStart[0] = PointS_4; spPosStart[1] = PointS_5; spPosStart[2] = PointS_6;
            spPosEnd[0] = PointE_4; spPosEnd[1] = PointE_5; spPosEnd[2] = PointE_6;
        }
    }_stEnemyConf;
    
    //初始化英雄和敌人
    for (unsigned int i = 0; i < 3; i++)
    {
        BaseSprite* hero = BaseSprite::create(m_bsConf[i]);
        hero->setScale(_stHeroConf.spScale[i]);
        hero->setPosition(_stHeroConf.spPosStart[i]);
        hero->setFlippedX(_stHeroConf.spFlip[i]);
        hero->startAction(SPRITESTATE_IDLE);
        hero->move(MoveType_To, _stHeroConf.spPosEnd[i], 4.5f);//执行完有回调
        addChild(hero,1);
        //加入向量
        m_vHeros.pushBack(hero);
        
        BaseSprite* enemy = BaseSprite::create(m_bsConf[i+3]);
        enemy->setScale(_stEnemyConf.spScale[i]);
        enemy->setPosition(_stEnemyConf.spPosStart[i]);
        enemy->setFlippedX(_stEnemyConf.spFlip[i]);
        enemy->startAction(SPRITESTATE_IDLE);
        enemy->move(MoveType_To, _stEnemyConf.spPosEnd[i], 4.5f);//执行完有回调
        addChild(enemy,1);
        //加入向量
        m_vEnemys.pushBack(enemy);
        
        Node* node = Node::create();
        Sprite* lifebg = Sprite::create(s_ptBg);//背景
        m_ptLifeTop[i] = ProgressTimer::create(Sprite::create(s_ptLife2));//进度条
        m_ptLifeTop[i]->setType(ProgressTimer::Type::BAR);//条形
        m_ptLifeTop[i]->setMidpoint(Point::ANCHOR_MIDDLE_LEFT);//(0,0.5)锚点为增长原点
        m_ptLifeTop[i]->setBarChangeRate(Point::ANCHOR_MIDDLE_RIGHT);//向(1,0.5)增长
        m_ptLifeTop[i]->setPercentage(100);
        
        node->setPosition(enemy->getContentSize().width/2,enemy->getContentSize().height*0.8);
        enemy->addChild(node);
        node->addChild(lifebg, 0);
        node->addChild(m_ptLifeTop[i], 1);
        node->setScale(0.8);
        
        //添加敌人生命值
    }
 
    setAttackTarget();//设置攻击目标
    
//    //初始化技能、远程攻击 --》不能这样，因为多个英雄释放同一种技能时，会重复addChild
//    for(int i = 0; i < 5; i++)
//    {
//        Magic* magic = Magic::create(m_magConf[i]);
//        m_vMagics.pushBack(magic);//这里会retain一下
//    }
    scheduleUpdate();
}

void SpriteSystem::setAttackTarget()
{
    //设置初始攻击对象
    for (auto hero : m_vHeros)
    {
        if (hero->getSpriteState() == SPRITESTATE_DEAD) continue;
        for (auto enemy : m_vEnemys)
        {
            if (enemy->getSpriteState() == SPRITESTATE_DEAD) continue;
            hero->setTargetSp(enemy);
            if (hero->getSpConf().id == SPRITEID_2)//设置魔法攻击目标
            {
                for(auto hero2 : m_vHeros)
                {
                    if (hero2->getSpriteState() == SPRITESTATE_DEAD) continue;
                    hero->setMagicTargetSP(hero2);
                    break;
                }
            }
            else
            {
                hero->setMagicTargetSP(enemy);
            }
            break;
        }
    }
    for (auto enemy : m_vEnemys)
    {
        if (enemy->getSpriteState() == SPRITESTATE_DEAD) continue;
        for (auto hero : m_vHeros)
        {
            if (hero->getSpriteState() == SPRITESTATE_DEAD) continue;
            enemy->setTargetSp(hero);
            enemy->setMagicTargetSP(hero);
            break;
        }
    }
}

int SpriteSystem::gameResult()
{
    int deathHero = 0;
    int deathEnemy = 0;
    for (auto hero : m_vHeros)
    {
        if (hero->getSpriteState() == SPRITESTATE_DEAD) deathHero++;
        if (deathHero == m_vHeros.size()) return 2;//失败
    }
    for (auto enemy : m_vEnemys)
    {
        if (enemy->getSpriteState() == SPRITESTATE_DEAD) deathEnemy++;
        if (deathEnemy == m_vEnemys.size()) return 1;//胜利
    }
    return 0;//未完成
}

void SpriteSystem::setMagicPanel()
{
    Sprite* panel = Sprite::create(s_magPan);
    panel->setPosition(380,900);
    this->addChild(panel,1);
    //    panel->setScaleY(0.5);
    
    struct _stMagPan
    {
        float scale[3];
        Point spPos[3];
        bool enable[3];
        Point ptPos[6];
        
        _stMagPan()
        {
            scale[0] = 0.8f; scale[1] = 0.8f; scale[2] = 0.8f;
            spPos[0] = Point(534, 90); spPos[1] = Point(318, 90); spPos[2] = Point(106, 90);
            enable[0] = false; enable[1] = false; enable[2] = false;
            ptPos[0] = Point(106, 30); ptPos[1] = Point(318, 30); ptPos[2] = Point(534, 30);
            ptPos[3] = Point(106, 10); ptPos[4] = Point(318, 10); ptPos[5] = Point(534, 10);
        }
    }_stMagPan;
    
    Sprite* spNor[3];
    Sprite* spDis[3];
    Sprite* ptBg[6];
    
    for (unsigned int i = 0; i < 3; i++)
    {
        //菜单----技能按钮
        spNor[i] = Sprite::create(s_panSpNor[i]);
        spDis[i] = Sprite::create(s_panSpDis[i]);
        
        m_itItem[i] = MenuItemSprite::create(spNor[i], spNor[i], spDis[i], std::bind(&SpriteSystem::itemCallback, this, std::placeholders::_1,i));
        m_itItem[i]->setScale(_stMagPan.scale[i]);
        m_itItem[i]->setPosition(_stMagPan.spPos[i]);
        m_itItem[i]->setEnabled(_stMagPan.enable[i]);
        
        //生命值、能量值
        m_ptLife[i] =  ProgressTimer::create(Sprite::create(s_ptLife));
        m_ptLife[i]->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        m_ptLife[i]->setType(ProgressTimer::Type::BAR);
        m_ptLife[i]->setMidpoint(Point::ANCHOR_MIDDLE_LEFT);
        m_ptLife[i]->setBarChangeRate(Point::ANCHOR_MIDDLE_RIGHT);
        m_ptLife[i]->setPercentage(100);
        
        m_ptPower[i] = ProgressTimer::create(Sprite::create(s_ptPower));
        m_ptPower[i]->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        m_ptPower[i]->setType(ProgressTimer::Type::BAR);
        m_ptPower[i]->setMidpoint(Point::ANCHOR_MIDDLE_LEFT);
        m_ptPower[i]->setBarChangeRate(Point::ANCHOR_MIDDLE_RIGHT);
        m_ptPower[i]->setPercentage(0);
        
    }
    auto menu = Menu::create(m_itItem[0], m_itItem[1], m_itItem[2], NULL);
    menu->setPosition(Point::ZERO);
    panel->addChild(menu,1);
    
    //生命值能量值的背景条
    for (unsigned int i = 0; i < 6; i++)
    {
        ptBg[i] = Sprite::create(s_ptBg2);
        ptBg[i]->setPosition(_stMagPan.ptPos[i]);
        panel->addChild(ptBg[i]);
        
    }
    
    ptBg[0]->addChild(m_ptLife[0],2);
    ptBg[1]->addChild(m_ptLife[1],2);
    ptBg[2]->addChild(m_ptLife[2],2);
    ptBg[3]->addChild(m_ptPower[0],2);
    ptBg[4]->addChild(m_ptPower[1],2);
    ptBg[5]->addChild(m_ptPower[2],2);
}

void SpriteSystem::update(float dt)
{
    m_fBeginElaseTime += dt;//记录游戏开始多久后可以攻击，时间取决于怪物跑动的时间
    
    if (m_fBeginElaseTime >= __RUN_ACTION_TIME &&GameManager::getInstance()->isGameOver == false)//4秒后允许攻击
    {
        enemyAttack(dt);
    }
    
    //显示技能按钮
    for (unsigned int i = 0; i < m_vHeros.size(); i++)
    {
        BaseSprite* hero = m_vHeros.at(i);
        if (hero->getSpriteState() != SPRITESTATE_DEAD)
        {
            this->showLifeButton(hero);
            this->showPowerButton(hero);
        }
    }
    
    //更新敌人生命值
    for (unsigned int i = 0; i < m_vEnemys.size(); i++)
    {
        BaseSprite* enemy = m_vEnemys.at(i);
        if (enemy->getSpriteState() != SPRITESTATE_DEAD)
        {
            showLifeTop(enemy);
        }
    }
    
    switch (gameResult())
    {
        case 0://未完成
            break;
        case 1://胜利
        {
            unscheduleUpdate();
            //给PlayLayer发信号
            Message* msg = Message::instance();
            msg->setMesType(Message_GameSuccess_);
            PlayLayer::s_messageQ->push(msg);
            //人物动画效果
            //添加一个层
            OverScene* overScene = OverScene::create(1);
            this->addChild(overScene,100);
            
        }
            break;
        case 2://失败
        {
            unscheduleUpdate();
            //给PlayLayer发信号
            Message* msg = Message::instance();
            msg->setMesType(Message_GameFailed_);
            PlayLayer::s_messageQ->push(msg);
        
            //人物动画效果
            //添加一个层
            OverScene* overScene = OverScene::create(2);
            this->addChild(overScene,100);
        }
            break;
        default:
            break;
    }
}

void SpriteSystem::enemyAttack(float dt)
{
    for(uint i = 0; i < m_vEnemys.size(); i++)
    {
        BaseSprite* enemy = m_vEnemys.at(i);
        if(enemy->getSpriteState() != SPRITESTATE_DEAD)//如果没有死亡
        {
            m_fAttackElapseTime[i] += dt;//更新时间
            if (m_fAttackElapseTime[i] >= enemy->getSpConf().attackSpeed)//怪物达到攻击时间
            {
                m_fAttackElapseTime[i] = 0;//计时归零
                enemy->startAction(SPRITESTATE_ATTACK);//攻击动作
                //判断弹道
                if (enemy->getSpConf().type != SPRITETYPE_FRONT)//不是前排人物
                {
                    int magicId = enemy->getSpConf().magicGroup.magic1;
//                    Magic* magic = m_vMagics.at(magicId);//这样多个英雄释放同一种技能时会重复addChild。
                    Magic* magic = Magic::create(m_magConf[magicId]);
                    magic->startMagic(enemy, enemy->getTargetSp(), 0.8);
                    magic->setFlippedX(true);//怪物攻击特效翻转
                    this->addChild(magic,2);
                }
                else
                {
                    //随机攻击力
                    int randAttack = enemy->getSpConf().attackMin + rand()%(enemy->getSpConf().attackMax - enemy->getSpConf().attackMin);
                    //更新生命值
                    enemy->getTargetSp()->setCurLife(enemy->getTargetSp()->getCurLife() - randAttack);
                }
                if(enemy->getTargetSp()->getCurLife() <= 0)
                {
                    enemy->getTargetSp()->startAction(SPRITESTATE_DEAD);//没有移除
                    long index = m_vHeros.getIndex(enemy->getTargetSp());
                    m_itItem[index]->setEnabled(false);//禁用技能按钮
                    //改变攻击对象
                    setAttackTarget();
                }
                
                if (enemy->getCurPower() < enemy->getSpConf().power)
                {
                    enemy->setCurPower(enemy->getCurPower() + 1);//怒气+1
                }
                else
                {
//                    enemy->setIsMagic(true);//允许释放魔法
                    //直接释放技能（大招）
                    enemy->startAction(SPRITESTATE_MAGIC);
                }
            }
        }
    }
}

void SpriteSystem::heroAttack(BaseSprite *hero)
{
    if (hero->getSpriteState() == SPRITESTATE_DEAD) return;
    
    hero->startAction(SPRITESTATE_ATTACK);
    //判断弹道
    if (hero->getSpConf().type != SPRITETYPE_FRONT)//不是前排人物
    {
        int magicId = hero->getSpConf().magicGroup.magic1;
        Magic* magic = Magic::create(m_magConf[magicId]);
        magic->startMagic(hero, hero->getTargetSp(), 0.8);
        this->addChild(magic,2);
    }
    else
    {
        //随机攻击力
        int randAttack = hero->getSpConf().attackMin + rand()%(hero->getSpConf().attackMax - hero->getSpConf().attackMin);
        //更新生命值
        hero->getTargetSp()->setCurLife(hero->getTargetSp()->getCurLife() - randAttack);
        //显示敌人生命值
        showLifeTop(hero->getTargetSp());
    }
    if(hero->getTargetSp()->getCurLife() <= 0)
    {
        hero->getTargetSp()->startAction(SPRITESTATE_DEAD);//没有移除
        //改变攻击对象
        setAttackTarget();
    }
}

void SpriteSystem::showLifeTop(BaseSprite *sp)
{
    float per = (((float)sp->getCurLife()/sp->getSpConf().life)*100);
    long index = m_vEnemys.getIndex(sp);
    m_ptLifeTop[index]->setPercentage(per);
}

void SpriteSystem::showLifeButton(BaseSprite *sp)
{
    float per = (((float)sp->getCurLife()/sp->getSpConf().life)*100);
    SpriteId id = sp->getSpConf().id;
    switch (id)
    {
        case SPRITEID_0:
            m_ptLife[2]->setPercentage(per);
            break;
        case SPRITEID_1:
            m_ptLife[1]->setPercentage(per);
            break;
        case SPRITEID_2:
            m_ptLife[0]->setPercentage(per);
            break;
        default:
            break;
    }
}

void SpriteSystem::showPowerButton(BaseSprite *sprite)
{
    float per = (((float)sprite->getCurPower()/sprite->getSpConf().power)*100);//在playlayer中每消除一个果实+1
    SpriteId id = sprite->getSpConf().id;
    switch (id)
    {
        case SPRITEID_0:
            m_ptPower[2]->setPercentage(per);
            if (per >= 100)
            {
                m_itItem[0]->setEnabled(true);
            }
            break;
        case SPRITEID_1:
            m_ptPower[1]->setPercentage(per);
            if (per >= 100)
            {
                m_itItem[1]->setEnabled(true);
            }
            break;
        case SPRITEID_2:
            m_ptPower[0]->setPercentage(per);
            if (per >= 100)
            {
                m_itItem[2]->setEnabled(true);
            }
            break;
        default:
            break;
    }
}

void SpriteSystem::itemCallback(Ref* pSender, int Num)
{
    m_itItem[Num]->setEnabled(false);
    CCLOG("Num%d", Num);
    BaseSprite* hero = m_vHeros.at(Num);
    hero->setCurPower(0);
    hero->startAction(SPRITESTATE_MAGIC);
    //释放大招
    Magic* bigMagic = Magic::create(m_magConf[hero->getSpConf().magicGroup.magic2]);
    bigMagic->startMagic(hero, hero->getMagicTargetSP(), 0);
    this->addChild(bigMagic,2);
    //    this->startMagic(m_bsSp[Num], m_bsSp[Num]->getSpConf().magicGroup.magic2, 3);
}