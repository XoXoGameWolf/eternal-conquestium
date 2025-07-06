linux: build/eternal-conquestium-linux.zip

windows: build/eternal-conquestium-windows.zip

build/eternal-conquestium-linux.zip: build/app
	zip -r build/eternal-conquestium-linux.zip build/app resources run.sh

build/eternal-conquestium-windows.zip: build/app.exe
	zip -r build/eternal-conquestium-windows.zip build/app.exe resources run.bat

build/app: src/* include/coco/*
	gcc -I include -o build/app src/main.c include/glad.c -lglfw -lGL -lm

build/app.exe: src/* include/coco/*
	x86_64-w64-mingw32-gcc -I include -o build/app.exe src/main.c include/glad.c -L windows -lglfw3 -lopengl32 -lm