CXX = g++
CXXFLAGS = -Wall -Wextra -I.

SOURCES = main.cpp JogoDeNim.cpp

OBJECTS = $(SOURCES:.cpp=.o)


EXECUTABLE = JogoDeNim

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean
