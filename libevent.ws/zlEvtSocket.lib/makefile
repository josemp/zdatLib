# output libreria

NAME    := zlEvtServer

# fuentes e includes del proyecto

FUENTES  :=  zlEvtServer.c zlEvtSocketComun.c zlEvtClient.c

INCLUDES := zlEvtServer.h zlEvtSocketComun.h zlEvtClient.h

# directorios de includes externos

OUTHER_DIR_INCLUDE := ../zlEvtTimer.lib /usr/local/zdat/include

INCLUDES_FOR_INSTALL:= zlEvtServer.h zlEvtSocketComun.h

# install directorios

OUTPUT_LIB_INSTALL ?= 
OUTPUT_INCLUDE_INSTALL ?= 

# variables intermedias


LIBRARY_NAME := lib$(NAME).a

CFLAGS = $(OUTHER_DIR_INCLUDE:%=-I%)
OBJETOS      := $(FUENTES:%.c=%.o)

# -------- RULES ------------------

all: $(LIBRARY_NAME)
$(OBJETOS):$(FUENTES) $(INCLUDES)

$(LIBRARY_NAME): $(OBJETOS)
	- rm -f $(LIBRARY_NAME)
	 ar -rf $@ $^

clean:
	 - rm -f $(OBJETOS) $(LIBRARY_NAME)
	make -C . -f makefileTestServer clean
	make -C . -f makefileTestClient clean
	make -C . -f makefileTestClient1 clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES_FOR_INSTALL) $(OUTPUT_INCLUDE_INSTALL)

test: $(LIBRARY_NAME)
	make -C . -f makefileTestServer
	make -C . -f makefileTestClient
	make -C . -f makefileTestClient1
