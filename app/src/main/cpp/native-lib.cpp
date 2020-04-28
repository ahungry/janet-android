#include <jni.h>
#include <string>

// #include "janet.h"
// #include "janet.c"

extern "C" JNIEXPORT jstring JNICALL
Java_com_ahungry_janet_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
