# output libreria

NAME    := zlLista1

# fuentes e includes del proyecto

FUENTES  :=  zlLista1.c

INCLUDES := zlLista1.h

# directorios de includes externos

OUTHER_DIR_INCLUDE :=

INCLUDES_FOR_INSTALL:=

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
	make -C . -f makefileTest1 clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES_FOR_INSTALL) $(OUTPUT_INCLUDE_INSTALL)

test: $(LIBRARY_NAME)
	make -C . -f makefileTest1
