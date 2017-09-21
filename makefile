# makefile for SNAKE

NAME = snake

WARNINGS = -Wall -Wextra -Winline -Werror
COMPILER = clang++-3.8 -std=c++1y

GAME_SRC = main.cpp game/*.cpp
CORE_SRC = core/*.cpp
SOURCES = $(CORE_SRC) $(GAME_SRC)

SDL = -lSDL2 -lSDL2_image -lSDL2_ttf

rel: clang
	date +"%nCompiled on: %A, %T (%Y %b %d)"

dbg: debug
	date +"%nCompiled DEBUG on: %A, %T (%Y %b %d)"

clang: $(SOURCES)
	$(COMPILER) -O2 $(WARNINGS) -o $(NAME) $(SOURCES) $(SDL)

debug: $(SOURCES)
	$(COMPILER) -g $(WARNINGS) -o $(NAME) $(SOURCES) $(SDL)

clean:
	rm ./$(NAME)


