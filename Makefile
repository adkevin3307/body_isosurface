EXE = main
OBJ_DIR = obj
TRASH = .cache imgui.ini

SOURCES = $(wildcard src/*.cpp)
SOURCES += $(wildcard src/imgui/*.cpp)

OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS = -std=c++17 -Wall -O3 -Iinclude -Iinclude/imgui

LIBS = -Llib -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW

all: create_object_directory $(EXE)
	@echo Compile Success

create_object_directory:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(EXE) $(OBJ_DIR) $(TRASH)
