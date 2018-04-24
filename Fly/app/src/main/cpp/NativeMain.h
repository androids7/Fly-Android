//
// Created by renjie on 18-4-21.
//

#ifndef FLY_NATIVEMAIN_H

#include <android/native_activity.h>
#include "MyGLTools.h"
extern "C" {
#include "Vector.h"
}

typedef struct tagEvent{
    unsigned int ID;
    unsigned int EventID;
} Event;

extern Vector *pEventVt;

void bindLifeCycle(ANativeActivity *activity);
void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize);
void *looper(void *args);
void onStart(ANativeActivity *activity);
void onResume(ANativeActivity *activity);
void *onSaveInstanceState(ANativeActivity *activity, size_t *outSize);
void onPause(ANativeActivity *activity);
void onStop(ANativeActivity *activity);
void onDestroy(ANativeActivity *activity);
void onWindowFocusChanged(ANativeActivity *activity, int hasFocus);
void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *window);
void onNativeWindowDestroyed(ANativeActivity *activity, ANativeWindow *window);
void onInputQueueCreated(ANativeActivity *activity, AInputQueue *queue);
void onInputQueueDestroyed(ANativeActivity *activity, AInputQueue *queue);
void onConfigurationChanged(ANativeActivity *activity);
void onLowMemory(ANativeActivity *activity);

void DoEvent(MyEventManager *pM);

#define FLY_NATIVEMAIN_H

#endif //FLY_NATIVEMAIN_H
