# output program

NAME    := testSocket

# fuentes e includes del proyecto

FUENTES  :=  testCliente1.c testServer1.c testSocketMain.c

INCLUDES :=

# directorios de includes externos

OUTHER_DIR_INCLUDE := .. ../../zlEvtTimer.lib /usr/local/zdat/include

OUTHER_DIR_LIB:= .. ../../zlEvtTimer.lib /usr/local/zdat/lib

# librerias utilizadas

OUTHER_LIBS:=  -lzlEvtSocket -lzlEvtTimer -lzlStd -levent

# variables intermedias

CFLAGS = $(OUTHER_DIR_INCLUDE:%=-I%)
LDFLAGS = $(OUTHER_DIR_LIB:%=-L%)
LDLIBS =     $(OUTHER_LIBS)
OBJETOS      := $(FUENTES:%.c=%.o)

# -------- RULES ------------------

all: $(NAME)
$(OBJETOS):$(FUENTES) $(INCLUDES)
$(NAME): $(OBJETOS)
	$(CC) $(LDFLAGS)  $(OBJETOS) $(LDLIBS) -o $(NAME)

clean:
	 - rm -f $(OBJETOS) $(NAME)
