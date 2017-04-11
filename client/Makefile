TARGET = client
OBJ = pthread_client.o key.o show.o tree.o

$(TARGET):$(OBJ)
	gcc -o client $(OBJ) -lpthread
%.o:%.c
	gcc -c $^
clean:
	rm *.o
	rm client
