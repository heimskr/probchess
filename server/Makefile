COMPILER	?= clang++
OUTPUT		:= chess
SOURCES		:= $(shell find src/*.cpp src/**/*.cpp)
OBJECTS		:= $(SOURCES:.cpp=.o)
CFLAGS		?= -Iinclude $(shell pkg-config --cflags boost openssl)
LDFLAGS		?= $(shell pkg-config --libs boost openssl)

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

test: $(OUTPUT)
	./$(OUTPUT) 38555

%.o: %.cpp
	$(COMPILER) $(CFLAGS) -std=c++17 -c $< -o $@

clean:
	rm -f src/*.o src/**/*.o $(OUTPUT)

DEPFILE  = .dep
DEPTOKEN = "\# MAKEDEPENDS"
DEPFLAGS = -f $(DEPFILE) -s $(DEPTOKEN)

depend:
	@ echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(COMPILER) $(CFLAGS) -- $(SOURCES) 2>/dev/null
	@ rm $(DEPFILE).bak

sinclude $(DEPFILE)
