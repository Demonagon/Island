INCLUDES = -Iheader
CFLAGS = -std=c99 -pedantic -Wall -fPIC -no-pie
LFLAGS = -L/usr/X11/lib -L/usr/X11R6/lib -L/usr/X11R6/lib -L./lib
LIBS = -lX11 -lXext -lpthread -lglut -lGL -lGLU -lm -lml -fPIC
DEBUG = #-g # <- retirer le premier # pour débugger

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

$(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.c lib/libml.a
	mkdir -p $(OBJ_FOLDER); \
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(DEBUG)

ML_SRC_FOLDER = lib/ml/src
ML_OBJ_FOLDER = lib/ml/obj

ML_SRCS = $(wildcard $(ML_SRC_FOLDER)/*.c)
ML_OBJS = $(patsubst $(ML_SRC_FOLDER)/%.c,$(ML_OBJ_FOLDER)/%.o,$(ML_SRCS))

lib/libml.a : $(ML_OBJS)
	ar -cvq lib/libml.a $(ML_OBJS)

$(ML_OBJ_FOLDER)/%.o : $(ML_SRC_FOLDER)/%.c
	mkdir -p $(ML_OBJ_FOLDER); \
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(DEBUG) -fPIC

clean:
	$(RM) $(OBJ_FOLDER)/*.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
