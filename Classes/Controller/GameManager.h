//
//  GameManager.h
//  DemonElimination
//
//  Created by 凯 于 on 15-6-29.
//  管理类---单例模式
//

#ifndef __DemonElimination__GameManager__
#define __DemonElimination__GameManager__

#include <stdio.h>
#include "cocos2d.h"
#include <vector>
#include <string>
#include "../Header.h"
#include "../Configer.h"
#include "json/document.h"
using namespace std;
USING_NS_CC;

//读写文件记录玩家数据
class GameManager
{
public:
    static GameManager* getInstance();
    GameManager();
    ~GameManager();
    
    //添加一个解锁的恶魔果实的编号
    void addunlockFruitsNo(const int a);
    //返回已解锁的恶魔果实编号的引用
    vector<int>& getunlockFruitsNo();
    
    //添加一个选择的恶魔果实的编号
    void addselectedFruitsNo(const int a);
    //移除一个选择的恶魔果实的编号
    void rmvselectedFruitsNo(const int a);
    //返回已选择的恶魔果实编号的引用
    vector<int>& getselectedFruitsNo();
    
    //根据已解锁的和已选择的编号，设置将要使用的果实的编号
    void setusedFruitsNo();
    //返回将要使用的果实的编号
    vector<int>& getusedFruitsNo();
    
    //根据讲要使用的果实编号，设置将果实的名字
    void setusedFruitsName();
    //返回将要是用的果实名字的引用
    vector<string>& getusedFruitsName();
    const string& getHighLightFruitName(int fruitType);//根据果实类型返回对应的高亮图片
    
    //读取技能等级
    int getSkillLevel(int fruitType);
    //设置技能等级
    void setSkillLevel(int fruitType, int level);
    
    
    bool isSkillUnlock(int skillNo);//判断技能是否解锁
    void addUnlockSkill(int skillNo);//解锁新技能
    
private:
    //已解锁的恶魔果实编号
    vector<int> m_unlockFruitsNo;
    //所选择的恶魔果实编号，选择个数需要在3~7个之间
    vector<int> m_selectedFruitsNo;
    //用来创建游戏的果实的编号 固定7个
    vector<int> m_usedFruitsNo;
    //用来创建游戏的果实的名字 固定7个
    vector<string> m_usedFruitsName;
    
    //已经解锁的技能 = 已解锁的果实种类 + 后来解锁的大招
    vector<int> m_unlockSkillNo;
public:
//---------------------------------------战斗系统-------------------------------------------
    Animate* setAnimate(const char* frameName, int frameCount, int fps, bool restore = true,int times =1);//创建动画，帧名、帧数、帧数（这里感觉应该改成帧用时）、是否重置为第一张图片，重复次数
    
    std::string getMagicName(MagicId id);//根据魔法的id获取魔法的名字
    int getMagicNum(MagicId id);//获取魔法的图片数
    
    std::string getSpriteName(SpriteId id);//获取人物的名字
    std::string getStateName(SpriteId id, SpriteState state);//获取状态的名字
    std::string getAttackName(MagicId magicId);//获取攻击动作的名字
    
    int heroNum;//英雄数
    int enemyNum;//敌人数
    bool spSel[6];//？
    bool isWin;//是否胜利
    bool isGameOver;//是否游戏结束
};
#endif /* defined(__DemonElimination__GameManager__) */
