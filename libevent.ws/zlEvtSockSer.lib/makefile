# output libreria

NAME    := zlEvtSockSer

# fuentes e includes del proyecto

FUENTES  :=  zlEvtSockSer.c zlEvtSockSerConexion.c zlEvtSockSerConexionItem.c

INCLUDES := zlEvtSockSer.h zlEvtSockSerConexion.h zlEvtSockSerConexionItem.h

# directorios de includes externos

OUTHER_DIR_INCLUDE := /usr/local/zdat/include

INCLUDES_FOR_INSTALL:= zlEvtSockSer.h zlEvtSockSerConexion.h zlEvtSockSerConexionItem.h

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
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES_FOR_INSTALL) $(OUTPUT_INCLUDE_INSTALL)

test:
