//
//  DemonLevel.h
//  DemonElimate
//
//  Created by 凯 于 on 15/7/6.
//
//

#ifndef __DemonElimate__DemonLevel__
#define __DemonElimate__DemonLevel__

#include <stdio.h>
#include "DemonFruit.h"
#include "DemonTile.h"
#include "DemonSwap.h"
#include "DemonChain.h"
#include "../Header.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "../Controller/GameManager.h"

using namespace rapidjson;
using namespace std;
class DemonLevel : public Node
{
public:
    DemonLevel();
    ~DemonLevel();
    static DemonLevel* create(const string& strName);//创建
    void shuffle();//开始游戏、洗牌
private:
    //Game Setup
    bool loadJson(const string& strName);//读取json文件
    bool initWithFile(const string& strName);//根据文件初始化关卡
    void initTileMatrix();//初始化m_tileMatrix
    void initIMatrix();//初始化m_iMatrix
    void initFruitMatrix();//初始化m_fruitMatrix
public:
    //pos of fruit
    void dropFruit(DemonFruit* fruit);
    void setZeroPoint();//设置左下角的果实的坐标
    Point getPosOfItem(int row, int col);//根据行列值返回对应的坐标
    
    
    //Querying the Level
    DemonFruit* fruitAtRC(int row, int col);//根据行列获得对应的果实--没用到
    DemonTile* tileAtRc(int row, int col);//根据行列获得对应的地图块
    DemonFruit* getFruitofPos(Point pos);//根据坐标返回果实
    DemonFruit* getFruitofRC(int row, int col);//根据行列返回果实
    
    bool isPossibleSwap(DemonSwap* swap);//判断当前的交换是否为有效交换
    
    //Detecting Swaps
    long detectPossibleSwaps();//检测交换，并将所有交换存入m_allPossibleSwaps,返回m_allPossibleSwaps的长度（有多少种交换）
    void printSwaps();//打印可交换的组合
    bool hasChainAtRC(int row, int col);//根据行列值判断矩阵中是否有可消除链
    
    //Swapping
    void performSwap(DemonSwap* swap);//当为有效交换时执行兵更新矩阵，无效只做动画，不更新矩阵
    
    //Detecting Matches
    long detectHorizontalMatches();//检测行
    long detectVerticalMatches();//检测列
    void removeChians();//移除，并更新矩阵
    
    //fill holes
    void fillHoles();//填空
    void fillTop();////填充顶端的
    
    //giveHints
    void giveHints();//随机给出一个可以消除的组合
    
    //giveScore
    //Note: number为每一轮消除中所有被消除的果实的个数，若产生连消，basicScore改变为上一轮消除分值
    long giveScore(long basicScore,long number);//根据分值基数和消除的果实个数给出分值
    
    Size m_fruitSize;//果实大小 64 X 64
    Document m_doc;//josn操作都在document中
    int** m_iMatrix;//保存果实编号的矩阵，如果当前格子没有果实，编号为-1
    DemonTile*** m_tileMatrix;//最终内容为DemonTile*
    DemonFruit*** m_fruitMatrix;//最终内容为DemonFruit*
    
    float m_leftBottomPosX;//果实矩阵做下角的点的横坐标
    float m_leftBottomPosY;//果实矩阵做下角的点的纵坐标
    

    Vector<DemonSwap*> m_allPossibleSwaps;//保存了所有的交换 DemonSwap引用+1而不是DemonSwap中的两个果实引用+1
    Vector<DemonChain*> m_removeMatches;//保存了所有的可移除的链 同上DemonChain引用+1
    Vector<DemonFruit*> m_fallFruits;//保存将要掉落的果实
    //蒙版
    Node* m_stencil;//将所有地图节点加入模板节点
    ClippingNode* m_clippingNode;//使用模板节点创建
  
};

#endif /* defined(__DemonElimate__DemonLevel__) */
