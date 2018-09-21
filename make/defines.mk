###########################
# File: 	defines.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef DEFINES.MK
DEFINES.MK := INCLUDED

##
# For reference, here are some automatic variables defined by make.
# There are also their D/F variants e.g. $(<D) - check the manual.
#
# $@ - File name of the target of the rule
# $% - Target member name when the target is archive member
# $< - The name of the first dependency
# $? - The names of all dependencies that are newer then the target
# $^ - The names of all dependencies
##

#
# Global variable used when building dynamic or static libraries
# Possible values: STATIC|SHARED
LINK_TYPE ?= STATIC

# Specifies whether the linker supports build-id or not
LD_HAS_BUILD_ID=$(strip $(shell $(MK)/scripts/build-id-test.sh $(CXX)))

LD_BUILD_ID=
ifeq ($(LD_HAS_BUILD_ID),yes)
	LD_BUILD_ID=-Wl,--build-id=sha1
endif

##
# Declare compilation and linking commands
##
COMPILE.c = $(CC) $(CPPFLAGS.$(BUILD)) $(CFLAGS.$(BUILD)) $(CFLAGS.$(CTYPE)) $($(CTGT).CFLAGS) $($(CTGT).CFLAGS.$(BUILD)) -MMD
COMPILE.cpp = $(CXX) $(CPPFLAGS.$(BUILD)) $(CXXFLAGS.$(BUILD)) $(CXXFLAGS.$(CTYPE)) $($(CTGT).CXXFLAGS) $($(CTGT).CXXFLAGS.$(BUILD)) -MMD

ifeq ($(OS), Linux)
	LINK.bin    = $(CXX) $(LD_BUILD_ID) $$($$(CTGT).GENOBJS) $$($$(CTGT).COBJS.$$(BUILD)) $$($$(CTGT).CXXOBJS.$$(BUILD)) $$(LDFLAGS.$$(BUILD)) $$($$(CTGT).LDFLAGS) $$($$(CTGT).LDFLAGS.$$(BUILD)) $$($$(CTGT).LDSYMBOLS)
	LINK.shared = $(CXX) $(LD_BUILD_ID) $$($$(CTGT).GENOBJS) $$($$(CTGT).COBJS.$$(BUILD)) $$($$(CTGT).CXXOBJS.$$(BUILD)) $$(LDFLAGS.$$(BUILD)) $$($$(CTGT).LDFLAGS) $$($$(CTGT).LDFLAGS.$$(BUILD)) $$($$(CTGT).LDSYMBOLS) -shared -Wl,-soname,$$($$(CTGT).BIN)
	LINK.static = $(AR) rc $$(LIBDIR.$$(BUILD))/$$($$(CTGT).BIN) $$($$(CTGT).GENOBJS) $$($$(CTGT).COBJS.$$(BUILD)) $$($$(CTGT).CXXOBJS.$$(BUILD)) && $(RANLIB) $$(LIBDIR.$$(BUILD))/$$($$(CTGT).BIN)
else
	LINK.bin    = $(CXX) $(LD_BUILD_ID) $$($$(CTGT).GENOBJS) $$($$(CTGT).COBJS.$$(BUILD)) $$($$(CTGT).CXXOBJS.$$(BUILD)) $$(LDFLAGS.$$(BUILD)) $$($$(CTGT).LDFLAGS) $$($$(CTGT).LDFLAGS.$$(BUILD)) $$($$(CTGT).LDSYMBOLS)
	LINK.shared = $(CXX) $(LD_BUILD_ID) $$($$(CTGT).GENOBJS) $$($$(CTGT).COBJS.$$(BUILD)) $$($$(CTGT).CXXOBJS.$$(BUILD)) $$(LDFLAGS.$$(BUILD)) $$($$(CTGT).LDFLAGS) $$($$(CTGT).LDFLAGS.$$(BUILD)) $$($$(CTGT).LDSYMBOLS) -shared
	LINK.static = $(AR) rc $$(LIBDIR.$$(BUILD))/$$($$(CTGT).BIN) $$($$(CTGT).GENOBJS) $$($$(CTGT).COBJS.$$(BUILD)) $$($$(CTGT).CXXOBJS.$$(BUILD)) && $(RANLIB) $$(LIBDIR.$$(BUILD))/$$($$(CTGT).BIN)
endif

##
# Macro that will automaticaly set RDIR sections for each rule
##
define CMDS.RULES
include ${1}
endef

##
# Macro that will automaticaly set Head/Tail sections for each module
##
define CMDS.MODULE
include $(MK)/module-head.mk
RDIR := $(patsubst ./%,%,$(patsubst %/,%,$(dir ${1})))
SDIR := $(subst src/,,$(patsubst ./%,%,$(patsubst %/,%,$(dir ${1}))))
include ${1}
include $(MK)/module-tail.mk
endef

