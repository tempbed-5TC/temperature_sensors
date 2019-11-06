NAME	= temperature_sensor
LIBS    = -lez430
SRC		= main.c send.c
SRC_DIR		= src
INC_DIR		= -I./ez430-drivers/inc -Iprotothreads
OUT_DIR		= bin
LIB_DIR		= ./ez430-drivers/lib
OBJ_DIR		= .obj
DOC_DIR		= doc
DEP_DIR 	= .deps
OBJ		    = $(patsubst %.c,${OBJ_DIR}/%.o,$(SRC))
DEPS		= $(patsubst %.c,$(DEP_DIR)/%.d,$(SRC))
# Platform EZ430
CPU			= msp430f2274
CFLAGS		= -g -Wall -mmcu=${CPU} ${INC_DIR}
LDFLAGS		= -static -L${LIB_DIR} ${LIBS}
CC			= msp430-gcc
MAKEDEPEND	= ${CC} ${CFLAGS} -MM -MP -MT $@ -MF

all: ${OUT_DIR}/${NAME}.elf ez430-drivers

flash: all
	mspdebug rf2500 "prog ${OUT_DIR}/${NAME}.elf"

${OUT_DIR}/${NAME}.elf: ${OBJ}
	@mkdir -p ${OUT_DIR}
	${CC} -mmcu=${CPU} ${OBJ} ${LDFLAGS} -o $@
	msp430-size ${OUT_DIR}/${NAME}.elf

$(OBJ_DIR)/%.o: ${SRC_DIR}/%.c
	@mkdir -p ${OBJ_DIR} ${DEP_DIR}
	${MAKEDEPEND} $(@:.o=.d) $<
	${CC} ${CFLAGS} -c $< -o $@

-include ${DEPS}

.PHONY: ez430-drivers
ez450-drivers:
	@$(MAKE) -C ez430-drivers

.PHONY: clean
clean:
	@rm -Rf ${OUT_DIR} ${OBJ_DIR} ${DEP_DIR} ${DOC_DIR}

.PHONY: rebuild
rebuild: clean all

.PHONY: doc
doc:
	doxygen

