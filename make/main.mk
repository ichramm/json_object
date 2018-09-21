###########################
# File:     main.mk
#
# Date:     Jan 09, 2015
#
# Author:   Tomás Kelly <tomas.kelly@intraway.com>
# Owner:    Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef MAIN.MK
MAIN.MK := INCLUDED

##
# Set the directoy for keeping scripts and utilities
##
SCRIPTS := $(MK)/scripts

##
# Load all necesary settings that need to be set before starting
##
include $(MK)/main-head.mk

##
# Determine Architecture and OS type
# These will be used for naming binary and library files
##
ARCH := $(shell uname -p)
OS := $(shell uname -s)

##
# Load configuration specific for Arch/OS, or the default one
##
ifneq ($(wildcard $(MK)/config-$(OS)-$(ARCH).mk),)
  include $(MK)/config-$(OS)-$(ARCH).mk
else
  include $(MK)/config-default.mk
endif

##
# Determine Verbose level, set it to OFF if not defined
# ON:   Full message all the time
# OFF:  Nice messages easy to read
##
VERBOSE ?= OFF
VERBOSEU := $(shell echo "$(VERBOSE)" | tr a-z A-Z)

ifneq ($(VERBOSEU),ON)
    VERBOSEU := OFF
endif

PRINT.ON :=
PRINT.OFF := @
PRINT = $(PRINT.$(VERBOSEU))

OUTPUT.ON :=
OUTPUT.OFF := &> /dev/null
OUTPUT = $(OUTPUT.$(VERBOSEU))

##
# Parse Gradle's version information
##
VERSION.CORE := 0
VERSION.MAJOR := 0
VERSION.MINOR := 0
VERSION.PATCH := 0

VERSION.STAGE := 4
VERSION.SEQUENCE := 0

ifneq ($(VERSION),)
   VERSION := $(strip $(VERSION))
else
   VERSION := $(strip $(shell git tag --points-at HEAD))
   ifeq ($(VERSION),)
      $(info Version missing (C.M.m.p). Using $(VERSION.CORE).$(VERSION.MAJOR).$(VERSION.MINOR).$(VERSION.PATCH)-nightly internally.)
      VERSION := $(VERSION.CORE).$(VERSION.MAJOR).$(VERSION.MINOR).$(VERSION.PATCH)
   endif
endif

VERSION.VALID := $(strip $(shell echo $(VERSION) | $(PERL) -pe 'if (/(\d+\.){2}\d+/) {s/.*\D((\d+\.){2}\d+)(-.*)?.*/\1\3/} else {$$_=""}'))

ifneq ($(VERSION.VALID),)
    VERSION.CORE := $(strip $(shell echo "$(VERSION)" | cut -d"-" -f 1 | cut -d"." -f 1))
    VERSION.MAJOR := $(strip $(shell echo "$(VERSION)" | cut -d"-" -f 1 | cut -d"." -f 2))
    VERSION.MINOR := $(strip $(shell echo "$(VERSION)" | cut -d"-" -f 1 | cut -d"." -f 3))
    VERSION.PATCH := $(strip $(shell echo "$(VERSION)" | cut -d"-" -f 1 | cut -d"." -f 4))

    VERSION.RELEASE := $(shell echo "$(VERSION)" | sed -n 's/.*-//p')

    ifneq ($(VERSION.RELEASE),)
        VERSION.STAGE := $(shell echo "$(VERSION.RELEASE)" | $(PERL) -pe 's/^([a-zA-Z]+).*/\1/')

        ifeq ($(VERSION.STAGE),nightly)
            VERSION.STAGE := 0
        else ifeq ($(VERSION.STAGE),SNAPSHOT)
            VERSION.STAGE := 0
        else ifeq ($(VERSION.STAGE),a)
            VERSION.STAGE := 1
        else ifeq ($(VERSION.STAGE),b)
            VERSION.STAGE := 2
        else ifeq ($(VERSION.STAGE),rc)
            VERSION.STAGE := 3
        else ifeq ($(VERSION.STAGE),)
            VERSION.STAGE := 4
        else
            $(error "Version's stage '$(VERSION.STAGE)' not recognized! Supported values 'nightly','a','b','rc','SNAPSHOT'.")
            VERSION.STAGE := 0
        endif

        VERSION.SEQUENCE := $(shell echo "$(VERSION.RELEASE)" | $(PERL) -pe 's/[^\d]//g')
        ifeq ($(VERSION.SEQUENCE),)
            VERSION.SEQUENCE := 0
        endif
    endif
else
    $(info Version '$(VERSION)' invalid (C.M.m.p). Using $(VERSION.CORE).$(VERSION.MAJOR).$(VERSION.MINOR).$(VERSION.PATCH)-nightly internally.)
endif

ifneq ($(VERSION.VALID),)
    VERSION.SHORT := $(VERSION.CORE).$(VERSION.MAJOR)
    VERSION.LONG := $(VERSION.CORE).$(VERSION.MAJOR).$(VERSION.MINOR)
    ifneq ($(VERSION.PATCH),)
        VERSION.LONG := $(VERSION.LONG).$(VERSION.PATCH)
    endif
else
    VERSION.SHORT := $(patsubst %-SNAPSHOT,%,$(VERSION))
    VERSION.LONG := $(VERSION)
endif

##
# Now that all VERSION-related variables have been set, I can finally set
# a default value for the fourth number
##
ifeq ($(VERSION.PATCH),)
    VERSION.PATCH := 0
endif

##
# Backward Compatibility Variables (To Be Removed)
##
GRADLE.VERSION := $(VERSION)
GRADLE.VERSION.VALID := $(VERSION.VALID)

GRADLE.VERSION.CORE := $(VERSION.CORE)
GRADLE.VERSION.MAJOR := $(VERSION.MAJOR)
GRADLE.VERSION.MINOR := $(VERSION.MINOR)
GRADLE.VERSION.PATCH := $(VERSION.PATCH)

GRADLE.VERSION.STAGE := $(VERSION.STAGE)
GRADLE.VERSION.SEQUENCE := $(VERSION.SEQUENCE)

##
# Load configuration specific for Arch/OS, or the default one
##
BUILDID := $(strip $(shell git rev-list --first-parent HEAD 2> /dev/null | wc -l || svn info | $(GREP) 'Last Changed Rev' | $(PERL) -pe 's/(\d+)/\1/'))

##
# Load necesary files for setting the build system
##
include $(MK)/defines.mk

##
# Load build configuration specific for Arch/OS, or the default one
##
ifneq ($(wildcard $(MK)/builds-$(OS)-$(ARCH).mk),)
  include $(MK)/builds-$(OS)-$(ARCH).mk
else
  include $(MK)/builds-default.mk
endif

##
# Keep Generated Source Files
##
.PRECIOUS: %.c %.cpp %.h %.hpp %.o
.SECONDARY: $(wildcard $(GENDIR.base)/*.c) $(wildcard $(GENDIR.base)/*.cpp) $(wildcard $(GENDIR.base)/*.h) $(wildcard $(GENDIR.base)/*.hpp)

##
# Load necesary files for setting the build system
##
include $(MK)/targets.mk
include $(MK)/tools.mk

##
# Load all necesary settings that need to be set after everything else has been set
##
include $(MK)/main-tail.mk

endif
