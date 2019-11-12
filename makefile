NAMES		= anchor token
LIBS    	= -lez430
SRC			= radio.c
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

all: $(patsubst %, ${OUT_DIR}/%.elf,${NAMES}) ez430-drivers

flash_token: all
	mspdebug rf2500 "prog ${OUT_DIR}/anchor.elf"

flash_anchor: all
	mspdebug rf2500 "prog ${OUT_DIR}/token.elf"

${OUT_DIR}/anchor.elf: ${OBJ_DIR}/anchor.o

${OUT_DIR}/token.elf: ${OBJ_DIR}/token.o

${OUT_DIR}/%.elf: ${OBJ}
	@mkdir -p ${OUT_DIR}
	${CC} -mmcu=${CPU} $^ ${LDFLAGS} -o $@
	msp430-size $@

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

