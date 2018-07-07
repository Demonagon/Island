#XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib
#GLLIBS = 
INCLUDES = -Iheader
CFLAGS = -std=c99 -pedantic -Wall $(INCLUDES)
LFLAGS = -L/usr/X11/lib -L/usr/X11R6/lib -L/usr/X11R6/lib
LIBS = -lX11 -lXext -lpthread -lglut -lGL -lGLU -lm
DEBUG = -ggdb3

CC = gcc

BIN_FOLDER=bin
OBJ_FOLDER=obj
HEADER_FOLDER=header
SRC_FOLDER=src

EXEC=island

MAIN=$(BIN_FOLDER)/$(EXEC)

HEADERS=$(wildcard $(HEADER_FOLDER)/*.h)
SRCS=$(wildcard $(SRC_FOLDER)/*.c)
OBJS=$(patsubst $(SRC_FOLDER)/%.c,$(OBJ_FOLDER)/%.o,$(SRCS))

.PHONY: depend clean

all: $(MAIN)
	@echo $(MAIN) has been compiled

$(MAIN): $(OBJS)
	mkdir -p $(BIN_FOLDER); \
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.c
	mkdir -p $(OBJ_FOLDER); \
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ_FOLDER)/*.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