##
# Macro that will automaticaly set Head/Tail sections for each example
##
define CMDS.EXAMPLE
include $(MK)/example-head.mk
RDIR := $(patsubst ./%,%,$(patsubst %/,%,$(dir ${1})))
SDIR := $(subst example/,,$(patsubst ./%,%,$(patsubst %/,%,$(dir ${1}))))
include ${1}
include $(MK)/example-tail.mk
endef

##
# Macro that will automaticaly set Head/Tail sections for each test
##
define CMDS.TEST
include $(MK)/test-head.mk
RDIR := $(patsubst ./%,%,$(patsubst %/,%,$(dir ${1})))
SDIR := $(subst test/,,$(patsubst ./%,%,$(patsubst %/,%,$(dir ${1}))))
include ${1}
include $(MK)/test-tail.mk
endef

##
# Macro that specifies hoy a single *.c file is to be compiled
# After compilation has been done, the dependencies file is created and modified
# to remove /opt libraries and unnecesary dependencies
##
define CMDS.c
	@$(MKDIR) $(dir $@)
	$(call CYAN,"Compiling $@...")
	$(PRINT)$(COMPILE.c) -c $< -o $@ $(OUTPUT)
	@$(SED) -e 's|/opt/[^ ]*||' -e '/^  \\$$/ d' -e 's/^ //' < ${@:%$(suffix $@)=%.d} > ${@:%$(suffix $@)=%.d.tmp}; \
	$(MV) ${@:%$(suffix $@)=%.d.tmp} ${@:%$(suffix $@)=%.d}
endef

##
# Macro that specifies hoy a single *.cpp file is to be compiled
# After compilation has been done, the dependencies file is created and modified
# to remove /opt libraries and unnecesary dependencies
##
define CMDS.cpp
	@$(MKDIR) -p $(dir $@)
	$(call CYAN,"Compiling $@...")
	$(PRINT)$(COMPILE.cpp) -c $< -o $@ $(OUTPUT)
	@$(SED) -e 's|/opt/[^ ]*||' -e '/^  \\$$/ d' -e 's/^ //' < ${@:%$(suffix $@)=%.d} > ${@:%$(suffix $@)=%.d.tmp}; \
	$(MV) ${@:%$(suffix $@)=%.d.tmp} ${@:%$(suffix $@)=%.d}
endef

##
# Macro for setting prerequisites and recepies for object files
# ${1}: Object directory
# ${2}: Prerequisites
# ${3}: Rule
##
define CMDS.o
${1}/%.o: ${2}
	${3}
endef

##
# Declare Generated Source File Compilation Rules
##
#$(eval $(call CMDS.o, ${GENDIR.base}, %.c, $${CMDS.c}))
#$(eval $(call CMDS.o, ${GENDIR.base}, %.cpp, $${CMDS.cpp}))

##
# Macro for setting all necesary targets for building an executable
# ${1}: Target
# ${2}: Build mode
##
define CMDS.bin
.PHONY: ${1}-${2}
${1}-${2}: ./$(BINDIR.${2})/${1} | ./$(BINDIR.${2})/$(${1}.BIN)

./$(BINDIR.${2})/${1}: ./$(BINDIR.${2})/$(${1}.BIN)
./$(BINDIR.${2})/${1}:
	$(call CMDS.${2},${RDIR},${1})

./$(BINDIR.${2})/$(${1}.BIN): override BUILD := ${2}
./$(BINDIR.${2})/$(${1}.BIN): CTGT := ${1}
./$(BINDIR.${2})/$(${1}.BIN): CDIR := $(RDIR)
./$(BINDIR.${2})/$(${1}.BIN): CTYPE := binary
./$(BINDIR.${2})/$(${1}.BIN): $(if $(${1}.PREREQS.LIBS),$(addprefix ./$(LIBDIR.${2})/,$(${1}.PREREQS.LIBS)))
./$(BINDIR.${2})/$(${1}.BIN): $(if $(${1}.PREREQS.TGTS),$(${1}.PREREQS.TGTS))
./$(BINDIR.${2})/$(${1}.BIN): $(if $(${1}.GENSRCS),$(${1}.GENSRCS))
./$(BINDIR.${2})/$(${1}.BIN): $(if $(${1}.GENOBJS),$(${1}.GENOBJS))
./$(BINDIR.${2})/$(${1}.BIN): $(${1}.COBJS.${2})
./$(BINDIR.${2})/$(${1}.BIN): $(${1}.CXXOBJS.${2})
./$(BINDIR.${2})/$(${1}.BIN):
	@$(MKDIR) $(BINDIR.${2})
	$$(call BLUE,"Linking $(BINDIR.${2})/$$($$(CTGT).BIN)...")
	$(PRINT)$(LINK.bin) -o $$(BINDIR.${2})/$$($$(CTGT).BIN) $(OUTPUT)
	$$(call GREEN,"Symbolic linking $(BINDIR.${2})/$$($$(CTGT).LINK) -\> $(BINDIR.${2})/$$($$(CTGT).BIN)...")
	$(PRINT)cd $(BINDIR.${2}) && /bin/ln -sf $$($$(CTGT).BIN) $$($$(CTGT).LINK)
