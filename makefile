# makefile for SNAKE

NAME = snake

WARNINGS = -Wall -Wextra -Winline -Werror
COMPILER = clang++-3.8 -std=c++1y -O3

GAME_SRC = main.cpp game/*.cpp
CORE_SRC = core/*.cpp
SOURCES = $(CORE_SRC) $(GAME_SRC)

SDL = -lSDL2 -lSDL2_image -lSDL2_ttf


clang: $(SOURCES)
	$(COMPILER) $(WARNINGS) -o $(NAME) $(SOURCES) $(SDL)

clean:
	rm ./$(NAME)


