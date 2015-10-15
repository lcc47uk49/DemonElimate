//
//  message.h
//  starBlazer
//
//  Created by jiabl on 13-12-5.
//
//

#ifndef __starBlazer__message__
#define __starBlazer__message__

#include <iostream>
#include <queue>
#include "cocos2d.h"
//#include <pthread.h>

class Message{
public:
    Message();
    ~Message();
    static Message* instance();
    int getMesType();
    void setMesType(int t);
    float getX();
    void setX(const float);
    float getY();
    void setY(const float);
    void setCount(const long);
    long getCount();
    void setId(int _id);
    int getId();
    
    CC_SYNTHESIZE(int, m_payId, PayId); //计费点ID
    CC_SYNTHESIZE(int, payNum, PayNum); //购买数量
    
private:
    int mesType;
    long count;
    float x;
    float y;
    
    int msgid;
};

//用于存储队列
class MessageQueue{
public:
    MessageQueue();
    Message* pop();
    Message* peek();
    void popFront();
    void push(Message* msg);
    unsigned int size();
    
    ~MessageQueue();
private:
    std::queue<Message *> msgs;
};

#endif /* defined(__starBlazer__message__) */
