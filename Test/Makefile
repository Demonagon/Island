XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11 -lXext -lpthread
GLLIBS = -L/usr/X11R6/lib -lglut -lGL -lGLU
 
CFLAGS =-ggdb3

simulBras : simulBras.o
	mkdir -p bin; \
	gcc  -o bin/simulBras simulBras.o $(GLLIBS) $(XLIBS) -lm
 