endef

##
# Macro for setting all necesary targets for building a static library
# ${1}: Target
# ${2}: Build mode
##
define CMDS.a
.PHONY: ${1}-${2}
${1}-${2}: ./$(LIBDIR.${2})/${1} | ./$(LIBDIR.${2})/$(${1}.BIN)

./$(LIBDIR.${2})/${1}: ./$(LIBDIR.${2})/$(${1}.BIN)
./$(LIBDIR.${2})/${1}:
	$(call CMDS.${2},${RDIR},${1})

./$(LIBDIR.${2})/$(${1}.LINK): ./$(LIBDIR.${2})/$(${1}.BIN)
./$(LIBDIR.${2})/$(${1}.LINK):
	$(call CMDS.${2},${RDIR},${1})

./$(LIBDIR.${2})/$(${1}.BIN): override BUILD := ${2}
./$(LIBDIR.${2})/$(${1}.BIN): CTGT := ${1}
./$(LIBDIR.${2})/$(${1}.BIN): CDIR := $(RDIR)
./$(LIBDIR.${2})/$(${1}.BIN): CTYPE := static
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.PREREQS.LIBS),$(addprefix ./$(LIBDIR.${2})/,$(${1}.PREREQS.LIBS)))
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.PREREQS.TGTS),$(${1}.PREREQS.TGTS))
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.GENSRCS),$(${1}.GENSRCS))
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.GENOBJS),$(${1}.GENOBJS))
./$(LIBDIR.${2})/$(${1}.BIN): $(${1}.COBJS.${2})
./$(LIBDIR.${2})/$(${1}.BIN): $(${1}.CXXOBJS.${2})
./$(LIBDIR.${2})/$(${1}.BIN):
	@$(MKDIR) $(LIBDIR.${2})
	$$(call BLUE,"Linking $(LIBDIR.${2})/$$($$(CTGT).BIN)...")
	$(PRINT)$(LINK.static)
	$$(call GREEN,"Symbolic linking $(LIBDIR.${2})/$$($$(CTGT).LINK) -\> $(LIBDIR.${2})/$$($$(CTGT).BIN)...")
	$(PRINT)cd $(LIBDIR.${2}) && /bin/ln -sf $$($$(CTGT).BIN) $$($$(CTGT).LINK)
endef

##
# Macro for setting all necesary targets for building a shared library
# ${1}: Target
# ${2}: Build mode
##
define CMDS.so
.PHONY: ${1}-${2}
${1}-${2}: ./$(LIBDIR.${2})/${1} | ./$(LIBDIR.${2})/$(${1}.BIN)

./$(LIBDIR.${2})/${1}: ./$(LIBDIR.${2})/$(${1}.BIN)
./$(LIBDIR.${2})/${1}:
	$(call CMDS.${2},${RDIR},${1})

./$(LIBDIR.${2})/$(${1}.LINK): ./$(LIBDIR.${2})/$(${1}.BIN)
./$(LIBDIR.${2})/$(${1}.LINK):
	$(call CMDS.${2},${RDIR},${1})

./$(LIBDIR.${2})/$(${1}.BIN): override BUILD := ${2}
./$(LIBDIR.${2})/$(${1}.BIN): CTGT := ${1}
./$(LIBDIR.${2})/$(${1}.BIN): CDIR := $(RDIR)
./$(LIBDIR.${2})/$(${1}.BIN): CTYPE := shared
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.PREREQS.LIBS),$(addprefix ./$(LIBDIR.${2})/,$(${1}.PREREQS.LIBS)))
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.PREREQS.TGTS),$(${1}.PREREQS.TGTS))
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.GENSRCS),$(${1}.GENSRCS))
./$(LIBDIR.${2})/$(${1}.BIN): $(if $(${1}.GENOBJS),$(${1}.GENOBJS))
./$(LIBDIR.${2})/$(${1}.BIN): $(${1}.COBJS.${2})
./$(LIBDIR.${2})/$(${1}.BIN): $(${1}.CXXOBJS.${2})
./$(LIBDIR.${2})/$(${1}.BIN):
	@$(MKDIR) $(LIBDIR.${2})
	$$(call BLUE,"Linking $(LIBDIR.${2})/$$($$(CTGT).BIN)...")
	$(PRINT)$(LINK.shared) -o $$(LIBDIR.${2})/$$($$(CTGT).BIN) $(OUTPUT)
	$$(call GREEN,"Symbolic linking $(LIBDIR.${2})/$$($$(CTGT).LINK) -\> $(LIBDIR.${2})/$$($$(CTGT).BIN)...")
	$(PRINT)cd $(LIBDIR.${2}) && /bin/ln -sf $$($$(CTGT).BIN) $$($$(CTGT).LINK)
