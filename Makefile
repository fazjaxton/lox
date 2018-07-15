SOURCES:=	main.cpp \
			error.cpp \
			scanner.cpp \
			token.cpp \
		
OBJECTS:=$(SOURCES:%.cpp=%.o)

CPP_FLAGS:= -std=c++11 -Wall

lox: $(OBJECTS)
	g++ -o $@ $^

%.o: %.cpp
	g++ $(CPP_FLAGS) -c -o $@ $<
