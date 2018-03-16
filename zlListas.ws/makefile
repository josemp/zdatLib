# nombre de la biblioteca 

NAME         := zlListas
LIBRARY_NAME := lib$(NAME).a

# Lista de objetos e includes del proyecto

OBJETOS  :=  zlLista1.lib/zlLista1.o

INCLUDES  := 


# install directorios

OUTPUT_LIB_INSTALL ?= /usr/local/zdat/lib
OUTPUT_INCLUDE_INSTALL ?= /usr/local/zdat/include

# -------- RULES ------------------

all: compila  $(INCLUDES) $(LIBRARY_NAME)

$(LIBRARY_NAME): $(OBJETOS)
	- rm -f $@
	ar -rf $@ $^

compila:
	$(MAKE) -C zlLista1.lib

clean:
	$(MAKE) -C zlLista1.lib clean
install:
	- mkdir -p $(OUTPUT_LIB_INSTALL)
	- mkdir -p $(OUTPUT_INCLUDE_INSTALL)
	cp $(LIBRARY_NAME) $(OUTPUT_LIB_INSTALL)
	cp $(INCLUDES) $(OUTPUT_INCLUDE_INSTALL)
