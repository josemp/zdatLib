# output libreria

NAME    := zlEvtTimer

# fuentes e includes del proyecto

FUENTES  :=  zlEvtTimer.c

INCLUDES := zlEvtTimer.h

# directorios de includes externos

OUTHER_DIR_INCLUDE := /usr/local/zdat/include

INCLUDES_FOR_INSTALL:= zlEvtTimer.h

# install directorios

OUTPUT_LIB_INSTALL ?= 
OUTPUT_INCLUDE_INSTALL ?= 

# variables intermedias


LIBRARY_NAME := lib$(NAME).a

CFLAGS = $(OUTHER_DIR_INCLUDE:%=-I%)
OBJETOS      := $(FUENTES:%.c=%.o)

# -------- RULES ------------------

all: $(LIBRARY_NAME)
$(OBJETOS):$(FUENTEOS) $(INCLUDES)

$(LIBRARY_NAME): $(OBJETOS)
	- rm -f $(LIBRARY_NAME)
	 ar -rf $@ $^

clean:
	 - rm -f $(OBJETOS) $(LIBRARY_NAME)
	make -f makefileTest1 clean
	make -f makefileTest2 clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES_FOR_INSTALL) $(OUTPUT_INCLUDE_INSTALL)

test: $(LIBRARY_NAME)
	make -f makefileTest1
	make -f makefileTest2
