//
//  MapScene.cpp
//  DemonElimate
//
//  Created by 凯 于 on 15/10/13.
//
//

#include "MapScene.h"
#include "PlayLayer.h"

MapScene::MapScene()
{
    
}

MapScene::~MapScene()
{
    
}

Scene* MapScene::createScene()
{
    Scene* scene = Scene::create();
    MapScene* layer = MapScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool MapScene::init()
{
    if (! Layer::init())
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    auto btn = dynamic_cast<Button*>(rootNode->getChildByName("svStageSelet")->getChildByName("Stage01"));
    
    btn->addClickEventListener([](Ref*){
        Director::getInstance()->replaceScene(TransitionFade::create(0, PlayLayer::createScene()));
    });//块代码 完成了回调
    
    addChild(rootNode);
    
    return true;
}
