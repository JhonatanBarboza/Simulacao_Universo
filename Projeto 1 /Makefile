# Nome do executável
TARGET = solar

# Compilador e flags
CC = g++
CFLAGS = -Wall -g

# Bibliotecas necessárias
LIBS = -lGLEW -lGL -lGLU -lglut

# Arquivos fonte
SRCS = solar.cpp

# Arquivos objeto
OBJS = $(SRCS:.cpp=.o)

# Regras
all: clean $(TARGET) run

# Regra para compilar o programa
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Regra para compilar os arquivos objeto
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -f $(OBJS) $(TARGET)

# Evita que make interprete "clean" ou "run" como nomes de arquivos
.PHONY: clean run all