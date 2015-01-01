CC=clang++ -std=c++11
default:
	@$(CC) main.cpp -o bin/main
	@./bin/main

