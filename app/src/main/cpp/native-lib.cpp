#include <jni.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "janet.h"

JNIEnv ** global_env;
jobject global_obj;

static Janet
call_jni (int32_t argc, Janet *argv)
{
  janet_fixarity (argc, 3);
  const char *message = janet_getcstring (argv, 0);
  const char *use_class = janet_getcstring (argv, 1);
  const char *use_method = janet_getcstring (argv, 2);

  // jstring jstr = (*global_env)->NewStringUTF("This comes from jni janet.");
  jstring jstr = (*global_env)->NewStringUTF(message);
  // jclass clazz = (*global_env)->FindClass("com/ahungry/janet/MainActivity");
  jclass clazz = (*global_env)->FindClass(use_class);

  jmethodID messageMe = (*global_env)->GetMethodID(clazz, use_method, "(Ljava/lang/String;)Ljava/lang/String;");
  jobject result = (*global_env)->CallObjectMethod(global_obj, messageMe, jstr);

  const char* str = (*global_env)->GetStringUTFChars((jstring) result, NULL);
  const uint8_t *s = janet_string (reinterpret_cast<const uint8_t *>(str), strlen (str));

  // free (str);
  // printf("%s\n", str);

  return janet_wrap_string (s);
}

static const JanetReg cfuns[] = {
  {"call-jni", call_jni, "lol"},
  {NULL,NULL,NULL}
};

extern "C" JNIEXPORT jstring JNICALL
Java_com_ahungry_janet_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject obj /* this */) {

  // Hoist these up for the janet runtime stuff.
  global_env = &env;
  global_obj = obj;

  // Eval some janet and do nothing else
  JanetTable *janet_env;
  janet_init ();
  janet_env = janet_core_env (NULL);
  janet_cfuns (janet_env, "lol", cfuns);
  // const char *embed = "(do (call-jni) ((fn [] \"a Greetings from janet env\")))";
  const char *embed = "(call-jni \"Woot This is Janet text\" \"com/ahungry/janet/MainActivity\" \"messageMe\")";
  Janet *out = (Janet *) malloc (1);
  janet_dostring (janet_env, embed, "main", out);
  janet_deinit ();
  const char *result = janet_getcstring (out, 0);

  return env->NewStringUTF (result);
}
