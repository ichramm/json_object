###########################
# File: 	build-profile.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef BUILD_PROFILE.MK
BUILD_PROFILE.MK := INCLUDED

##
# Configure output directories
##
BINDIR.profile := $(BINDIR.base)/profile
LIBDIR.profile := $(LIBDIR.base)/profile
OBJDIR.profile := $(OBJDIR.base)/profile
OUTDIR.profile := $(OUTDIR.base)/profile

##
# Configure Compilation/Linking flags
##
CPPFLAGS.profile :=
CFLAGS.profile := -O0 -g0 $(CFLAGS.base)
CXXFLAGS.profile := -O0 -g0 $(CXXFLAGS.base)
LDFLAGS.profile := $(LDFLAGS) -L$(LIBDIR.profile) -L$(LIBDIR.libs)/profile -lprofiler -ltcmalloc $(LDFLAGS.base)

##
# Declare Profile Rules
##
$(eval $(call CMDS.o, ${OBJDIR.profile}, %.c, $${CMDS.c}))
$(eval $(call CMDS.o, ${OBJDIR.profile}, %.cpp, $${CMDS.cpp}))

##
# Addditional commands to be executed after the target is built in profile mode
# ${1} = $(CDIR)
# ${2} = $(CTGT)
##
define CMDS.profile
endef

##
# Addditional commands to be executed after the target is built in profile-report mode
# ${1} = $(CDIR)
# ${2} = $(CTGT)
##
define CMDS.profile-report
endef

endif
