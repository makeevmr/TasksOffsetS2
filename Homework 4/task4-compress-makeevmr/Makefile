CC=g++
CFLAGS=-c -pedantic-errors -g
LDFLAGS=
SOURCES=compress.cpp algorithms/move_to_front.cpp algorithms/lz_77.cpp algorithms/huffman.cpp algorithms/DataStructures/DoublyLinkedList.cpp algorithms/DataStructures/MinHeap.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=compress

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
