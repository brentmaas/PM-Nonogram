all:
	@g++ -o Assignment3 src/maas3.cc -std=c++17 -Wall -Wextra

clean:
	@rm Assignment3.exe

run:
	@./Assignment3.exe