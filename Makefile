all: build/app
	@build/app

build/app: src/* include/coco/*
	@gcc -I include -o build/app src/main.c include/glad.c -lglfw -lGL -lm