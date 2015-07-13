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
#include "../GameTools.h"
#include "../Header.h"
#include "../Controller/GameManager.h"
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
    virtual void update(float dt);
    //单点触摸
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
public:
    void animateSwap(DemonSwap* swap);//交换成功动画
    void animateInvalidSwap(DemonSwap* swap);//交换失败动画
    void animateFall();//掉落动画
    void animateExplode();//消除动画
    void animateScore(long score,Point pos);//在给定的坐标上进行得分动画
    void animateExplodeEffect(int fruitType, Point pos);//根据果实类型和坐标，设置消除特效
    
    void callNDBackSetZOrder(Sprite* sp,int zOrder);//设置z轴顺序
    void callNActionEndRelease(Node* node);//动作结束引用-1
    void callNRemoveScoreLabel(Node* node);//动作结束移除标签
private:
    DemonLevel* m_level;//保存关卡信息，同时也是屏幕中心适配节点
    Node* m_bgFitNode;//背景适配节点
    
    DemonFruit* m_srcFruit;//选择的果实
    DemonFruit* m_dstFruit;//要交换的果实
    
    //提示
    double m_prevTime;//不能用float，溢出了
    double m_aftTime;
    
    //触摸控制
    bool m_touchEnable;//是否可触摸
    bool m_isAnimation;//是否有动画
    
    //基本分值，每次连续消除会使m_basicScore乘2，效果果实个数从三个开始，每多一个分值乘2。每次交换都会重置m_basicScore为__BASIC_SCORE。如果是自动连续消除，则不经过交换，所以不重置m_basicScore。例如 第一次消除3个引发第二次的4消，第一次__BASIC_SCORE分，第二次为2*2*__BASIC_SCORE分（第二次的基本分值为__BASIC_SCORE*2）。第一次5消，引发三次连续的3消，则分值为2*2*__BASIC_SCORE，2*__BASIC_SCORE,4*__BASIC_SCORE，8*__BASIC_SCORE
    long m_basicScore;
    
};

#endif /* defined(__DemonElimate__PlayLayer__) */
