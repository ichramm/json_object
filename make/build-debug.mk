###########################
# File: 	build-debug.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef BUILD_DEBUG.MK
BUILD_DEBUG.MK := INCLUDED

##
# Configure output directories
##
BINDIR.debug := $(BINDIR.base)/debug
LIBDIR.debug := $(LIBDIR.base)/debug
OBJDIR.debug := $(OBJDIR.base)/debug
OUTDIR.debug := $(OUTDIR.base)/debug

##
# Configure Compilation/Linking flags
##
CPPFLAGS.debug :=
CFLAGS.debug := -O0 -g3 $(CFLAGS.base) $(CFLAGS)    
CXXFLAGS.debug := -O0 -g3 $(CXXFLAGS.base) $(CXXFLAGS)
LDFLAGS.debug := $(LDFLAGS) -L$(LIBDIR.debug) -L$(LIBDIR.libs)/debug $(LDFLAGS.base)

##
# Declare Debug Rules
##
$(eval $(call CMDS.o, ${OBJDIR.debug}, %.c, $${CMDS.c}))
$(eval $(call CMDS.o, ${OBJDIR.debug}, %.cpp, $${CMDS.cpp}))

##
# Addditional commands to be executed after the target is built in debug mode
# ${1} = $(CDIR)
# ${2} = $(CTGT)
##
define CMDS.debug
endef

endif
