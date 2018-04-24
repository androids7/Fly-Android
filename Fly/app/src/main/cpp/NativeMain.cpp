#include <android/log.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <pthread.h>

#include "NativeMain.h"

#define LOG_TAG "NATIVE_MAIN_CPP"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR ,LOG_TAG ,__VA_ARGS__)

static bool isLoop=false;
static pthread_t loopID;
static pthread_mutex_t MyEventMut;
static MyEventManager *pMyEventManager;

void onStart(ANativeActivity *activity) {
}

void onResume(ANativeActivity *activity) {

}

void *onSaveInstanceState(ANativeActivity *activity, size_t *outSize) {

}

void onPause(ANativeActivity *activity) {

}

void onStop(ANativeActivity *activity) {

}

void onDestroy(ANativeActivity *activity) {

}

void onWindowFocusChanged(ANativeActivity *activity, int hasFocus) {

}

void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *window) {
    MyEvent *pEvent=CreateMyEventCreateWindow(window);
    pthread_mutex_lock(&MyEventMut);
    AddMyEvent(pMyEventManager,pEvent);
    pthread_mutex_unlock(&MyEventMut);
}

void onNativeWindowDestroyed(ANativeActivity *activity, ANativeWindow *window) {

}

void onInputQueueCreated(ANativeActivity *activity, AInputQueue *queue) {
    isLoop=true;
    activity->instance=(void *)queue;
    pthread_create(&loopID,NULL,looper,activity);
}

void onInputQueueDestroyed(ANativeActivity *activity, AInputQueue *queue) {
    isLoop=false;
}

void DoCreateWindow(MyEvent *pEvent){
    MyCreateWindowEventTag *pTag=(MyCreateWindowEventTag *)pEvent->pTag;

    InitGL((ANativeWindow *)pTag->pWindow);
}

void DoEvent(MyEventManager *pM){
    for(int i=0;i<vtCount(pM->pEventVt);i++){
        MyEvent *pEvent=(MyEvent *)vtGet(pM->pEventVt,i);

        switch(pEvent->EventID){
            case MYEVENT_ID_CREATEWINDOW:
                DoCreateWindow(pEvent);
                break;
        }
    }
}

void *looper(void *args) {

    ANativeActivity *activity = (ANativeActivity *) args;
    AInputQueue *queue = (AInputQueue *) activity->instance;
    AInputEvent *event = NULL;
    while (isLoop) {
        pthread_mutex_lock(&MyEventMut);
        if(MyEventManagerHasEvent(pMyEventManager)){
            DoEvent(pMyEventManager);
        }
        DestoryAllEvents(pMyEventManager);
        pthread_mutex_unlock(&MyEventMut);
        if (!AInputQueue_hasEvents(queue)) {
            continue;
        }
        AInputQueue_getEvent(queue, &event);
        float mx = AMotionEvent_getX(event, 0);
        float my = AMotionEvent_getY(event, 0);
        switch (AInputEvent_getType(event)) {
            case AINPUT_EVENT_TYPE_MOTION: {
                switch (AMotionEvent_getAction(event)) {
                    case AMOTION_EVENT_ACTION_DOWN: {
                        LOGE("Touch Screen Down");
                        break;
                    }
                    case AMOTION_EVENT_ACTION_UP: {
                        LOGE("Touch Screen UP");
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case AINPUT_EVENT_TYPE_KEY: {
                switch (AKeyEvent_getAction(event)) {
                    case AKEY_EVENT_ACTION_DOWN: {
                        LOGE("key down");
                        switch (AKeyEvent_getKeyCode(event)) {
                            case AKEYCODE_BACK: {
                                LOGE("BACK down");
                                ANativeActivity_finish(activity);
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    }
                    case AKEY_EVENT_ACTION_UP: {
                        LOGE("key up");
                        break;
                    }
                    default:
                        break;
                }
            }
            default:
                break;
        }
        AInputQueue_finishEvent(queue, event, 1);
    }
    return args;
}


void onConfigurationChanged(ANativeActivity *activity) {

}

void onLowMemory(ANativeActivity *activity) {

}

void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize) {
    pMyEventManager=CreateMyEventManager();
    pthread_mutex_init(&MyEventMut,NULL);
    bindLifeCycle(activity);
}

void bindLifeCycle(ANativeActivity *activity) {
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
}