endef

##
# Macro for setting the correct targets, based on the extension of the file to be generated
# ${1}: Target
# ${2}: Build mode
##
define CMDS.TGT
ifeq (${${1}.TYPE},BINARY)
$$(eval $$(call CMDS.bin,${1},${2}))
else ifeq (${${1}.TYPE},STATIC)
$$(eval $$(call CMDS.a,${1},${2}))
else ifeq (${${1}.TYPE},SHARED)
$$(eval $$(call CMDS.so,${1},${2}))
else
$$(error ${1} - TGT.TYPE = ${${1}.TYPE} is not valid)
endif

ifeq ($(strip $(MAKECMDGOALS)),)
-include $$(${1}.CDEPS.${2})
-include $$(${1}.CXXDEPS.${2})
else ifneq (,$(filter-out clean help, $(MAKECMDGOALS)))
-include $$(${1}.CDEPS.${2})
-include $$(${1}.CXXDEPS.${2})
endif

endef

##
# Macro for setting the correct targets when generating examples
# ${1}: Directory
# ${2}: Target
# ${3}: Build mode
##
define CMDS.EXAMPLE.TGT
./$(BUILDDIR.base)/${1}/${2}-${3}: override BUILD := ${3}
./$(BUILDDIR.base)/${1}/${2}-${3}: CTGT := ${1}.${2}
./$(BUILDDIR.base)/${1}/${2}-${3}: CBIN := ${2}
./$(BUILDDIR.base)/${1}/${2}-${3}: CDIR := ${1}
./$(BUILDDIR.base)/${1}/${2}-${3}: CTYPE := binary
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.PREREQS.LIBS),$(addprefix ./$(LIBDIR.${3})/,$(${1}.${2}.PREREQS.LIBS)))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.PREREQS.TGTS),$(${1}.${2}.PREREQS.TGTS))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.GENSRCS),$(${1}.${2}.GENSRCS))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.GENOBJS),$(${1}.${2}.GENOBJS))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(${1}.${2}.COBJS.${3})
./$(BUILDDIR.base)/${1}/${2}-${3}: $(${1}.${2}.CXXOBJS.${3})
./$(BUILDDIR.base)/${1}/${2}-${3}:
	@$(MKDIR) $(BUILDDIR.base)/$$(CDIR)
	$$(call YELLOW,"Linking $(BUILDDIR.base)/$$(CDIR)/$$(CBIN)-$$(BUILD)...")
	$(PRINT)$(LINK.bin) -o $(BUILDDIR.base)/$$(CDIR)/$$(CBIN)-$$(BUILD) $(OUTPUT)
endef

##
# Macro for setting the correct targets when generating tests
# ${1}: Directory
# ${2}: Target
# ${3}: Build mode
##
define CMDS.TEST.TGT
./$(BUILDDIR.base)/${1}/${2}-${3}: override BUILD := ${3}
./$(BUILDDIR.base)/${1}/${2}-${3}: CTGT := ${1}.${2}
./$(BUILDDIR.base)/${1}/${2}-${3}: CBIN := ${2}
./$(BUILDDIR.base)/${1}/${2}-${3}: CDIR := ${1}
./$(BUILDDIR.base)/${1}/${2}-${3}: CTYPE := binary
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.PREREQS.LIBS),$(addprefix ./$(LIBDIR.${3})/,$(${1}.${2}.PREREQS.LIBS)))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.PREREQS.TGTS),$(${1}.${2}.PREREQS.TGTS))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.GENSRCS),$(${1}.${2}.GENSRCS))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(if $(${1}.${2}.GENOBJS),$(${1}.${2}.GENOBJS))
./$(BUILDDIR.base)/${1}/${2}-${3}: $(${1}.${2}.COBJS.${3})
./$(BUILDDIR.base)/${1}/${2}-${3}: $(${1}.${2}.CXXOBJS.${3})
./$(BUILDDIR.base)/${1}/${2}-${3}:
	@$(MKDIR) $(BUILDDIR.base)/$$(CDIR)
	$$(call YELLOW,"Linking $(BUILDDIR.base)/$$(CDIR)/$$(CBIN)-$$(BUILD)...")
	$(PRINT)$(LINK.bin) -o $(BUILDDIR.base)/$$(CDIR)/$$(CBIN)-$$(BUILD) $(OUTPUT)
endef

endif
