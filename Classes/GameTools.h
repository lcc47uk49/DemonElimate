//
//  GameTools.h
//  DemonElimination
//
//  Created by 凯 于 on 15-6-30.
//  单利 工具类
//

#ifndef __DemonElimination__GameTools__
#define __DemonElimination__GameTools__

#include <stdio.h>
#include "cocos2d.h"
#include "Header.h"
using namespace std;
USING_NS_CC;

class GameTools
{
public:
    static GameTools* getInstance();
    // 获取系统时间，毫秒，比如返回5100即5.100秒
    static double getCurrentTime();
    //获得文件名
    static string getName(string str);
    
    
    GameTools();
    ~GameTools();
    
    /**
     *  初始化对象,初始化默认设备宽高比
     */
    void initObj();
    
    //===================适配接口===================
    //全屏拉伸
    void scaleFullScreen(Node*);
    //不出屏幕拉伸
    void scaleFitInScreen(Node*);
    //出屏幕拉伸
    void scaleFitOutScreen(Node*);
    
    //横向拉伸
    void scaleXScreen(Node*);
    //纵向拉伸
    void scaleYScreen(Node*);
    //=============================================
    /**
     *  创建文件名
     *
     *  @return 文件名
     */
    std::string createBufFormat(const char *fmt, ...);
    
    bool isContain(vector<Point> &v, Point p);//判断是否包含的函数
    
    Size m_winSize;
private:
    Size m_defaultSize;
    
    float m_fDefaultRatio;            //默认宽高比
    float m_fWinRatio;                //设备宽高比
    float m_fScaleX;                  //x拉伸比
    float m_fScaleY;                  //y拉伸比
    float m_fScaleFit;                //不出屏幕拉伸
    float m_fScaleFitReverse;         //出屏幕拉伸

};


#endif /* defined(__DemonElimination__GameTools__) */
