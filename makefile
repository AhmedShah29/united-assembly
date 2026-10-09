build:
	gcc -I. src/*.c -o usm
ctest:
	usm examples/hello.usm
	nasm -f win64 output.asm -o output.obj
	gcc -o hello.exe output.obj
	./hello.exe