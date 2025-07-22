NAME := eternal-conquestium
ANDROID := /home/wolf/Android/Sdk/platforms/android-36/android.jar

all: linux windows android

linux: build/$(NAME)-linux.zip
windows: build/$(NAME)-windows.zip
android: build/$(NAME)-android.apk

build/$(NAME)-linux.zip: build/app
	rm build/$(NAME)-linux.zip
	mkdir $(NAME)
	cp -rf build/app resources $(NAME)
	zip -r build/$(NAME)-linux.zip $(NAME)
	rm -rf $(NAME)

build/$(NAME)-windows.zip: build/app.exe
	rm build/$(NAME)-windows.zip
	mkdir $(NAME)
	cp -rf build/app.exe resources windows/* $(NAME)
	zip -r build/$(NAME)-windows.zip $(NAME)
	rm -rf $(NAME)

build/$(NAME)-android.apk: android/key.keystore android/jni/include/coco/* src/* android/jni/include/*
	cp -rf $(shell find resources/* -type f) android/res/raw
	cp src/* android/jni/src
	aapt package -f -m -J android/src -M android/AndroidManifest.xml -I $(ANDROID) -S android/res
	cd android; ndk-build
	javac -d android/obj -classpath android/src -classpath $(ANDROID) android/src/org/coco/*.java
	d8 --output . android/obj/org/coco/*.class
	aapt package -f -m -F android/unaligned-$(NAME)-android.apk -M android/AndroidManifest.xml -I $(ANDROID) -S android/res
	aapt add android/unaligned-$(NAME)-android.apk classes.dex
	cp -rf android/libs/* lib
	aapt add android/unaligned-$(NAME)-android.apk lib/arm64-v8a/*.so lib/armeabi-v7a/*.so
	aapt add android/unaligned-$(NAME)-android.apk lib/x86/*.so lib/x86_64/*.so
	zipalign -f 4 android/unaligned-$(NAME)-android.apk build/$(NAME)-android.apk
	apksigner sign --ks android/key.keystore build/$(NAME)-android.apk
	rm -rf android/res/raw/* classes.dex android/obj android/unaligned-$(NAME)-android.apk android/jni/src/*

android/key.keystore:
	rm android/key.keystore
	keytool -genkeypair -validity 1000000 -keystore android/key.keystore -keyalg RSA -keysize 2048

build/app: src/* include/coco/*
	gcc -I include -o build/app src/main.c include/glad.c -lglfw -lGL -lm

build/app.exe: src/* include/coco/*
	x86_64-w64-mingw32-gcc -I include -o build/app.exe src/main.c include/glad.c -L windows -lglfw3 -lopengl32 -lm