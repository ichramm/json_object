#
# Makefile for json_object
#
PROJECT_NAME     =json_object
PROJECT_VERSION ?=2

Target ?= Release

LIB_NAME:=$(PROJECT_NAME)

INCLUDE_DIR       ?=/usr/local/include
LIB_DIR           ?=/usr/local/lib

BOOST_VERSION ?=1_55
BOOST_DIR     ?=$(INCLUDE_DIR)/boost-$(BOOST_VERSION)
BOOST_LIB     ?=$(LIB_DIR)

CPPC   ?= g++
CC     ?= gcc
LINKER ?= g++

GCC_VERSION          ?= $(shell expr substr "`$(CPPC) -dumpversion | tr -dc '[0-9]'`" 1 2)

ROOT_DIR   =.
SRC_DIR    = $(ROOT_DIR)/src
BUILD_DIR  = $(ROOT_DIR)/build
OUTPUT_DIR = $(BUILD_DIR)/lib/$(Target)
OBJS_ROOT  = $(BUILD_DIR)/.obj


OBJS_DIR = $(OBJS_ROOT)/$(Target)
OBJS     = $(OBJS_DIR)/json_object.o \
           $(OBJS_DIR)/libjson.o \
	   $(OBJS_DIR)/json_diffcheck.o \
	   $(OBJS_DIR)/encodings.o \
	   $(OBJS_DIR)/json_io.o

CFLAGS  :=$(CFLAGS) -Wall -Wextra -pedantic -fPIC -g -c
LDFLAGS :=$(LDFLAGS) -Wall -fPIC -g -shared
INCLUDES = -I$(ROOT_DIR)/include -I$(BOOST_DIR)

ifeq (Debug, $(findstring Debug,$(Target)))
	CFLAGS   :=$(CFLAGS) -D_DEBUG
else
	CFLAGS :=$(CFLAGS) -O2
endif

# TODO: Check for GCC >= 4.7 and compile C++11 by default (at least in v2)
#--std=c++11
CXXFLAGS =$(CFLAGS)

OUTPUT_LIB  =lib$(LIB_NAME).so
OUTPUT_FILE =$(OUTPUT_DIR)/$(OUTPUT_LIB)
LDFLAGS    :=$(LDFLAGS) -Wl,-soname,$(OUTPUT_LIB)

CPPCOMPILE =$(CPPC) $(CXXFLAGS) "$<" -o "$(OBJS_DIR)/$(*F).o" $(INCLUDES)
CCOMPILE   =$(CC) $(CFLAGS) "$<" -o "$(OBJS_DIR)/$(*F).o" $(INCLUDES)
LINK       =$(LINKER) $(LDFLAGS) -o "$(OUTPUT_FILE)" $(OBJS)

build : $(OUTPUT_FILE)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPPCOMPILE)
$(OBJS_DIR)/%.o: $(SRC_DIR)/detail/%.cpp
	$(CPPCOMPILE)
$(OBJS_DIR)/libjson.o: $(SRC_DIR)/detail/libjson/json.c
	$(CCOMPILE) -Wno-extra -Wno-variadic-macros

$(OUTPUT_FILE): $(OBJS_DIR) $(OUTPUT_DIR) $(OBJS)
	$(LINK)
	$(POST_BUILD_EVENTS)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(BUILD_DIR)
