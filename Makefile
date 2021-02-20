main: main.c Makefile
	${CC} -o main  main.c 

    #${CC} -o main -Wno-cpp	 main.c 
clean:
	rm main
