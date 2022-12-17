CC		:= g++
OUT		:= prog
FLAGS	:= -Wall -g -std=c++17
INC		:= -Isrc -Isrc
LIBS	:= -lglfw -lGL -lGLEW -lSDL2 -lSDL2_image -lm -lassimp
SRCDIR	:= src
OBJDIR	:= obj
MAIN	:= main.cpp

SRCFILES:=${shell find ${SRCDIR} -type f -iname '*.cpp'}
OBJFILESWPATH:=${patsubst ${SRCDIR}/%.cpp,${OBJDIR}/%.o,${SRCFILES}}
OBJFILESWOUTPATH:=${addprefix ${OBJDIR}/,${notdir ${patsubst %.cpp,%.o,${SRCFILES}}}}


# ${info $$SRCFILES are [${SRCFILES}]}
# ${info $$OBJFILESWPATH are [${OBJFILESWPATH}]}
# ${info $$OBJFILESWOUTPATH are [${OBJFILESWOUTPATH}]}


run: all
	./${OUT}

all: ${OBJFILESWPATH}
	${CC} -o ${OUT} ${OBJFILESWOUTPATH} ${FLAGS} ${INC} ${LIBS} ${MAIN}

clean:
	rm -rf ${OBJDIR}/* && rm ${OUT}

${OBJDIR}/%.o: ${SRCDIR}/%.cpp
	${CC} -c -o ${addprefix ${OBJDIR}/,${notdir $@}} $< ${FLAGS} ${INC} ${LIBS}
