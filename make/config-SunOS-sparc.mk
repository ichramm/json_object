###########################
# File: 	config-SunOS-sparc.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef CONFIG_SUNOS_SPARC.MK
CONFIG_SUNOS_SPARC.MK := INCLUDED

##
# Declare the compilers to be used, if they were not previously declared
##
CC ?= gcc
CXX ?= g++

##
# Declare common commands that will be used through the build system
##
CD := cd
CP := cp -rf
EGREP := egrep
GREP := grep
MKDIR := mkdir -p
MV := mv
PERL := perl
RM := rm -rf
RSYNC := rsync -aq
SED := sed
SHELL := /bin/bash
TAR := gtar -czf
TOUCH := touch
UNTAR := gtar -xzf

##
# Declare special commands for building static libraries
##
AR ?= ar
RANLIB ?= ranlib

##
# Colored output commends
##

#tput setaf {CODE}- Set foreground color, see color {CODE} below
#tput setab {CODE}- Set background color, see color {CODE} below
#tput sgr0 - Reset everything

#Code  Color
# 0    Black
# 1    Red
# 2    Green
# 3    Yellow
# 4    Blue
# 5    Magenta
# 6    Cyan
# 7    White

#
# Test whether tput works with an arbitrary color (won't work on dumb or null terminal)
TPUT_WORKS := $(shell tput setaf 4 &> /dev/null && echo yes || echo no)

define BLUE
      @if [ "$(VERBOSEU)" = "OFF" ]; then \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 4; fi; \
          echo -e "$1"; \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
      fi
endef

define CYAN
      @if [ "$(VERBOSEU)" = "OFF" ]; then \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 6; fi; \
          echo -e "$1"; \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
      fi
endef

define GREEN
      @if [ "$(VERBOSEU)" = "OFF" ]; then \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 2; fi; \
          echo -e "$1"; \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
      fi
endef

define MAGENTA
      @if [ "$(VERBOSEU)" = "OFF" ]; then \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 5; fi; \
          echo -e "$1"; \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
      fi
endef

define RED
      @if [ "$(VERBOSEU)" = "OFF" ]; then \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 1; fi; \
          echo -e "$1"; \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
      fi
endef

define YELLOW
      @if [ "$(VERBOSEU)" = "OFF" ]; then \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 3; fi; \
          echo -e "$1"; \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
      fi
endef

define WHITE
      @if [ "$(VERBOSEU)" = "OFF" ]; then \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput setaf 7; fi; \
          echo -e "$1"; \
          if [ "$(TPUT_WORKS)" = "yes" ]; then tput sgr0; fi; \
      fi
endef

endif
