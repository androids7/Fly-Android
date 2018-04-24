//
// Created by renjie on 18-4-23.
//

#ifndef MYGLTOOLS_H
#define MYGLTOOLS_H

#include <android/native_window.h>
extern "C" {
#include "Vector.h"
}

#define             MYEVENT_ID_NONE                     0
#define             MYEVENT_ID_CREATEWINDOW             1

typedef struct tagMyEvent{                              //事件
    unsigned int ID;                                    //对象ID
    unsigned int EventID;                               //事件类型ID
    void *pTag;
} MyEvent;

typedef struct tagMyCreateWindowEventTag{               //创建窗口附加信息
    void *pWindow;
} MyCreateWindowEventTag;

typedef struct tagMyEventManager{                       //事件管理器
    Vector *pEventVt;
} MyEventManager;

MyEvent *CreateMyEvent();                               //创建事件
void DestoryMyEvent(MyEvent *pEvent);                   //销毁事件

MyEvent *CreateMyEventCreateWindow(
                ANativeWindow *window);                 //创建窗口事件

MyEventManager *CreateMyEventManager();                 //创建事件管理器
unsigned int AddMyEvent(MyEventManager *pM,              //添加事件
                    MyEvent *pEvent);

MyEvent *RemoveMyEvent(MyEventManager *pM,                //移除事件
                    unsigned int ID);
void DestoryAllEvents(MyEventManager *pM);
bool MyEventManagerHasEvent(MyEventManager *pM);

void InitGL(ANativeWindow *window);
unsigned int MyMakeID();                                //产生一个独一无二的ID

#endif
