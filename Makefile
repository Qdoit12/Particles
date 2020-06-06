run:
	g++ main.cpp -std=c++11 -O0 -Wall -Wshadow -Wpedantic -Wextra -fsanitize=undefined -fsanitize=address -lsfml-graphics -lsfml-window -lsfml-system -o main && ./main
build:
	g++ main.cpp -std=c++11 -O0 -Wall -Wshadow -Wpedantic -Wextra -fsanitize=undefined -fsanitize=address -lsfml-graphics -lsfml-window -lsfml-system -o main
