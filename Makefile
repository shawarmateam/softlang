# vars
BUILD_D = bin
CODE_D = src

# cmds
all: build

build: ./${CODE_D}/main.c
	gcc ${CODE_D}/main.c -o ${BUILD_D}/soft

run: build
	./${BUILD_D}/soft

clean:
	rm -rf ./${BUILD_D}/*

# aliases
clr: clean
