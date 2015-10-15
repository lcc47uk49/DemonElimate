//
//  OverScene.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/10/13.
//
//

#include "OverScene.h"


OverScene::OverScene()
{
    
}

OverScene::~OverScene()
{
    
}

OverScene* OverScene::create(int index)
{
    OverScene* ovScene = new OverScene();
    ovScene->initWithColor(Color4B(0,0,0,128));
    if (ovScene && ovScene->init(index))
    {
        ovScene->autorelease();
        return ovScene;
    }
    else
    {
        delete ovScene;
        ovScene = nullptr;
        return nullptr;
    }
}

bool OverScene::init(int index)
{
    if (index == 1)
    {
        Label* lab = Label::createWithTTF("Success", "Marker Felt.ttf", 60);
        lab->setPosition(this->getContentSize().width/2,this->getContentSize().height/2);
        addChild(lab);
    }
    else if(index == 2)
    {
        Label* lab = Label::createWithTTF("Failed", "Marker Felt.ttf", 60);
        lab->setPosition(this->getContentSize().width/2,this->getContentSize().height/2);
        addChild(lab);
    }
    
    //吞噬触摸
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(OverScene::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

bool OverScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}