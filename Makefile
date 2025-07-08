all: linux windows

linux: build/eternal-conquestium-linux.zip
windows: build/eternal-conquestium-windows.zip

build/eternal-conquestium-linux.zip: build/app
	rm build/eternal-conquestium-linux.zip
	mkdir game
	cp -rf build/app resources game
	zip -r build/eternal-conquestium-linux.zip game
	rm -rf game

build/eternal-conquestium-windows.zip: build/app.exe
	rm build/eternal-conquestium-windows.zip
	mkdir game
	cp -rf build/app.exe resources windows/* game
	zip -r build/eternal-conquestium-windows.zip game
	rm -rf game

build/app: src/* include/coco/*
	gcc -I include -o build/app src/main.c include/glad.c -lglfw -lGL -lm

build/app.exe: src/* include/coco/*
	x86_64-w64-mingw32-gcc -I include -o build/app.exe src/main.c include/glad.c -L windows -lglfw3 -lopengl32 -lm