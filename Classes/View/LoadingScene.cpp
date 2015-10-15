//
//  LoadingScene.cpp
//  EG
//
//  Created by 凯 于 on 15/10/1.
//
//

#include "LoadingScene.h"
#include "GameTools.h"
#include "PlayLayer.h"
#include "MapScene.h"
LoadingScene::LoadingScene()
{
    m_iNumOfLoadedRes = 0;
    m_iNumOfRes = 42;
}

LoadingScene::~LoadingScene()
{
    
}

Scene* LoadingScene::createScene()
{
    Scene* scene = Scene::create();
    LoadingScene* layer = LoadingScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool LoadingScene::init()
{
    if (! Layer::init())
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    //背景适配节点
    m_nBgFitNode = Node::create();
    m_nBgFitNode->setContentSize(Size(__DEFAULT_WINWIDTH,__DEFAULT_WINHEIGHT));
    m_nBgFitNode->setAnchorPoint(Point(0.5,0.5));
    m_nBgFitNode->setPosition(winSize.width/2,winSize.height/2);
    GameTools::getInstance()->scaleFitOutScreen(m_nBgFitNode);
    addChild(m_nBgFitNode,0);
    
    //中心适配节点
    m_nCtFitNode = Node::create();
    m_nCtFitNode->setContentSize(Size(__DEFAULT_WINWIDTH,__DEFAULT_WINHEIGHT));
    m_nCtFitNode->setAnchorPoint(Point(0.5,0.5));
    m_nCtFitNode->setPosition(winSize.width/2,winSize.height/2);
    GameTools::getInstance()->scaleFitInScreen(m_nCtFitNode);
    addChild(m_nCtFitNode,1);
    
    //加载资源
    loadRes();
    //初始化背景和标签等
    initBg();
    return true;
}

void LoadingScene::loadRes()
{
    
    //标签
    m_labelPercent = Label::createWithTTF("", "Marker Felt.ttf", 40);
    m_labelPercent->setPosition(m_nCtFitNode->getContentSize().width/2,m_nCtFitNode->getContentSize().height/3 + 60);
    m_nCtFitNode->addChild(m_labelPercent);
    
    //异步加载plist资源
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GameTools::getName("LoadingPlist"), GameTools::getName("LoadingPng"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GameTools::getName("DFPlist"), GameTools::getName("DFPng"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GameTools::getName("Item2Plist"), GameTools::getName("Item2Png"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GameTools::getName("BulletPlist"), GameTools::getName("BulletPng"));
    
    
    for (int i = 0; i < 8; i++)
    {
        char buf1[1024];
        char buf2[1024];
        sprintf(buf1, "Explode%dPlist",i+1);
        sprintf(buf2, "Explode%dPng",i+1);
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GameTools::getName(buf1), GameTools::getName(buf2));
        Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName(buf2), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    }
    
    for (int i = 0; i < 8; i++)
    {
        char buf1[1024];
        char buf2[1024];
        sprintf(buf1, "Monster%dPlist",i+1);
        sprintf(buf2, "Monster%dPng",i+1);
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GameTools::getName(buf1), GameTools::getName(buf2));
        Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName(buf2), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    }
    
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GameTools::getName("StarPlist"), GameTools::getName("StarPng"));//粒子系统的不用加载
    
    //异步加载图片资源
    Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName("LoadingSceneBg"), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName("LoadingPng"), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName("LoadingPng"), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName("DFPng"), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName("BulletPng"), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName("Item2Png"), CC_CALLBACK_1(LoadingScene::loadCallback, this));
   
    for (int i = 0; i < 3; i++)
    {
        char buf1[1024];
        char buf2[1024];
        sprintf(buf1, "sprite%d.png",i+1);
        sprintf(buf2, "sprite%dno.png",i+1);
        Director::getInstance()->getTextureCache()->addImageAsync(buf1, CC_CALLBACK_1(LoadingScene::loadCallback, this));
        Director::getInstance()->getTextureCache()->addImageAsync(buf2, CC_CALLBACK_1(LoadingScene::loadCallback, this));
    }
    
    Director::getInstance()->getTextureCache()->addImageAsync("life.png", CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync("lifebg.png", CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync("lifebg2.png", CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync("panel.png", CC_CALLBACK_1(LoadingScene::loadCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync("power.png", CC_CALLBACK_1(LoadingScene::loadCallback, this));
    
    Director::getInstance()->getTextureCache()->addImageAsync("block.png", CC_CALLBACK_1(LoadingScene::loadCallback, this));
    
    for (int i = 0; i < 8; i++)
    {
        char buf1[1024];
        sprintf(buf1, "ditu_%02d.png",i+1);
        Director::getInstance()->getTextureCache()->addImageAsync(buf1, CC_CALLBACK_1(LoadingScene::loadCallback, this));
    }
    
    
//    Director::getInstance()->getTextureCache()->addImageAsync(GameTools::getName("StarPng"), CC_CALLBACK_1(LoadingScene::loadCallback, this));
    
    initBg();
    
}

void LoadingScene::initBg()
{
    Sprite* spBg = Sprite::create(GameTools::getName("LoadingSceneBg"));
    spBg->setPosition(m_nBgFitNode->getContentSize().width/2, m_nBgFitNode->getContentSize().height/2);
    m_nBgFitNode->addChild(spBg);
    
    
    Sprite* spLoad = Sprite::createWithSpriteFrameName("Loading1.png");
    spLoad->setPosition(m_nCtFitNode->getContentSize().width/2,m_nCtFitNode->getContentSize().height/3);
    m_nCtFitNode->addChild(spLoad);
    
    Vector<SpriteFrame*> vec;
    for (int i = 0; i < 3; i++)
    {
        char buf[1024];
        sprintf(buf, "Loading%d.png",i+1);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buf);
        vec.pushBack(frame);
    }
    //创建动画
    float time = 0.4;
    auto animation = Animation::createWithSpriteFrames(vec,time);
    auto animate = Animate::create(animation);

    spLoad->runAction(RepeatForever::create(animate));
    
    
}

void LoadingScene::loadCallback(cocos2d::Texture2D *texture)
{
    ++m_iNumOfLoadedRes;
    char tmp[1024];
    sprintf(tmp, "%d%%",(int)(((float)m_iNumOfLoadedRes/m_iNumOfRes)*100) );
    m_labelPercent->setString(tmp);
    if (m_iNumOfRes == m_iNumOfLoadedRes)//加载完毕
    {
        scheduleOnce(schedule_selector(LoadingScene::toStartScene), 1.0f);
    }
}

void LoadingScene::toStartScene(float dt)
{
    Scene* scene = MapScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0, scene));
}