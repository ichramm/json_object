##
# Target name
##
TGT := json_object

##
# Target Type (BINARY|STATIC|SHARED)
# IMPORTANT: Don't use STATIC and SHARED directly unless it is strictly required.
#            Use $(LINK_TYPE) instead (default: STATIC).
#            Like this: TGT.TYPE := $(LINK_TYPE)
##
TGT.TYPE := $(LINK_TYPE)

##
# Target Specific Prerequisites - Required Library (.a|.so)
##
TGT.PREREQS.LIBS :=

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
TGT.CFLAGS := -Wno-conversion -Wno-sign-conversion -Wno-sign-compare
TGT.CFLAGS.analysis :=
TGT.CFLAGS.coverage :=
TGT.CFLAGS.debug :=
TGT.CFLAGS.profile :=
TGT.CFLAGS.release :=

TGT.CSRCS := $(shell find $(RDIR) -type f -name "*.c" -print | sort | $(SED) 's/^\.\///')

##
# Target Specific - C++ Files/Flags (Build Mode Specific Flags Are Appended After TGT.CXXFLAGS)
##
TGT.CXXFLAGS :=
TGT.CXXFLAGS.analysis :=
TGT.CXXFLAGS.coverage :=
TGT.CXXFLAGS.debug :=
TGT.CXXFLAGS.profile :=
TGT.CXXFLAGS.release :=

TGT.CXXSRCS := $(shell find $(RDIR) -type f -name "*.cpp" -print | sort | $(SED) 's/^\.\///')

##
# Target Specific - Linker Flags (Build Mode Specific Flags Are Appended After TGT.LDFLAGS)
##
TGT.LDFLAGS :=
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
# CTGT: Current Target
# CDIR: Current Directory
##
###########################
# Install
###########################
$(TGT)-install:
	$(call MAGENTA,"Installing $(CTGT)-$($(CTGT).LONGVERSION) into $(DESTDIR)...") && mkdir -p $(DESTDIR) && cp -rf $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)/* $(DESTDIR)

###########################
# Package
###########################
$(TGT)-package:
	$(call YELLOW,"Packing $(CTGT)-$($(CTGT).LONGVERSION) \<$(BUILD)/$(BUILD.ARCH)\>...")
	$(PRINT)$(RM) $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)
	$(PRINT)$(MKDIR) $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)
	$(PRINT)$(CP) $(ROOT)/include $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)/
	$(PRINT)$(CP) $(ROOT)/src $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)/
	$(PRINT)$(MKDIR) $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)/lib/$(BUILD)
	$(PRINT)$(CP) $(LIBDIR.base)/$(BUILD)/$($(CTGT).LINK) $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)/lib/$(BUILD)
	$(PRINT)$(CP) $(LIBDIR.base)/$(BUILD)/$($(CTGT).BIN) $(PKGDIR.base)/$(CTGT)-$($(CTGT).LONGVERSION)/lib/$(BUILD)
