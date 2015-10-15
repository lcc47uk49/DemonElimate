//
//  Header.h
//  DemonElimate
//
//  Created by 凯 于 on 15-7-6.
//
//

#ifndef DemonElimate_Header_h
#define DemonElimate_Header_h

#define __FRUIT_MATRIX_WIDTH            (9) //果实矩阵列数
#define __FRUIT_MATRIX_HEIGHT           (9) //果实矩阵行数
#define __DEFAULT_WINWIDTH              (750)//设计屏幕宽
#define __DEFAULT_WINHEIGHT             (1334)//设计屏幕高
#define __FRUIT_WIDTH                   (64)//果实的宽
#define __FRUIT_HEIGHT                  (64)//果实的高
#define __FRUIT_GAP                     (6) //果实间的间隔
#define __TIME_DIFFERENCE_HINT          (5000)//提示时间间隔
#define __TIME_DIFFERENCE_COMBOS        (1000)//重置combos的时间间隔
#define TOTAL_SELECTEDFRUIT_NUM         (6)//最多可以选择的恶魔果实的数量
#define __BASIC_SCORE                   (60)//基本分值60分
#define __SKILL_DIFFERENCE              (1000)//大招和普通技能之间差值为1000
#define __RUN_ACTION_TIME               (4)//人物跑动的时间

//路径
static const char s_magPan[]              =  "panel.png";

static const char s_panSpNor[3][64]     =  {"sprite1.png","sprite2.png","sprite3.png"};
static const char s_panSpDis[3][64]     =  {"sprite1no.png","sprite2no.png","sprite3no.png"};

static const char s_ptBg[]           =  "lifebg.png";
static const char s_ptBg2[]          =  "lifebg2.png";
static const char s_ptLife[]         =  "herolife.png";
static const char s_ptLife2[]         =  "life.png";
static const char s_ptPower[]         =  "power.png";

static const char s_spConfJson[]          =  "json/spriteConfig.json";
static const char s_spMagJson[]          =  "json/magicConfig.json";

#endif
