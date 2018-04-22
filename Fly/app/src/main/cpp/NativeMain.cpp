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
static pthread_mutex_t mut;

Vector *pEventVt;
EGLDisplay display;
EGLSurface surface;

void onStart(ANativeActivity *activity) {
    LOGE("Application is start");
   // Vector *pVt=vtCreate();

    //LOGE("%d",123);
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

ANativeWindow *gWindow;

void InitDisplay(ANativeWindow *window){

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


    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
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

    // Initialize GL state.
    ::display=display;
    ::surface=surface;
}

void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *window) {
    gWindow=window;
    Event *pEvent=(Event *)malloc(sizeof(Event));
    pEvent->EventID=1111;
    pthread_mutex_lock(&mut);
    vtAddBack(pEventVt,pEvent);
    pthread_mutex_unlock(&mut);
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

void DoEvent(){
    InitDisplay(gWindow);
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(display,surface);
}

void *looper(void *args) {
    ANativeActivity *activity = (ANativeActivity *) args;
    AInputQueue *queue = (AInputQueue *) activity->instance;
    AInputEvent *event = NULL;
    while (isLoop) {
        pthread_mutex_lock(&mut);
        if(vtCount(pEventVt)){
            DoEvent();
        }
        pthread_mutex_unlock(&mut);

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
                        break;
                    }
                    case AMOTION_EVENT_ACTION_UP: {
                        LOGE("Touch Scerrn UP");
                        break; } default: break; } break; } case AINPUT_EVENT_TYPE_KEY: { switch (AKeyEvent_getAction(event)) { case AKEY_EVENT_ACTION_DOWN: { LOGE("key down"); switch (AKeyEvent_getKeyCode(event)) { case AKEYCODE_BACK: {
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
    pEventVt=vtCreate();
    LOGE("SSSSSS");
    pthread_mutex_init(&mut,NULL);
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
