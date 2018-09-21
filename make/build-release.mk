###########################
# File: 	build-release.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef BUILD_RELEASE.MK
BUILD_RELEASE.MK := INCLUDED

##
# Optimize options (https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
ifeq ($(DONTOPTIMIZE),true)
	OPTIMIZE_LEVEL=1
	DBGLEVEL=3
else
	OPTIMIZE_LEVEL=2
	DBGLEVEL=1
endif

# Overrides setting from DONTOPTIMIZE
ifeq ($(KEEPSYMBOLS),true)
	DBGLEVEL=3
endif

##
# Configure output directories
##
BINDIR.release := $(BINDIR.base)/release
LIBDIR.release := $(LIBDIR.base)/release
OBJDIR.release := $(OBJDIR.base)/release
OUTDIR.release := $(OUTDIR.base)/release

##
# Configure Compilation/Linking flags
# Note: -g is ignored by GNU ld (http://linux.die.net/man/1/ld)
##
CPPFLAGS.release :=
CFLAGS.release := -O$(OPTIMIZE_LEVEL) -g$(DBGLEVEL) $(CFLAGS.base) $(CFLAGS)
CXXFLAGS.release := -O$(OPTIMIZE_LEVEL) -g$(DBGLEVEL) $(CXXFLAGS.base) $(CXXFLAGS)
LDFLAGS.release := $(LDFLAGS) -g$(DBGLEVEL) -L$(LIBDIR.release) -L$(LIBDIR.libs)/release $(LDFLAGS.base)

##
# Declare Release Rules
##
$(eval $(call CMDS.o, ${OBJDIR.release}, %.c, $${CMDS.c}))
$(eval $(call CMDS.o, ${OBJDIR.release}, %.cpp, $${CMDS.cpp}))

##
# Addditional commands to be executed after the target is built in release mode
# ${1} = $(CDIR)
# ${2} = $(CTGT)
##
define CMDS.release
endef

endif
