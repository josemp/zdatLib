# output libreria

NAME    := zlTimer

# fuentes e includes del proyecto

FUENTES  :=  zlTimer.c

INCLUDES := zlTimer.h

# directorios de includes externos

OUTHER_DIR_INCLUDE :=

INCLUDES_FOR_INSTALL:= zlTimer.h

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
	 tar -rf $@ $^

clean:
	 - rm -f $(OBJETOS) $(LIBRARY_NAME)
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES_FOR_INSTALL) $(OUTPUT_INCLUDE_INSTALL)
