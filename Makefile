#Makefile - Biblioteca Modular de Cálculo 1 em C (libcalc)

#Compilador e Flags
CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -std=c99 -pedantic -Iinclude
PICFLAGS := -fPIC
LDFLAGS  := -lm -fsanitize=address

#Diretórios de Instalação
PREFIX   ?= /usr/local

#Diretórios do Projeto
BUILD_DIR := build
LIB_DIR   := lib
BIN_DIR   := bin
SRC_DIR   := src

#Arquivos Fonte e Objetos
SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
PIC_OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_pic.o)

.PHONY: all clean run test install

#Target Padrão: Constrói as bibliotecas e os executáveis estático e dinâmico
all: $(LIB_DIR)/libcalc.a $(LIB_DIR)/libcalc.so $(BIN_DIR)/app_static $(BIN_DIR)/app_dynamic


#Regras de Compilação Incremental dos Objetos------------------------

#Objetos estáticos (.o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c include/%.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Objetos relocáveis (.o com PIC) para biblioteca dinâmica
$(BUILD_DIR)/%_pic.o: $(SRC_DIR)/%.c include/%.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PICFLAGS) -c $< -o $@


#Geração das Bibliotecas-------------------------------

#Biblioteca Estática (libcalc.a)
$(LIB_DIR)/libcalc.a: $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

#Biblioteca Dinâmica (libcalc.so)
$(LIB_DIR)/libcalc.so: $(PIC_OBJS)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^ $(LDFLAGS)


#Compilação da Aplicação Cliente (app/main.c)------------------------

#Executável vinculado estaticamente
$(BIN_DIR)/app_static: app/main.c $(LIB_DIR)/libcalc.a
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) app/main.c -L$(LIB_DIR) -lcalc $(LDFLAGS) -o $@

#Executável vinculado dinamicamente com RPATH ($ORIGIN/../lib)
$(BIN_DIR)/app_dynamic: app/main.c $(LIB_DIR)/libcalc.so
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) app/main.c -L$(LIB_DIR) -lcalc -Wl,-rpath,'$$ORIGIN/../lib' $(LDFLAGS) -o $@


#Targets de Execução, Testes, Instalação e Limpeza-------------------------------

#Executa o aplicativo dinâmico
run: $(BIN_DIR)/app_dynamic
	./$(BIN_DIR)/app_dynamic

#Compila e executa os testes automatizados
test: $(LIB_DIR)/libcalc.a
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) tests/*.c -L$(LIB_DIR) -lcalc $(LDFLAGS) -o $(BIN_DIR)/test_runner
	./$(BIN_DIR)/test_runner
	

#Instala bibliotecas e cabeçalhos
install: all
	install -d $(DESTDIR)$(PREFIX)/lib
	install -d $(DESTDIR)$(PREFIX)/include/calc
	install -m 644 $(LIB_DIR)/libcalc.a $(DESTDIR)$(PREFIX)/lib/
	install -m 755 $(LIB_DIR)/libcalc.so $(DESTDIR)$(PREFIX)/lib/
	install -m 644 include/*.h $(DESTDIR)$(PREFIX)/include/calc/

#Limpa os artefatos compilados
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(BIN_DIR)