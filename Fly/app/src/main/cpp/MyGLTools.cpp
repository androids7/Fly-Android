//
// Created by renjie on 18-4-23.
//

#include <android/log.h>
#include <stdlib.h>
#include "MyGLTools.h"
#include <EGL/egl.h>
#include <android/native_window.h>

#define LOG_TAG "NATIVE_MAIN_CPP"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR ,LOG_TAG ,__VA_ARGS__)

MyEvent *CreateMyEvent(){
    MyEvent *pEvent=(MyEvent *)malloc(sizeof(MyEvent));
    pEvent->EventID=MYEVENT_ID_NONE;
    pEvent->ID=MyMakeID();
    pEvent->pTag=NULL;
    return pEvent;
}

MyEvent *CreateMyEventCreateWindow(ANativeWindow *window){
    MyCreateWindowEventTag *pTag=(MyCreateWindowEventTag *)malloc(sizeof(MyCreateWindowEventTag));
    MyEvent *pEvent=CreateMyEvent();
    pEvent->EventID=MYEVENT_ID_CREATEWINDOW;
    pTag->pWindow=window;
    pEvent->pTag=pTag;

    return pEvent;
}

void DestoryMyEvent(MyEvent *pEvent){
    if(pEvent->pTag) free(pEvent->pTag);
    free(pEvent);
}

MyEventManager *CreateMyEventManager(){
    MyEventManager *pM=(MyEventManager *)malloc(sizeof(MyEventManager));
    pM->pEventVt=vtCreate();
    return pM;
}

unsigned int AddMyEvent(MyEventManager *pM,
                MyEvent *pEvent){
    vtAddBack(pM->pEventVt,pEvent);

    return pEvent->EventID;
}

MyEvent *RemoveMyEvent(MyEventManager *pM,
                unsigned int ID){
    for(int i=0;i<vtCount(pM->pEventVt);i++){
        MyEvent *pEvent=(MyEvent *)vtGet(pM->pEventVt,i);

        if(pEvent->ID==ID){
            vtRemove(pM->pEventVt,i);
            return pEvent;
        }
    }
    return NULL;
}

void DestoryAllEvents(MyEventManager *pM){
    for(int i=0;i<vtCount(pM->pEventVt);i++){
        MyEvent *pEvent=(MyEvent *)vtGet(pM->pEventVt,i);
        DestoryMyEvent(pEvent);
    }

    pM->pEventVt->nCount=0;
}

bool MyEventManagerHasEvent(MyEventManager *pM){
    return vtCount(pM->pEventVt);
}

void InitGL(ANativeWindow *window){
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    int ver;
    eglInitialize(display, &ver, 0);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGE("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
}

unsigned int MyMakeID(){
    static unsigned int ID=0;

    return ++ID;
}