###########################
# File: 	build-coverage.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef BUILD_COVERAGE.MK
BUILD_COVERAGE.MK := INCLUDED

##
# Configure output directories
##
BINDIR.coverage := $(BINDIR.base)/coverage
LIBDIR.coverage := $(LIBDIR.base)/coverage
OBJDIR.coverage := $(OBJDIR.base)/coverage
OUTDIR.coverage := $(OUTDIR.base)/coverage

##
# Configure Compilation/Linking flags
##
CPPFLAGS.coverage :=
CFLAGS.coverage := -O0 -g0 $(CFLAGS.base) -fprofile-arcs -ftest-coverage --coverage $(CFLAGS)
CXXFLAGS.coverage := -O0 -g0 $(CXXFLAGS.base) -fprofile-arcs -ftest-coverage --coverage $(CXXFLAGS)
LDFLAGS.coverage := $(LDFLAGS) -L$(LIBDIR.coverage) -L$(LIBDIR.libs)/coverage -lgcov --coverage $(LDFLAGS.base)

##
# Declare Coverage Rules
##
$(eval $(call CMDS.o, ${OBJDIR.coverage}, %.c, $${CMDS.c}))
$(eval $(call CMDS.o, ${OBJDIR.coverage}, %.cpp, $${CMDS.cpp}))

##
# Addditional commands to be executed after the target is built in coverage mode
# ${1} = $(CDIR)
# ${2} = $(CTGT)
##
define CMDS.coverage

ifneq (${${2}.NOCOVERAGE},true)
	$(PRINT)lcov --zerocounters --quiet --directory $(OBJDIR.coverage)/${1} --base-directory ${1}
	$(PRINT)lcov --capture -initial --directory $(OBJDIR.coverage)/${1} --base-directory ${1} --output-file $(OBJDIR.coverage)/${2}-base.lcov
endif
endef

##
# Addditional commands to be executed after the target is built in coverage-report mode
# ${1} = $(CDIR)
# ${2} = $(CTGT)
##
define CMDS.coverage-report
	$(PRINT)if [ "${${2}.NOCOVERAGE}" != "true" ]; then \
		lcov --capture --directory $(OBJDIR.coverage)/${1} --base-directory ${1} --output-file $(OBJDIR.coverage)/${2}-run.lcov || true; \
	fi
endef

endif
