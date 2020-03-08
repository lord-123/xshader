all: xshader

xshader: xshader.o
	gcc xshader.o -lX11 -lGL -lGLU -o xshader

xshader.o: xshader.c config.h
	gcc -c -lX11 -lGL -lGLU xshader.c

clean:
	rm -f *.o xshader

install: all
	mkdir -p /usr/local/bin
	cp -f xshader /usr/local/bin
	chmod 755 /usr/local/bin/xshader
