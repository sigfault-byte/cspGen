# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Output executable
TARGET = cspGen

# Source and object files
SRC = main.c \
      html_fetch.c \
      model/html_struct/html_struct.c \
      model/script_struct/script_struct.c\
      extractor/tag_extract.c \
      extractor/extract_relevant_tags_and_pre_filter.c \
      extractor/populate_script_struct.c\
      extractor/scripts_attribute_extract.c\
      extractor/script_inline_is_unsafe.c\

OBJ = $(SRC:.c=.o)

# Default target: build everything
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)