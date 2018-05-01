# nombre de la biblioteca 

NAME         := zlEvt
LIBRARY_NAME := lib$(NAME).a

# Lista de objetos e includes del proyecto

OBJETOS  :=  zlEvtMain.lib/zlEventMain.o zlEvtSocket.lib/zlEvtServer.o zlEvtSocket.lib/zlEvtSocketComun.o zlEvtSocket.lib/zlEvtClient.o zlEvtTimer.lib/zlEvtTimer.o

INCLUDES  :=  zlEvtMain.lib/zlEventMain.h zlEvtSocket.lib/zlEvtServer.h zlEvtSocket.lib/zlEvtClient.h zlEvtSocket.lib/zlEvtSocketComun.h zlEvtTimer.lib/zlEvtTimer.h zlEvt.h


# install directorios

OUTPUT_LIB_INSTALL ?= /usr/local/zdat/lib
OUTPUT_INCLUDE_INSTALL ?= /usr/local/zdat/include

# -------- RULES ------------------

all: compila  $(INCLUDES) $(LIBRARY_NAME)

$(LIBRARY_NAME): $(OBJETOS)
	- rm -f $@
	ar -rf $@ $^

compila:
	$(MAKE) -C zlEvtMain.lib
	$(MAKE) -C zlEvtSocket.lib
	$(MAKE) -C zlEvtTimer.lib

clean:
	$(MAKE) -C zlEvtMain.lib clean
	$(MAKE) -C zlEvtSocket.lib clean
	$(MAKE) -C zlEvtTimer.lib clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES) $(OUTPUT_INCLUDE_INSTALL)
