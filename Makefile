all:
	gcc toralizer.c -o toralize.so -fPIC -shared -ldl -D_GNU_SOURCE