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


void *looper(void *args) {

    return NULL;
}


void onConfigurationChanged(ANativeActivity *activity) {

}

void onLowMemory(ANativeActivity *activity) {

}

void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize) {

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
