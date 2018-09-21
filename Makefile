###########################
# File: 	Makefile.in
#
# Date: 	Feb 03, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

# There are "reserved" variables that you should not modify:
# - $(ROOT) is the top level directory of the project tree
# - $(MK) is the directory where the included *.mk makefiles are

#######################################################
### Makefile Framework
#######################################################

##
# This variable is set by the C++ gradle plugin based on the
# property 'component', which is set in gradle.properties.
##
COMPONENT := $(strip $(COMPONENT))

##
# Makefile Template
#
# Reserved variables that MUST be set:
# - $(ROOT): Top level directory of the project tree, from where 'make' will be executed.
# - $(MK): Directory in which all the build system's *.mk files are located
##
ROOT := .
MK := $(shell readlink -f $(ROOT)/make)

##
# Uncomment this to disable compiler optimizations in release mode
# The default setting is to enable compiler optimizations up to level 2
##
#DONTOPTIMIZE=true

##
# Load the build system's configuration
##
include $(MK)/main.mk

##
# Set this variable if your package directory is different from the default.
# Default package directory: $(PKGDIR.base)/$(CTGT)-$($(CTGT).SHORTVERSION)
# Each module has its own package directory so setting this variable makes
# sense when your project puts multiple modules in the same package
#PACKAGEDIR=$(PKGDIR.base)/$(COMPONENT)-$(VERSION.SHORT)

##
# Specify the relative path to all the modules (including main module) that are to be loaded into the build system's
##
MODULES := $(shell find $(ROOT)/src -name \*.mk 2>/dev/null | sort)
$(foreach MODULE,$(MODULES),$(eval $(call CMDS.MODULE,$(MODULE))))

##
# Specify the relative path to all the examples that are to be loaded into the build system's
# By default, all tests within the 'example' directory will be loaded
##
EXAMPLES := $(shell find $(ROOT)/example -name \*.mk 2>/dev/null | sort)
$(foreach EXAMPLE,$(EXAMPLES),$(eval $(call CMDS.EXAMPLE,$(EXAMPLE))))

##
# Specify the relative path to all the tests that are to be loaded into the build system's
# By default, all tests within the 'test' directory will be loaded
##
TESTS := $(shell find $(ROOT)/test -name \*.mk 2>/dev/null | sort)
$(foreach TEST,$(TESTS),$(eval $(call CMDS.TEST,$(TEST))))
