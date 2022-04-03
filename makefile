OBJS=main.o Administrator.o Buyers.o Calculator.o InfoManager.o MainWindow.o Sellers.o SqlHelper.o Users.o ChatRoom.o
CC=g++                
CFLAGS+= -c -Wall -g          
PRO=project

$(PRO): $(OBJS)               
	$(CC) $^ -o $@
%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@
run:
	./$(PRO)
clean:
	rm -f *.o $(PRO)
