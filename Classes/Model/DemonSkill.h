//
//  DemonSkill.h
//  DemonElimate
//
//  Created by 凯 于 on 15/7/24.
//
//

#ifndef __DemonElimate__DemonSkill__
#define __DemonElimate__DemonSkill__

#include <stdio.h>
#include "cocos2d.h"
#include "DemonLevel.h"
#include "DemonChain.h"
#include "../Controller/GameManager.h"
#include "../GameTools.h"
using namespace std;
USING_NS_CC;

//技能类，效果添加到m_level中
class DemonSkill
{
public:
    DemonSkill();
    ~DemonSkill();
    static DemonSkill* getInstance();
    bool initWithLevel(DemonLevel* level);
public:
    void fireSkill(DemonChain* chain);//火焰技能
    void iceSkill(DemonChain* chain);//冰霜技能
    void lightSkill(DemonChain* chain);//炫光技能
    void treatSkill();//治愈果实
    void powerSkill();//力量果实
    void goldSkill();//金钱果实
private:
    DemonLevel* m_level;
};

#endif /* defined(__DemonElimate__DemonSkill__) */
