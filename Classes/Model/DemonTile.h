//
//  DemonTile.h
//  DemonElimate
//
//  Created by 凯 于 on 15/7/7.
//
//

#ifndef __DemonElimate__DemonTile__
#define __DemonElimate__DemonTile__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

//根据json创建地图，如果是0，则为空洞，1为正常
class DemonTile : public Sprite
{
public:
    DemonTile();
    ~DemonTile();
    static DemonTile* create(const string& strName);
    // Note: To support different types of tiles, you can add properties here that
    // indicate how this tile should behave. For example, if a cookie is matched
    // that sits on a jelly tile, you'd set isJelly to NO to make it a normal tile.
    // for example     @property (assign, nonatomic) BOOL isJelly;
};

#endif /* defined(__DemonElimate__DemonTile__) */
