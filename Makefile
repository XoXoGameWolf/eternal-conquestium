linux: build/eternal-conquestium-linux.zip
windows: build/eternal-conquestium-windows.zip

build/eternal-conquestium-linux.zip: build/app
	rm build/eternal-conquestium-linux.zip
	cp -rf build/app resources zip
	cd zip; zip -r ../build/eternal-conquestium-linux.zip *; rm -rf *

build/eternal-conquestium-windows.zip: build/app.exe
	rm build/eternal-conquestium-windows.zip
	cp -rf build/app.exe resources windows/* zip
	cd zip; zip -r ../build/eternal-conquestium-windows.zip *; rm -rf *

build/app: src/* include/coco/*
	gcc -I include -o build/app src/main.c include/glad.c -lglfw -lGL -lm

build/app.exe: src/* include/coco/*
	x86_64-w64-mingw32-gcc -I include -o build/app.exe src/main.c include/glad.c -L windows -lglfw3 -lopengl32 -lm