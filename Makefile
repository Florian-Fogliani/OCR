#Makefile

CC=gcc
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags sdl2 SDL2_image) -lm
LDLIBS = $(shell pkg-config --libs sdl2 SDL2_image) -lm

SRC= solver.c draw.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}


SRC2= pret_orient.c pret_tocut.c pret_analyse.c pret.c
OBJ2=${SRC2:.c=.o}
EXE2=${SRC2:.c=}

SRC3= cutter.c hough.c
OBJ3= cutter.o hough.o

SRC4= process.c src/*.c
OBJ4= process.o file.o network.o training.o useful_maths.o


all:solver pret process cutter

-include  ${DEP}

solver: ${OBJ}
pret: ${OBJ2}
cutter: ${OBJ3}
process: ${OBJ4}

${OBJ4}:
	${CC} ${CFLAGS} -c ${SRC4}

.PHONY: clean


clean:
	${RM} ${OBJ}
	${RM} ${OBJ2}
	${RM} ${OBJ3}
	${RM} ${OBJ4}
	${RM} ${DEP}
	${RM} ${EXE2}
	${RM} ${EXE3}
	${RM} ${DEP4}
	${RM} solver
	${RM} pret
	${RM} cutter
	${RM} process
	${RM} *result
	${RM} *result.bmp

#end
