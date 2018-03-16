# nombre de la biblioteca 

NAME         := zlEvt
LIBRARY_NAME := lib$(NAME).a

# Lista de objetos e includes del proyecto

OBJETOS  :=  zlEvtMain.lib/zlEventMain.o zlEvtSockCli.lib/zlSocketClient.o zlEvtSocket.lib/zlEvtSocket.o zlEvtSocket.lib/zlEvtSocketList.o zlEvtSocket.lib/zlEvtSocketServer.o zlEvtSockSer.lib/zlEvtSockSer.o zlEvtSockSer.lib/zlEvtSockSerConexion.o zlEvtSockSer.lib/zlEvtSockSerConexionItem.o zlEvtTimer.lib/zlEvtTimer.o

INCLUDES  :=  zlEvtSockCli.lib/zlSocketClient.h zlEvtSocket.lib/zlEvtSocket.h zlEvtSocket.lib/zlEvtSocketList.h zlEvtSocket.lib/zlEvtSocketServer.h zlEvtSockSer.lib/zlEvtSockSer.h zlEvtSockSer.lib/zlEvtSockSerConexion.h zlEvtSockSer.lib/zlEvtSockSerConexionItem.h zlEvtTimer.lib/zlEvtTimer.h


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
	$(MAKE) -C zlEvtSockCli.lib
	$(MAKE) -C zlEvtSocket.lib
	$(MAKE) -C zlEvtSockSer.lib
	$(MAKE) -C zlEvtTimer.lib

clean:
	$(MAKE) -C zlEvtMain.lib clean
	$(MAKE) -C zlEvtSockCli.lib clean
	$(MAKE) -C zlEvtSocket.lib clean
	$(MAKE) -C zlEvtSockSer.lib clean
	$(MAKE) -C zlEvtTimer.lib clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES) $(OUTPUT_INCLUDE_INSTALL)
