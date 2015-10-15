//
//  Configer.h
//  DemonFruit
//
//  Created by 凯 于 on 15/9/22.
//
//

#ifndef __DemonFruit__Configer__
#define __DemonFruit__Configer__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

//定义数据结构

//人物坐标
const Point PointE_1 = Point(355, 1057);
const Point PointE_2 = Point(190, 990);
const Point PointE_3 = Point(80, 1000);
const Point PointE_4 = Point(445, 1000);
const Point PointE_5 = Point(550, 1023);
const Point PointE_6 = Point(720, 1085);

const Point PointS_1 = PointE_1 - Point(380, 0);
const Point PointS_2 = PointE_2- Point(380, 0);
const Point PointS_3 = PointE_3- Point(380, 0);
const Point PointS_4 = PointE_4+ Point(380, 0);
const Point PointS_5 = PointE_5+ Point(380, 0);
const Point PointS_6 = PointE_6+ Point(380, 0);

//人物编号 1~6
enum SpriteId
{
    SPRITEID_0 = 0,
    SPRITEID_1,
    SPRITEID_2,
    SPRITEID_3,
    SPRITEID_4,
    SPRITEID_5
};

//技能编号 1~11
enum MagicId
{
    MAGIC_0 = 0,
    MAGIC_1 = 1,
    MAGIC_2,
    MAGIC_3,
    MAGIC_4,
    MAGIC_5,
    MAGIC_6,
    MAGIC_7,
    MAGIC_8,
    MAGIC_9,
    MAGIC_10,
    MAGIC_11,
};

//人物的状态
enum SpriteState
{
    SPRITESTATE_IDLE,//空闲
    SPRITESTATE_RUN,//跑动
    SPRITESTATE_ATTACK,//进攻
    SPRITESTATE_MAGIC,//释放技能
    SPRITESTATE_HURT,//受伤
    SPRITESTATE_DEAD//死亡
};

//人物的站位
enum  SpriteType
{
    SPRITETYPE_FRONT = 1,    //前排
    SPRITETYPE_MIDDLE,   //中排
    SPRITETYPE_BACK      //后排
};

//移动的种类
enum MoveType
{
    MoveType_To,
    MoveType_By
};

//人物释放技能或者普通攻击对应的果实编号
enum FruitNo
{
    F1 = 0,
    F2,
    F3,
    F4,
    F5,
    F6
};

//每种状态对应的动作所需的图片数
struct StateNum
{
    int idleNum;
    int runNum;
    int attackNum;
    int hurtNum;
    int deadNum;
};

//技能编号。释放技能对应的果实编号
struct MagicGroup
{
    MagicId magic1;//普通远程攻击
    MagicId magic2;//大招
};

//人物的属性
struct _stSpriteConfig
{
    SpriteId id;                //唯一标识
    std::string name;           //名字
    SpriteType type;            //站位 ，前中后
    int life;                   //生命值
    int attackMax;              //最大攻击力
    int attackMin;              //最小攻击力
    double attackSpeed;         //攻击速度
    int power;                  //释放技能所需能量值，对应消除果实的个数
    MagicGroup magicGroup;      //每个人物有两个技能
    StateNum stateNum;          //每种状态的图片数
};

//魔法的属性
struct _stMagicConfig
{
    MagicId id;//唯一标识
    std::string name;//名字
    int count;//图片数
    int attack;//攻击力
};

//？
struct AttackId
{
    bool s1;
    bool s2;
    bool s3;
};

typedef enum{
    Message_GameFailed_            = 1,
    Message_GameSuccess_
    
}Message_Type_;

#endif /* defined(__DemonFruit__Configer__) */
