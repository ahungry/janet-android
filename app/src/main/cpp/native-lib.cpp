#include <jni.h>
#include <string>

#include "janet.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_ahungry_janet_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

  // Eval some janet and do nothing else
  JanetTable *janet_env;
  janet_init ();
  janet_env = janet_core_env (NULL);
  const char *embed = "((fn [] \"Greetings from janet env\"))";
  Janet *out = (Janet *) malloc (1);
  janet_dostring (janet_env, embed, "main", out);
  janet_deinit ();
  const char *result = janet_getcstring (out, 0);

  return env->NewStringUTF (result);
}
