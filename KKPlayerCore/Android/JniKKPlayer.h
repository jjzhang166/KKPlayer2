/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_ic70_kkplayer_kkplayer_CJniKKPlayer */
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#ifndef _Included_com_ic70_kkplayer_kkplayer_CJniKKPlayer
#define _Included_com_ic70_kkplayer_kkplayer_CJniKKPlayer


JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_GetCapImg(JNIEnv *env, jobject instance, jobject obj)
{
     jclass cls_objClass=env->GetObjectClass( obj);
     jfieldID ImgPixfmtId=env->GetFieldID(cls_objClass,"ImgPixfmt","Ljava/lang/String;");
     jstring BGR= env->NewStringUTF("BGR");
     env->SetObjectField(obj,ImgPixfmtId,BGR);


     jfieldID imgHeightId=env->GetFieldID(cls_objClass,"imgHeight","I");
     env->SetIntField(obj,imgHeightId,1);
     jfieldID imgWidthId=env->GetFieldID(cls_objClass,"imgWidth","I");
     env->SetIntField(obj,imgWidthId,2);

     jfieldID dataSizeId=env->GetFieldID(cls_objClass,"dataSize","I");
     env->SetIntField(obj, dataSizeId,100);

     jfieldID ImgDataId=env->GetFieldID(cls_objClass,"ImgData","[");
     jbyteArray dataImg= env->NewByteArray(100);
     env->SetObjectField(obj, ImgDataId,dataImg);

     //jfieldID ImgPixfmtId=env->GetFieldID(cls_objClass,"ImgPixfmt","Ljava/lang/String;");
}

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_ic70_kkplayer_kkplayer_CJniKKPlayer
 * Method:    IniKK
 * Signature: ()I
 */
 
/**
*初始化一个播放器核心
*@param Render 0 openglview， 1 surfaceview
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_IniKK(JNIEnv *pEv, jobject instance, jint Render);

/**
*初始化opengl 2.0
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_IniGl(JNIEnv *env, jobject instance, jint obj);
/**
*调整程序区域大小
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_OnSize(JNIEnv *env, jobject instance, jint obj, jint w, jint h);
/**
*Gl 呈现call
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_GlRender(JNIEnv *env, jobject instance, jint obj);

/**
*设置是否保持视频长宽比例
*/
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_SetKeepRatio(JNIEnv *env, jobject instance, jint obj, jint KeepRatio);
/**
*Surface 显示,目前不支持
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_SurfaceRender(JNIEnv * env, jobject instance, jint obj, jobject surface);
/**
*删除一个播放器核心
*/
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_DelKK(JNIEnv *env, jobject instance,jint obj);
/**
*打开媒体
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKOpenMedia(JNIEnv *env, jobject instance, jstring str_,jint obj);
/**
*获取音视信息
*/
JNIEXPORT jstring JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_GetMediaInfo(JNIEnv *env, jobject instance, jint obj);
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_Pause(JNIEnv *env, jobject instance,jint obj);
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_Seek(JNIEnv *env, jobject instance, jint obj, jint value);
/**
*是否需要重连，如果是直播则需要重连
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKIsNeedReConnect(JNIEnv *env, jobject instance,jint obj);
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKCloseMedia(JNIEnv *env, jobject instance,jint obj);

/**
* -1 播放器关闭状态 -2，媒体打开失败。如果是实时媒体-2是流终断 。 -3，媒体播放关闭
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKGetPlayerState(JNIEnv *env, jobject instance,jint obj);
/**
*视频已经准备好了 返回1，其它表示没有
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKIsReady(JNIEnv *env, jobject instance,jint obj);

/**
* 获取视频播放时间
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKGetRealtime(JNIEnv *env, jobject instance,jint obj);
/**
*获取视频延迟
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKGetRealtimeDelay(JNIEnv *env, jobject instance,jint obj);
/**
*设置最小延迟
*/
JNIEXPORT jint JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKSetMinRealtimeDelay(JNIEnv *env, jobject instance,jint obj, jint value);
JNIEXPORT void JNICALL Java_com_ic70_kkplayer_kkplayer_CJniKKPlayer_KKForceFlushQue(JNIEnv *env, jobject instance,jint obj);



/**
* JNI 库模块加载事件处理接口。
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * java_vm, void * reserved);
/**
* JNI 库模块卸载事件处理接口。
*/
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * java_vm, void * reserved);
#ifdef __cplusplus
}
#endif
#endif
