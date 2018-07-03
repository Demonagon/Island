XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11 -lXext -lpthread
GLLIBS = -L/usr/X11R6/lib -lglut -lGL -lGLU
 
CFLAGS =-ggdb3

_BIN :
	mkdir bin

simulBras : simulBras.o $(BIN)
	gcc  -o bin/simulBras simulBras.o $(GLLIBS) $(XLIBS) -lm
 
