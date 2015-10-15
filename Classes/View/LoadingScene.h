//
//  LoadingScene.h
//  EG
//
//  Created by 凯 于 on 15/10/1.
//
//

#ifndef __DemonElimate__LoadingScene__
#define __DemonElimate__LoadingScene__

//载入界面,加载资源
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class LoadingScene : public Layer
{
public:
    LoadingScene();
    ~LoadingScene();
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LoadingScene);
private:
    void loadRes();//加载资源
    void initBg();//添加背景和动画等
    void loadCallback(Texture2D* texture);//加载完成的回调
    void toStartScene(float dt);//切换到开始场景
private:
    int m_iNumOfRes;//需要加载的资源总数
    int m_iNumOfLoadedRes;//已加载的资源数
    ProgressTimer* m_pt;//载入进度条
    Label* m_labelLoad;//载入标签
    Label* m_labelPercent;//载入百分比
    Node* m_nBgFitNode;//背景适配节点
    Node* m_nCtFitNode;//中心适配节点
};

#endif /* defined(__DemonElimate__LoadingScene__) */
