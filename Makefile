CC = C:/MinGW/bin/gcc.exe
SRC = trabalho_aeroportos/main.c trabalho_aeroportos/menu.c trabalho_aeroportos/grafo.c
OUT = programa.exe

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	del $(OUT)
