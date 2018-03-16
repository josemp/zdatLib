# nombre de la biblioteca 

NAME         := zlStd
LIBRARY_NAME := lib$(NAME).a

# Lista de objetos e includes del proyecto

OBJETOS  :=  zlLog.lib/zlLog.o zlTimer.lib/zlTimer.o zlUtil.lib/file2Buf.o zlUtil.lib/zlVarios.o zlReadBuf.lib/zlBufRead.o

INCLUDES  :=  zlLog.lib/zlLog.h zlTimer.lib/zlTimer.h zlUtil.lib/file2Buf.h zlUtil.lib/zlUtil.h zlReadBuf.lib/zlBufRead.h zlStd.h zlStdComun.h


# install directorios

OUTPUT_LIB_INSTALL ?= /usr/local/zdat/lib
OUTPUT_INCLUDE_INSTALL ?= /usr/local/zdat/include

# -------- RULES ------------------

all: compila  $(INCLUDES) $(LIBRARY_NAME)

$(LIBRARY_NAME): $(OBJETOS)
	- rm -f $@
	ar -rf $@ $^

compila:
	$(MAKE) -C zlLog.lib
	$(MAKE) -C zlTimer.lib
	$(MAKE) -C zlUtil.lib
	$(MAKE) -C zlReadBuf.lib

clean:
	$(MAKE) -C zlLog.lib clean
	$(MAKE) -C zlTimer.lib clean
	$(MAKE) -C zlUtil.lib clean
	$(MAKE) -C zlReadBuf.lib clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES) $(OUTPUT_INCLUDE_INSTALL)
