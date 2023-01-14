all:
	@echo "use either 'make posix' or 'make winapi' depending on the technology you are using for developing the program"

posix:
	@gcc prog.c -o prog -lpthread

winapi:
	@winegcc prog.c -o prog 
