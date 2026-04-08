SOURCES= \
		test.cpp\
		mandelbrot.cpp\
		\
		visuals/visuals.cpp\
		\
		common/buffer/buffer.cpp\
		common/etc/tools.cpp\
		\
		core/core.cpp\
		\
		core/core_avx_3.cpp
		# core/core_compiler.cpp
		# core/openCL/core_gpu.cpp
		# core/simple_core.cpp
	


INCLUDES_DIR = \
	       visuals/\
	       core/\
	       common/buffer/\
	       common/etc/\

OBJ_DIR = obj
SOURCE_DIR = src

INCLUDES = $(addprefix -I$(SOURCE_DIR)/, $(INCLUDES_DIR))

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
SOURCES := $(addprefix $(SOURCE_DIR)/, $(SOURCES))
TARGET = mandelbrot.out

# c++/c compiler options
CC = g++
CFLAGS =  -D _DEBUG -ggdb3 -std=c++17 -O2 -march=native 
CFLAGS += $(INCLUDES)
CFLAGS += -lsfml-graphics -lsfml-window -lsfml-system -lOpenCL 
# CFLAGS += -fopt-info-vec-missed


$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling" $<
	@$(CC) $(CFLAGS) -c $< -o $@ 	
	@echo "Compiled Successfully" $<

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Linked Successfully"

all: $(TARGET)
	@./$(TARGET)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(TARGET)
	@echo "Cleaned Successfully"
