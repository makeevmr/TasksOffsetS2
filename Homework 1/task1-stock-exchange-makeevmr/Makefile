CC=g++
CFLAGS=-c -pedantic-errors -g
LDFLAGS=
SOURCES=stockexchange.cpp StockDB.cpp request.cpp exceptions.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=stockexchange

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)