//
//  PlayLayer.h
//  DemonElimate
//
//  Created by 凯 于 on 15/7/8.
//
//

#ifndef __DemonElimate__PlayLayer__
#define __DemonElimate__PlayLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "../Model/DemonFruit.h"
#include "../Model/DemonLevel.h"
#include "../Model/DemonTile.h"
#include "../Model/DemonSkill.h"
#include "../GameTools.h"
#include "../Header.h"
#include "../Controller/GameManager.h"
#include "SpriteSystem.h"
#include "../Message.h"

USING_NS_CC;
using namespace std;

class PlayLayer : public Layer
{
public:
    PlayLayer();
    ~PlayLayer();
     static Scene* createScene();
    CREATE_FUNC(PlayLayer);
    virtual bool init();
    //定时器
    virtual void update(float dt);
    void addEnemyTimer(float dt);
    //单点触摸
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
public:
    //animations
    void animateSwap(DemonSwap* swap);//交换成功动画
    void animateInvalidSwap(DemonSwap* swap);//交换失败动画
    void animateFall();//掉落动画
    void animateExplode();//消除动画
    void animateSkillExplode(Vector<DemonChain*>& chains);//技能消除动画，消除chains中的果实
    void animateScore(long score,Point pos);//在给定的坐标上进行得分动画
    void animateExplodeEffect(int fruitType, Point pos);//根据果实类型和坐标，设置消除特效
    void animateCombos();//消除连击
    void animateSkill(int fruitType);//技能动画
    void animateCountDown();//倒计时动画
    
    //functions
    void calcuteScore();//计算分值
    void giveSkill();//释放技能，根据m_elimateFruitType
    void collideWithTree();//碰撞检测，判断是否与树有碰撞了
    void collideWithEnemy();//碰撞检测，判断子弹是否与怪物碰撞了
    //callbacks
    void callNDBackSetZOrder(Sprite* sp,int zOrder);//设置z轴顺序
    void callNActionEndRelease(Node* node);//动作结束引用-1
    void callNRemoveScoreLabel(Node* node);//动作结束移除标签
    void callGameBegin(float dt);//游戏开始,允许触摸，计时开始
    
    //消息处理
    void handleMsg(float dt);
private:
    DemonLevel* m_level;//保存关卡信息，同时也是屏幕中心适配节点
    SpriteSystem* m_spriteSystem;//战斗层
    Node* m_bgFitNode;//背景适配节点
    
    DemonFruit* m_srcFruit;//选择的果实
    DemonFruit* m_dstFruit;//要交换的果实
    
    //提示
    double m_prevTime;//不能用float，溢出了
    double m_aftTime;
    
    //触摸控制
    bool m_touchEnable;//是否可触摸
    bool m_isAnimation;//是否有动画
    
    int m_skillElimateCount;//记录技能消除了多少个果实，用于释放必杀
    
    //基本分值，每次连续消除会使m_basicScore乘2，效果果实个数从三个开始，每多一个分值乘2。每次交换都会重置m_basicScore为__BASIC_SCORE。如果是自动连续消除，则不经过交换，所以不重置m_basicScore。例如 第一次消除3个引发第二次的4消，第一次__BASIC_SCORE分，第二次为2*2*__BASIC_SCORE分（第二次的基本分值为__BASIC_SCORE*2）。第一次5消，引发三次连续的3消，则分值为2*2*__BASIC_SCORE，2*__BASIC_SCORE,4*__BASIC_SCORE，8*__BASIC_SCORE
    long m_basicScore;
    int m_comboNum;//记录连击数，每次消除连击数+1，但是每次交换会置为0（默认）
    vector<int> m_elimateFruitType;//记录消除的果实的种类，方便判断是否释放技能，只记录玩家消除或者连消的，不记录技能打击消除的
    Label* m_comboLabel;//连击标签,加在m_level中，这样可以固定相对位置
    
public:
    //静态消息队列
    static MessageQueue* s_messageQ;
};

#endif /* defined(__DemonElimate__PlayLayer__) */
