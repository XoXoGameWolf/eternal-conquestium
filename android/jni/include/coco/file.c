#pragma once

void readFile(char* buf, char* path, int maxSize) {
    jclass r = (*env)->FindClass(env, "org/coco/R$raw");

    char nameDynamic[64];
    int i = 0;

    for (int j = 0; j < strlen(path); j++) {
        if (path[j] == '/') {
            i = 0;
            continue;
        } else if (path[j] == '.') {
            nameDynamic[i] = 0;
            break;
        }
        nameDynamic[i++] = path[j];
    }

    jfieldID fid = (*env)->GetStaticFieldID(env, r, nameDynamic, "I");
    int rid = (*env)->GetStaticIntField(env, r, fid);

    jmethodID mid = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, res), "openRawResource", "(I)Ljava/io/InputStream;");
    jobject stream = (*env)->CallObjectMethod(env, res, mid, rid);

    jmethodID availableMid = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, stream), "available", "()I");
    jint available = (*env)->CallIntMethod(env, stream, availableMid);

    if (available > maxSize) available = maxSize;

    jbyteArray jdata = (*env)->NewByteArray(env, available);
    mid = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, stream), "read", "([B)I");
    jint bytesRead = (*env)->CallIntMethod(env, stream, mid, jdata);

    jbyte* data = (*env)->GetByteArrayElements(env, jdata, NULL);
    memcpy(buf, data, bytesRead);
    (*env)->ReleaseByteArrayElements(env, jdata, data, JNI_ABORT);
}