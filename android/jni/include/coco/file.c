#pragma once

void readFile(char* buf, char* path, int size) {
    jclass r = (*env)->FindClass(env, "org/coco/R$raw");

    char* nameDynamic = malloc(64);
    int i = 0;

    for(int j = 0; j < strlen(path); j++) {
        if(path[j] == '/') {
            free(nameDynamic);
            nameDynamic = malloc(64);
            i = 0;
            continue;

        } else if(path[j] == '.') {
            nameDynamic[i] = 0;
            break;
        }

        nameDynamic[i] = path[j];
        i++;
    }

    const char* name = nameDynamic;

    //__android_log_print(ANDROID_LOG_INFO, "Coco Engine", "%s", name);

    jfieldID fid = (*env)->GetStaticFieldID(env, r, name, "I");
    int rid = (int)((*env)->GetStaticIntField(env, r, fid));

    jmethodID mid = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, res), "openRawResource", "(I)Ljava/io/InputStream;");
    jobject stream = (*env)->CallObjectMethod(env, res, mid, (jint)rid);

    jbyteArray jdata = (*env)->NewByteArray(env, size);

    mid = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, stream), "read", "([B)I");
    (*env)->CallIntMethod(env, stream, mid, jdata);

    char* data = (char*)((*env)->GetByteArrayElements(env, jdata, false));

    memcpy(buf, data, size);
}