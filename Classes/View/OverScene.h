//
//  OverScene.h
//  DemonElimate
//
//  Created by 凯 于 on 15/10/13.
//
//

#ifndef __DemonElimate__OverScene__
#define __DemonElimate__OverScene__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class OverScene : public LayerColor
{
public:
    OverScene();
    ~OverScene();
    bool init(int index);
    static OverScene* create(int index);//根据index创建胜利失败的对话框
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};
#endif /* defined(__DemonElimate__OverScene__) */
