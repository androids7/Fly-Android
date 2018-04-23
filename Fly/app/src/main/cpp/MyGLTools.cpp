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

MyEvent *CreateMyEventCreateWindow(){
    MyEvent *pEvent=CreateMyEvent();
    pEvent->EventID=MYEVENT_ID_CREATEWINDOW;

    return pEvent;
}

void DestoryMyEvent(MyEvent *pEvent){
    if(pEvent->pTag) free(pEvent->pTag);
    free(pEvent);
}

unsigned int AddEvent(MyEventManager *pM,
                MyEvent *pEvent){
    vtAddBack(pM->pEventVt,pEvent);

    return pEvent->EventID;
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