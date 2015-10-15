//
//  message.cpp
//  starBlazer
//
//  Created by jiabl on 13-12-5.
//
//

#include "message.h"
#include <iostream>

using namespace std;

Message* Message::instance(){
    //todo release
    Message *e = new Message();
    return e;
}

Message::Message(){
    mesType = 0;
    x = 0.f;
    y = 0.f;
}

Message::~Message(){
    //    cout << "message release" << endl;
}
int Message::getMesType(){
    return mesType;
}
void Message::setMesType(int t){
    mesType = t;
}
float Message::getX(){
    return x;
}
void Message::setX(const float xt){
    x = xt;
}
float Message::getY(){
    return y;
}
void Message::setY(const float yt){
    y = yt;
}
void Message::setCount(const long _count){
    count = _count;
}
long Message::getCount(){
    return count;
}
void Message::setId(int _id){
    msgid = _id;
}
int Message::getId(){
    return msgid;
}

MessageQueue::MessageQueue(){
    
}
Message* MessageQueue::pop(){
    if(msgs.size() > 0){
        //        pthread_mutex_lock(&work_mutex);
        Message *ms = msgs.front();
        msgs.pop();
        //        pthread_mutex_unlock(&work_mutex);
        return ms;
    }
    return NULL;
}

Message* MessageQueue::peek(){
    if(msgs.size() > 0){
        //        pthread_mutex_lock(&work_mutex);
        Message *ms = msgs.front();
        //        pthread_mutex_unlock(&work_mutex);
        return ms;
    }
    return NULL;
}

//移除头
void MessageQueue::popFront(){
    //    pthread_mutex_lock(&work_mutex);
    msgs.pop();
    //    pthread_mutex_unlock(&work_mutex);
}

void MessageQueue::push(Message *ms){
    //    pthread_mutex_lock(&work_mutex);
    msgs.push(ms);
    //    pthread_mutex_unlock(&work_mutex);
}

unsigned int MessageQueue::size(){
    return msgs.size();
}

MessageQueue::~MessageQueue(){
    //    pthread_mutex_destroy(&work_mutex);
    cout << "MessageQueue release" << endl;
}