###########################
# File: 	builds.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef BUILDS.MK
BUILDS.MK := INCLUDED

##
# Configure gradle directory, where all wrapper files will be
##
GRADLEDIR.base := $(ROOT)/gradle

##
# Configure make directory, where all mk files will be
##
MAKEDIR.base := $(ROOT)/make

##
# Configure include directory, where all headers will be
##
INCDIR.base := $(ROOT)/include

##
# Configure base output directories, where all targets will use
##
BUILDDIR ?= $(ROOT)/build
BUILDDIR.base := $(BUILDDIR)

BINDIR.base := $(BUILDDIR.base)/bin
DISTDIR.base := $(BUILDDIR.base)/dist
GENDIR.base := $(BUILDDIR.base)/gen
LIBDIR.base := $(BUILDDIR.base)/lib
LOGDIR.base := $(BUILDDIR.base)/logs
OBJDIR.base := $(BUILDDIR.base)/.objs
PKGDIR.base := $(BUILDDIR.base)/package
PKGDBGDIR.base := $(BUILDDIR.base)/package-dbg
RPMDIR.base := $(BUILDDIR.base)/rpm
SRCDIR.base := $(BUILDDIR.base)/src
TARDIR.base := $(BUILDDIR.base)/tar

##
# Define directories in which external headers and libraries will be placed
##
LIBSDIR.base := $(ROOT)/libs
INCDIR.libs := $(LIBSDIR.base)/include
LIBDIR.libs := $(LIBSDIR.base)/lib

##
# Determine if the default output directory will be used, or an overrie has been specified
##
ifdef OUTDIR
OUTDIR.base := $(OUTDIR)
LOGDIR.base := $(OUTDIR.base)/logs
else
OUTDIR.base := $(ROOT)
endif

##
# Set the default build mode that will be used, if no override was specified
##
BUILD ?= debug

##
# Configure common Compilation/Linking flags
##
CPPFLAGS.base :=
CFLAGS.base := -Wall -Wextra -Wfatal-errors -Wconversion -Wno-long-long -Wno-variadic-macros -Wunused -Wuninitialized -funwind-tables -I$(INCDIR.libs) -I$(INCDIR.base) -I$(GENDIR.base)
CXXFLAGS.base := -Wall -Wextra -Wfatal-errors -Wconversion -Wno-long-long -Wno-variadic-macros -Wnon-virtual-dtor -Wunused -Wuninitialized -funwind-tables -I$(INCDIR.libs) -I$(INCDIR.base) -I$(GENDIR.base)
LDFLAGS.base := -rdynamic -L$(LIBDIR.libs) -L$(LIBDIR.base)

##
# Configure Compilation/Linking flags based on the type of target file
##
CFLAGS.binary :=
CFLAGS.shared := -fpic -fPIC
CFLAGS.static :=

CXXFLAGS.binary :=
CXXFLAGS.shared := -fpic -fPIC
CXXFLAGS.static :=

##
# Configure common Target Build Architecture
##
BUILD.ARCH := $(OS)-$(ARCH)-$(shell basename $(CXX))-$(shell $(CXX) -dumpversion)

##
# Load all build modes settings
##
include $(sort $(wildcard $(MK)/build-*.mk))

endif
