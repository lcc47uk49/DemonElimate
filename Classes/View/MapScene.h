//
//  MapScene.h
//  DemonElimate
//
//  Created by 凯 于 on 15/10/13.
//
//

#ifndef __DemonElimate__MapScene__
#define __DemonElimate__MapScene__

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio::timeline;

//地图类，用于选择关卡
class MapScene : public Layer
{
public:
    MapScene();
    ~MapScene();
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MapScene);
};
#endif /* defined(__DemonElimate__MapScene__) */
