
CC=g++
CFLAGS =  -g -Wall -std=c++11
LFLAGS = -L/usr/lib

all: rest

rest: bin/Restaurant.o bin/Action.o bin/Table.o bin/Customer.o bin/Dish.o bin/main.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	$(CC)   -o bin/rest bin/Restaurant.o bin/Action.o bin/Table.o bin/Customer.o bin/Dish.o bin/main.o $(LFLAGS)
	@echo 'Finished building target: rest'
	@echo ' '

bin/main.o: src/main.cpp src/Restaurant.cpp	
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp
	
bin/Restaurant.o: src/Restaurant.cpp include/Restaurant.h  src/Action.cpp src/Table.cpp src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

bin/Action.o: src/Action.cpp include/Action.h src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Table.o: src/Table.cpp include/Table.h src/Customer.cpp src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp

bin/Customer.o: src/Customer.cpp include/Customer.h src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Dish.o: src/Dish.cpp include/Dish.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp

clean:
	rm -f bin/*


