# output libreria

NAME    := zlUtil

# fuentes e includes del proyecto

FUENTES  :=  file2Buf.c zlVarios.c zlValidIpv4.c zlHostbyName.c

INCLUDES := file2Buf.h zlUtil.h zlValidIpv4.h zlHostbyName.h

# directorios de includes externos

OUTHER_DIR_INCLUDE := ..

INCLUDES_FOR_INSTALL:= file2Buf.h zlUtil.h zlHostbyName.h zlValidIpv4.h

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
	make -C . -f makefileTestIp clean
	make -C . -f makefileTestHostbyName clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES_FOR_INSTALL) $(OUTPUT_INCLUDE_INSTALL)

test: $(LIBRARY_NAME)
	make -C . -f makefileTestIp
	make -C . -f makefileTestHostbyName
