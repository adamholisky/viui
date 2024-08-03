#Make config things
.DEFAULT_GOAL := all
SHELL := /bin/bash -O globstar
COLOR_RED = "\033[0;31m"
COLOR_GREEN = "\033[0;32m"
COLOR_BLUE = "\033[0;34m"
COLOR_END = "\033[0m"
START_OF_LINE = "\r"
MF_COMPILE = $(COLOR_BLUE)[COMPILE]$(COLOR_END)
MF_FAIL    = $(START_OF_LINE)$(COLOR_RED)[ ERROR ]$(COLOR_END)
MF_SUCCESS = $(START_OF_LINE)$(COLOR_GREEN)[SUCCESS]$(COLOR_END)

# Directory structure
ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SOURCE_DIR_NAME := src
HEADER_DIR_NAME := include
BUILD_DIR_NAME := build
SOURCE_DIR := $(ROOT_DIR)/$(SOURCE_DIR_NAME)
HEADER_DIR := $(ROOT_DIR)/$(HEADER_DIR_NAME)
BUILD_DIR := $(ROOT_DIR)/$(BUILD_DIR_NAME)

# Basic variables
EXE_NAME := vit
SOURCES_C := $(shell ls $(SOURCE_DIR_NAME)/**/*.c )
HEADERS := $(shell ls $(HEADER_DIR_NAME)/**/*.h )
OBJECTS_C := $(patsubst %.c, $(BUILD_DIR_NAME)/%.o, $(shell echo $(SOURCES_C) | xargs -n 1 basename))

# Path setup
VPATH = $(shell find $(SOURCE_DIR) -type d -printf "$(SOURCE_DIR_NAME)/%P:")

# Compiler control
CC := /usr/bin/gcc
LD := /usr/bin/ld
DEFINES := -DVI_ENV_DEV
CFLAGS := -g -O0 -I$(HEADER_DIR)
LDFLAGS := 

# Helpers

mfdebug:
	@echo "Debug Info"
	@echo " "
	@echo "ROOT_DIR:   " $(ROOT_DIR)
	@echo "SOURCE_DIR: " $(SOURCE_DIR)
	@echo "HEADER_DIR: " $(HEADER_DIR)
	@echo "BUILD_DIR:  " $(BUILD_DIR)
	@echo "VPATH:      " $(VPATH)
	@echo " "
	@echo "EXE_NAME:   " $(EXE_NAME)
	@echo "SOURCES_C:  " $(SOURCES_C)
	@echo "OBJECTS_C:  " $(OBJECTS_C)
	@echo "HEADERS:    " $(HEADERS)
	@echo " "
	@echo "CC:         " $(CC)
	@echo "LD:         " $(LD)
	@echo "CFLAGS:     " $(CFLAGS)
	@echo "LDFLAGS:    " $(LDFLAGS)
	@echo "DEFINES:    " $(DEFINES)

# Basic 
all: $(EXE_NAME)

run: $(EXE_NAME)
	./$(EXE_NAME)

clean:
	@$(eval MF_ACTION := $(COLOR_BLUE)Cleaning $(COLOR_END))
	@echo -n -e $(MF_ACTION)
	@rm -rf $(BUILD_DIR)/*.o
	@rm -rf $(EXE_NAME)
	@echo -e $(MF_SUCCESS) $(MF_ACTION)

# Specifics

$(EXE_NAME): $(OBJECTS_C)
	@$(eval MF_ACTION := $(COLOR_BLUE)Building $(EXE_NAME)$(COLOR_END))
	@echo -n -e $(MF_ACTION)
	@$(CC) $(CFLAGS) $(OBJECTS_C) -o $(EXE_NAME)
	@echo -e $(MF_SUCCESS) $(MF_ACTION)

build/%.o: %.c
	@$(eval MF_ACTION := $(COLOR_BLUE)Building $*.c$(COLOR_END))
	@echo -n -e $(MF_COMPILE) $(MF_ACTION)
	@$(CC) $(CFLAGS) -fdiagnostics-color=always -c $< -o $(BUILD_DIR_NAME)/$*.o &> /tmp/MF_OUT || (echo -e $(MF_FAIL) $(MF_ACTION) && cat /tmp/MF_OUT && exit 1)
	@echo -e $(MF_SUCCESS) $(MF_ACTION)

