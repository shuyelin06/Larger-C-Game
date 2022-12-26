CC 			= gcc
FLAGS		:= -MMD
INCLUDE		= -I./include -I./src

LIB			= -L./lib
LIBPATH 	:= -lmingw32 -lSDL2main -lSDL2

# SOURCES 	:= $(wildcard *.c)
SOURCES		:= main.c geometry/polygon.c entity/entity.c graphics/graphics.c
OBJECTS 	:= $(patsubst %.c, bin/%.o, ${SOURCES})
EXE  		= main.exe

FILES := 

default: all
all: ${EXE}

${EXE}: ${OBJECTS}
	${CC} ${FLAGS} ${LIB} ${OBJECTS} -o ${EXE} ${LIBPATH}

bin/%.o: src/%.c
	$(CC) ${FLAGS} ${INCLUDE} -c $< -o $@

clean:
	echo ${OBJECTS} | rm

-include $(OBJS:.o=.d)