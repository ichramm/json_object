###########################
# File: 	build-analysis.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef BUILD_ANALYSIS.MK
BUILD_ANALYSIS.MK := INCLUDED

##
# Configure output directories
##
BINDIR.analysis := $(BINDIR.base)/analysis
LIBDIR.analysis := $(LIBDIR.base)/analysis
OBJDIR.analysis := $(OBJDIR.base)/analysis
OUTDIR.analysis := $(OUTDIR.base)/analysis

##
# Configure Compilation/Linking flags
##
CPPFLAGS.analysis :=
CFLAGS.analysis := -O0 -g0 $(CFLAGS.base) $(CFLAGS)   
CXXFLAGS.analysis := -O0 -g0 $(CXXFLAGS.base) $(CXXFLAGS)
LDFLAGS.analysis := $(LDFLAGS) -L$(LIBDIR.analysis) -L$(LIBDIR.libs)/analysis $(LDFLAGS.base)

##
# Declare Analysis Rules
##
$(eval $(call CMDS.o, ${OBJDIR.analysis}, %.c, $${CMDS.c}))
$(eval $(call CMDS.o, ${OBJDIR.analysis}, %.cpp, $${CMDS.cpp}))

##
# Addditional commands to be executed after the target is built in analysis mode
# ${1} = $(CDIR)
# ${2} = $(CTGT)
##
define CMDS.analysis
endef

endif
