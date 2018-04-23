//
// Created by renjie on 18-4-23.
//

#ifndef MYGLTOOLS_H
#define MYGLTOOLS_H

#include <android/native_window.h>
#include "Vector.h"

#define             MYEVENT_ID_NONE                     0
#define             MYEVENT_ID_CREATEWINDOW             1

typedef struct tagMyEvent{                              //事件
    unsigned int ID;                                    //对象ID
    unsigned int EventID;                               //事件类型ID
    void *pTag;
} MyEvent;

typedef struct tagMyEventManager{                       //事件管理器
    Vector *pEventVt;
} MyEventManager;

MyEvent *CreateMyEvent();                               //创建事件
void DestoryMyEvent(MyEvent *pEvent);                   //销毁事件

MyEvent *CreateMyEventCreateWindow();                   //创建窗口事件

unsigned int AddEvent(MyEventManager *pM,               //添加事件
                    MyEvent *pEvent);

MyEvent *RemoveEvent(MyEventManager *pM,                //移除事件
                    unsigned int ID);

void InitGL(ANativeWindow *window);
unsigned int MyMakeID();                                //产生一个独一无二的ID

#endif
