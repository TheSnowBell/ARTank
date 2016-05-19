/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class br_artoolkit_artank_ARTank */

#ifndef _Included_br_artoolkit_artank_ARTank
#define _Included_br_artoolkit_artank_ARTank
#ifdef __cplusplus
extern "C" {
#endif
#undef br_artoolkit_artank_ARTank_MODE_PRIVATE
#define br_artoolkit_artank_ARTank_MODE_PRIVATE 0L
#undef br_artoolkit_artank_ARTank_MODE_WORLD_READABLE
#define br_artoolkit_artank_ARTank_MODE_WORLD_READABLE 1L
#undef br_artoolkit_artank_ARTank_MODE_WORLD_WRITEABLE
#define br_artoolkit_artank_ARTank_MODE_WORLD_WRITEABLE 2L
#undef br_artoolkit_artank_ARTank_MODE_APPEND
#define br_artoolkit_artank_ARTank_MODE_APPEND 32768L
#undef br_artoolkit_artank_ARTank_MODE_MULTI_PROCESS
#define br_artoolkit_artank_ARTank_MODE_MULTI_PROCESS 4L
#undef br_artoolkit_artank_ARTank_MODE_ENABLE_WRITE_AHEAD_LOGGING
#define br_artoolkit_artank_ARTank_MODE_ENABLE_WRITE_AHEAD_LOGGING 8L
#undef br_artoolkit_artank_ARTank_BIND_AUTO_CREATE
#define br_artoolkit_artank_ARTank_BIND_AUTO_CREATE 1L
#undef br_artoolkit_artank_ARTank_BIND_DEBUG_UNBIND
#define br_artoolkit_artank_ARTank_BIND_DEBUG_UNBIND 2L
#undef br_artoolkit_artank_ARTank_BIND_NOT_FOREGROUND
#define br_artoolkit_artank_ARTank_BIND_NOT_FOREGROUND 4L
#undef br_artoolkit_artank_ARTank_BIND_ABOVE_CLIENT
#define br_artoolkit_artank_ARTank_BIND_ABOVE_CLIENT 8L
#undef br_artoolkit_artank_ARTank_BIND_ALLOW_OOM_MANAGEMENT
#define br_artoolkit_artank_ARTank_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef br_artoolkit_artank_ARTank_BIND_WAIVE_PRIORITY
#define br_artoolkit_artank_ARTank_BIND_WAIVE_PRIORITY 32L
#undef br_artoolkit_artank_ARTank_BIND_IMPORTANT
#define br_artoolkit_artank_ARTank_BIND_IMPORTANT 64L
#undef br_artoolkit_artank_ARTank_BIND_ADJUST_WITH_ACTIVITY
#define br_artoolkit_artank_ARTank_BIND_ADJUST_WITH_ACTIVITY 128L
#undef br_artoolkit_artank_ARTank_CONTEXT_INCLUDE_CODE
#define br_artoolkit_artank_ARTank_CONTEXT_INCLUDE_CODE 1L
#undef br_artoolkit_artank_ARTank_CONTEXT_IGNORE_SECURITY
#define br_artoolkit_artank_ARTank_CONTEXT_IGNORE_SECURITY 2L
#undef br_artoolkit_artank_ARTank_CONTEXT_RESTRICTED
#define br_artoolkit_artank_ARTank_CONTEXT_RESTRICTED 4L
#undef br_artoolkit_artank_ARTank_RESULT_CANCELED
#define br_artoolkit_artank_ARTank_RESULT_CANCELED 0L
#undef br_artoolkit_artank_ARTank_RESULT_OK
#define br_artoolkit_artank_ARTank_RESULT_OK -1L
#undef br_artoolkit_artank_ARTank_RESULT_FIRST_USER
#define br_artoolkit_artank_ARTank_RESULT_FIRST_USER 1L
#undef br_artoolkit_artank_ARTank_DEFAULT_KEYS_DISABLE
#define br_artoolkit_artank_ARTank_DEFAULT_KEYS_DISABLE 0L
#undef br_artoolkit_artank_ARTank_DEFAULT_KEYS_DIALER
#define br_artoolkit_artank_ARTank_DEFAULT_KEYS_DIALER 1L
#undef br_artoolkit_artank_ARTank_DEFAULT_KEYS_SHORTCUT
#define br_artoolkit_artank_ARTank_DEFAULT_KEYS_SHORTCUT 2L
#undef br_artoolkit_artank_ARTank_DEFAULT_KEYS_SEARCH_LOCAL
#define br_artoolkit_artank_ARTank_DEFAULT_KEYS_SEARCH_LOCAL 3L
#undef br_artoolkit_artank_ARTank_DEFAULT_KEYS_SEARCH_GLOBAL
#define br_artoolkit_artank_ARTank_DEFAULT_KEYS_SEARCH_GLOBAL 4L
/*
 * Class:     br_artoolkit_artank_ARTank
 * Method:    shutdown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTank_shutdown
  (JNIEnv *, jobject object);

/*
 * Class:     br_artoolkit_artank_ARTank
 * Method:    shot
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTank_shot
  (JNIEnv *, jobject object);

/*
 * Class:     br_artoolkit_artank_ARTank
 * Method:    cannonUp
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTank_cannonUp
  (JNIEnv *, jobject object);

/*
 * Class:     br_artoolkit_artank_ARTank
 * Method:    cannonDown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTank_cannonDown
  (JNIEnv *, jobject object);

/*
 * Class:     br_artoolkit_artank_ARTank
 * Method:    reset
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTank_reset
  (JNIEnv *, jobject object);

/*
 * Class:     br_artoolkit_artank_ARTank
 * Method:    initialise
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTank_initialise
  (JNIEnv *, jobject object);

#ifdef __cplusplus
}
#endif
#endif
