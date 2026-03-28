all: clean
	g++ *.cc -lraylib -lm -g -fsanitize=address -o main  ;./main
main: clean
	g++ *.cc -lraylib -lm -o main ; ./main
clean:
	rm -rf main
