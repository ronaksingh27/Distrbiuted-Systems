CXX = gcc

CXXFLAGs = -Wall -Wextra -O2 -g

SOURCES = client.c server.c

OBJECTS = $(SOURCES: .c = .o )

EXEC = server client

all : $(EXEC)


server : server.o
	$(CXX) $(CXXFLAGS) -o server server.o

client : client.o
	$(CXX) $(CXXFLAGS) -o client client.o

clean :
	rm -f *.o $(EXEC)