# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Output executable
TARGET = cspGen

# Source and object files
SRC = main.c \
      html_fetch.c \
      model/html_struct/html_struct.c \
      model/script_struct/script_struct.c \
      extractor/tag_extract.c \
      extractor/extract_relevant_tags_and_pre_filter.c \
      extractor/populate_script_struct.c \
      extractor/scripts_attribute_extract.c \
      extractor/script_inline_is_unsafe.c \

OBJ = $(SRC:.c=.o)

# Default target: build everything
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
# Compiler and flags
CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -std=c99 -g -O0 -fno-omit-frame-pointer
# Auto-deps for headers; safe even if no headers are used
CFLAGS  += -MMD -MP

# Optional sanitizers: enable with `make ASAN=1`
ifeq ($(ASAN),1)
CFLAGS  += -fsanitize=address,undefined
LDFLAGS += -fsanitize=address,undefined
endif

# Output executable
TARGET := cspGen

# Source files (explicit list kept)
SRC := main.c \
       html_fetch.c \
       model/html_struct/html_struct.c \
       model/script_struct/script_struct.c \
       extractor/tag_extract.c \
       extractor/extract_relevant_tags_and_pre_filter.c \
       extractor/populate_script_struct.c \
       extractor/scripts_attribute_extract.c \
       extractor/script_inline_is_unsafe.c

# Object and dep files
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

.PHONY: all clean rebuild run printvars

# Default target: build everything
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

# Compile with pattern rule (generates .o and .d via -MMD -MP)
%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Include auto-generated dependency files
-include $(DEP)

# Force a thorough rebuild
rebuild: clean all

# Run the binary (useful sanity check)
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

# Debug helper: print key vars
printvars:
	@echo CC=$(CC)
	@echo CFLAGS=$(CFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo TARGET=$(TARGET)
	@echo OBJ=$(OBJ)
	@echo DEP=$(DEP)