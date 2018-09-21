###########################
# File: 	test.mk.in
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

##
# Target name
##
TGT := Object

##
# Target Specific Prerequisites - Required Library (.a|.so)
##
TGT.PREREQS.LIBS := json_object

##
# Target Specific Prerequisites - Required Targets
##
TGT.PREREQS.TGTS :=

##
# Target Specific - Generated Source Files
##
TGT.GENRAWS := $(shell find $(RDIR) -type f -name "*.raw" -print | sort | sed 's/^\.\///')
TGT.GENSRCS := $(addprefix $(GENDIR.base)/, $(patsubst %.raw, %.cpp, $(subst src/,,$(TGT.GENRAWS))))

##
# Target Specific - C Files/Flags (Build Mode Specific Flags Are Appended After TGT.CFLAGS)
##
TGT.CFLAGS :=
TGT.CFLAGS.analysis :=
TGT.CFLAGS.coverage :=
TGT.CFLAGS.debug :=
TGT.CFLAGS.profile :=
TGT.CFLAGS.release :=

TGT.CSRCS := $(shell find $(RDIR) -type f -name "*.c" -print | sort | $(SED) 's/^\.\///')

##
# Target Specific - C++ Files/Flags (Build Mode Specific Flags Are Appended After TGT.CXXFLAGS)
##
TGT.CXXFLAGS := -Wno-sign-compare
TGT.CXXFLAGS.analysis :=
TGT.CXXFLAGS.coverage :=
TGT.CXXFLAGS.debug :=
TGT.CXXFLAGS.profile :=
TGT.CXXFLAGS.release :=

TGT.CXXSRCS := $(RDIR)/Object.cpp

##
# Target Specific - Linker Flags (Build Mode Specific Flags Are Appended After TGT.LDFLAGS)
##
TGT.LDFLAGS := -ljson_object -lgtest -lgtest_main -lpthread
TGT.LDFLAGS.analysis :=
TGT.LDFLAGS.coverage :=
TGT.LDFLAGS.debug :=
TGT.LDFLAGS.profile :=
TGT.LDFLAGS.release :=

###########################
# Additional Target-Specific Rules
###########################

##
# Recipes specified MUST used local variables:
# CTGT: Current Target (Including Directory)
# CBIN: Current Binary (Excluding Directory)
# CDIR: Current Directory
##

###########################
# Run (For Check)
###########################
$(RDIR)/$(TGT)-run:
	$(call MAGENTA,"File: $(BUILDDIR.base)/$(CDIR)/$(CBIN)-$(BUILD)")
	@$(MKDIR) $(LOGDIR.base)/$(dir $(CDIR)/$(CBIN))
	@$(BUILDDIR.base)/$(CDIR)/$(CBIN)-$(BUILD) --gtest_output="xml:$(LOGDIR.base)/$(dir $(CDIR)/$(CBIN))/$(notdir $(CDIR)/$(CBIN)).xml"
