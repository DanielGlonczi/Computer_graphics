CC = gcc

CFLAGS = -Wall -std=c99

OBJ = main.o cuboid.o
TARGET = cuboids.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c cuboid.h
	$(CC) $(CFLAGS) -c main.c

cuboid.o: cuboid.c cuboid.h
	$(CC) $(CFLAGS) -c cuboid.c

clean:
	del *.o $(TARGET)
