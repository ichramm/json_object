###########################
# File: 	module-tail.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifneq ($(strip $(TGT)),)

##
# Register the Target
##
TGTS := $(TGTS) $(TGT)

##
# Target Type (BINARY|STATIC|SHARED)
##
$(TGT).TYPE := $(shell echo "$(TGT.TYPE)" | tr a-z A-Z)

ifneq (,$(filter-out BINARY STATIC SHARED,$($(TGT).TYPE)))
$(error $(TGT) - TGT.TYPE = '$(TGT.TYPE)' is not valid)
endif

##
# Target Version
# C.M.m.p
##
$(TGT).CORE  := $(VERSION.CORE)
$(TGT).MAJOR := $(VERSION.MAJOR)
$(TGT).MINOR := $(VERSION.MINOR)
$(TGT).PATCH := $(VERSION.PATCH)
ifeq ($(TGT).PATCH,)
	$(TGT).PATCH := 0
endif

$(TGT).SHORTVERSION := $(VERSION.SHORT)
$(TGT).VERSION := $(VERSION)
$(TGT).LONGVERSION := $(VERSION.LONG)

##
# Target Stage/Sequences
# - Nightly   0
# - Alpha 	  1
# - Beta	  2
# - RC		  3
# - Final     4
##
$(TGT).STAGE := $(VERSION.STAGE)
$(TGT).SEQUENCE := $(VERSION.SEQUENCE)

##
# Target Build Number
##
$(TGT).BUILD := $(BUILDID)

##
# Target Release Version
##
ifeq ($($(TGT).STAGE), 0)
$(TGT).RELEASE := $($(TGT).LONGVERSION)-nightly
else ifeq ($($(TGT).STAGE), 1)
$(TGT).RELEASE := $($(TGT).LONGVERSION)-a$($(TGT).SEQUENCE)
else ifeq ($($(TGT).STAGE), 2)
$(TGT).RELEASE := $($(TGT).LONGVERSION)-b$($(TGT).SEQUENCE)
else ifeq ($($(TGT).STAGE), 3)
$(TGT).RELEASE := $($(TGT).LONGVERSION)-rc$($(TGT).SEQUENCE)
else ifeq ($($(TGT).STAGE), 4)
$(TGT).RELEASE := $($(TGT).LONGVERSION)
else
$(error $(TGT) - TGT.STAGE = $(TGT.STAGE) is not valid)
endif

##
# Target Prefix/Suffix Name
##
#$(TGT).BASE := $(basename $(TGT))
#$(TGT).SUFFIX := $(suffix $(TGT))

##
# Target Name
##
$(TGT).NAME := $(TGT)-$($(TGT).RELEASE)-$($(TGT).BUILD)-$(BUILD.ARCH)

##
# Target Binary Name
##
ifeq ($($(TGT).TYPE), BINARY)
$(TGT).BIN := $($(TGT).NAME)
else ifeq ($($(TGT).TYPE), STATIC)
$(TGT).BIN := lib$($(TGT).NAME).a
else ifeq ($($(TGT).TYPE), SHARED)
$(TGT).BIN := lib$($(TGT).NAME).so
endif

##
# Target Link Name
##
ifeq ($($(TGT).TYPE), BINARY)
$(TGT).LINK := $(TGT)
else ifeq ($($(TGT).TYPE), STATIC)
$(TGT).LINK := lib$(TGT).a
else ifeq ($($(TGT).TYPE), SHARED)
$(TGT).LINK := lib$(TGT).so
endif

##
# Target Specific Prerequisites - Required Library|Targets
##
# Deprecated: $(TGT.PREREQS)
$(TGT).PREREQS.LIBS := $(TGT.PREREQS.LIBS) $(TGT.PREREQS)
$(TGT).PREREQS.TGTS := $(TGT.PREREQS.TGTS)

##
# Target Specific - Generated Source Files
##
$(TGT).GENRAWS := $(subst ./,,$(TGT.GENRAWS))
$(TGT).GENSRCS := $(subst ./,,$(TGT.GENSRCS))
$(TGT).GENOBJS := $(filter %.o, $(patsubst %.c, %.o, $($(TGT).GENSRCS)) $(patsubst %.cpp, %.o, $($(TGT).GENSRCS)))

##
# Target Specific - C Files/Flags
##
$(TGT).CFLAGS := $(TGT.CFLAGS)
$(TGT).CFLAGS.analysis := $(TGT.CFLAGS.analysis)
$(TGT).CFLAGS.coverage := $(TGT.CFLAGS.coverage)
$(TGT).CFLAGS.debug := $(TGT.CFLAGS.debug)
$(TGT).CFLAGS.profile := $(TGT.CFLAGS.profile)
$(TGT).CFLAGS.release := $(TGT.CFLAGS.release)

$(TGT).CSRCS := $(subst ./,,$(TGT.CSRCS))

##
# Target Specific - C++ Files/Flags
##
$(TGT).CXXFLAGS := $(TGT.CXXFLAGS)
$(TGT).CXXFLAGS.analysis := $(TGT.CXXFLAGS.analysis)
$(TGT).CXXFLAGS.coverage := $(TGT.CXXFLAGS.coverage)
$(TGT).CXXFLAGS.debug := $(TGT.CXXFLAGS.debug)
$(TGT).CXXFLAGS.profile := $(TGT.CXXFLAGS.profile)
$(TGT).CXXFLAGS.release := $(TGT.CXXFLAGS.release)

$(TGT).CXXSRCS := $(subst ./,,$(TGT.CXXSRCS))

##
# Target Specific - Linker Flags
##
$(TGT).LDFLAGS := $(TGT.LDFLAGS)
$(TGT).LDFLAGS.analysis := $(TGT.LDFLAGS.analysis)
$(TGT).LDFLAGS.coverage := $(TGT.LDFLAGS.coverage)
$(TGT).LDFLAGS.debug := $(TGT.LDFLAGS.debug)
$(TGT).LDFLAGS.profile := $(TGT.LDFLAGS.profile)
$(TGT).LDFLAGS.release := $(TGT.LDFLAGS.release)

##
# Target Specific - Linker Symbols
##
ifeq ($(OS), Linux)
	$(TGT).LDSYMBOLS := -Wl,--defsym,__CORE__=$($(TGT).CORE) \
 	   				    -Wl,--defsym,__MAJOR__=$($(TGT).MAJOR) \
					    -Wl,--defsym,__MINOR__=$($(TGT).MINOR) \
					    -Wl,--defsym,__PATCH__=$($(TGT).PATCH) \
					    -Wl,--defsym,__STAGE__=$($(TGT).STAGE) \
					    -Wl,--defsym,__SEQUENCE__=$($(TGT).SEQUENCE) \
					    -Wl,--defsym,__BUILD__=$($(TGT).BUILD)
else
	$(TGT).LDSYMBOLS :=
endif

###########################
# Target
###########################
.PHONY: $(TGT)
$(TGT): $(TGT)-$(BUILD)

###########################
# Analysis
###########################
$(TGT).COBJS.analysis := $(addprefix $(OBJDIR.analysis)/, $(patsubst %.c, %.o, $($(TGT).CSRCS)))
$(TGT).CDEPS.analysis := $(patsubst %.o, %.d, $($(TGT).COBJS.analysis))

$(TGT).CXXOBJS.analysis := $(addprefix $(OBJDIR.analysis)/, $(patsubst %.cpp, %.o, $($(TGT).CXXSRCS)))
$(TGT).CXXDEPS.analysis := $(patsubst %.o, %.d, $($(TGT).CXXOBJS.analysis))

$(eval $(call CMDS.TGT,$(TGT),analysis))

###########################
# Clean
###########################
$(TGT)-clean: CTGT := $(TGT)
$(TGT)-clean: CDIR := $(RDIR)
$(TGT)-clean:
	$(call RED,"Removing $(CDIR)/$(CTGT)-status...")
	$(PRINT)$(RM) $(CDIR)/$(CTGT)-status
	$(call RED,"Removing $(BINDIR.analysis)/$(CTGT)...")
	$(PRINT)$(RM) $(BINDIR.analysis)/$(CTGT)
	$(call RED,"Removing $(BINDIR.analysis)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(BINDIR.analysis)/$($(CTGT).BIN)
	$(call RED,"Removing $(BINDIR.coverage)/$(CTGT)...")
	$(PRINT)$(RM) $(BINDIR.coverage)/$(CTGT)
	$(call RED,"Removing $(BINDIR.coverage)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(BINDIR.coverage)/$($(CTGT).BIN)
	$(call RED,"Removing $(BINDIR.debug)/$(CTGT)...")
	$(PRINT)$(RM) $(BINDIR.debug)/$(CTGT)
	$(call RED,"Removing $(BINDIR.debug)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(BINDIR.debug)/$($(CTGT).BIN)
	$(call RED,"Removing $(BINDIR.profile)/$(CTGT)...")
	$(PRINT)$(RM) $(BINDIR.profile)/$(CTGT)
	$(call RED,"Removing $(BINDIR.profile)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(BINDIR.profile)/$($(CTGT).BIN)
	$(call RED,"Removing $(BINDIR.release)/$(CTGT)...")
	$(PRINT)$(RM) $(BINDIR.release)/$(CTGT)
	$(call RED,"Removing $(BINDIR.release)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(BINDIR.release)/$($(CTGT)_BIN)
	$(call RED,"Removing $(LIBDIR.analysis)/$(CTGT)...")
	$(PRINT)$(RM) $(LIBDIR.analysis)/$(CTGT)
	$(call RED,"Removing $(LIBDIR.analysis)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(LIBDIR.analysis)/$($(CTGT).BIN)
	$(call RED,"Removing $(LIBDIR.coverage)/$(CTGT)...")
	$(PRINT)$(RM) $(LIBDIR.coverage)/$(CTGT)
	$(call RED,"Removing $(LIBDIR.coverage)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(LIBDIR.coverage)/$($(CTGT).BIN)
	$(call RED,"Removing $(LIBDIR.debug)/$(CTGT)...")
	$(PRINT)$(RM) $(LIBDIR.debug)/$(CTGT)
	$(call RED,"Removing $(LIBDIR.debug)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(LIBDIR.debug)/$($(CTGT).BIN)
	$(call RED,"Removing $(LIBDIR.profile)/$(CTGT)...")
	$(PRINT)$(RM) $(LIBDIR.profile)/$(CTGT)
	$(call RED,"Removing $(LIBDIR.profile)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(LIBDIR.profile)/$($(CTGT).BIN)
	$(call RED,"Removing $(LIBDIR.release)/$(CTGT)...")
	$(PRINT)$(RM) $(LIBDIR.release)/$(CTGT)
	$(call RED,"Removing $(LIBDIR.release)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(LIBDIR.release)/$($(CTGT)_BIN)
	$(call RED,"Removing $(OBJDIR.coverage)/$(CDIR)/$(CTGT)-base.lcov...")
	$(PRINT)$(RM) $(OBJDIR.coverage)/$(CDIR)/$(CTGT)-base.lcov
	$(call RED,"Removing $(OBJDIR.coverage)/$(CDIR)/$(CTGT)-run.lcov...")
	$(PRINT)$(RM) $(OBJDIR.coverage)/$(CDIR)/$(CTGT)-run.lcov
	$(call RED,"Removing $(PKGDIR.base)/$($(CTGT).BIN)...")
	$(PRINT)$(RM) $(PKGDIR.base)/$($(CTGT).BIN)

###########################
# Coverage
###########################
$(TGT).COBJS.coverage := $(addprefix $(OBJDIR.coverage)/, $(patsubst %.c, %.o, $($(TGT).CSRCS)))
$(TGT).CDEPS.coverage := $(patsubst %.o, %.d, $($(TGT).COBJS.coverage))

$(TGT).CXXOBJS.coverage := $(addprefix $(OBJDIR.coverage)/, $(patsubst %.cpp, %.o, $($(TGT).CXXSRCS)))
$(TGT).CXXDEPS.coverage := $(patsubst %.o, %.d, $($(TGT).CXXOBJS.coverage))

$(eval $(call CMDS.TGT,$(TGT),coverage))

$(TGT)-coverage-report: CTGT := $(TGT)
$(TGT)-coverage-report: CDIR := $(RDIR)
$(TGT)-coverage-report:
	$(call CMDS.coverage-report,$(CDIR),$(CTGT))

###########################
# Debug
###########################
$(TGT).COBJS.debug := $(addprefix $(OBJDIR.debug)/, $(patsubst %.c, %.o, $($(TGT).CSRCS)))
$(TGT).CDEPS.debug := $(patsubst %.o, %.d, $($(TGT).COBJS.debug))

$(TGT).CXXOBJS.debug := $(addprefix $(OBJDIR.debug)/, $(patsubst %.cpp, %.o, $($(TGT).CXXSRCS)))
$(TGT).CXXDEPS.debug := $(patsubst %.o, %.d, $($(TGT).CXXOBJS.debug))

$(eval $(call CMDS.TGT,$(TGT),debug))

###########################
# Generate Source
###########################
$(TGT)-gen: CTGT := $(TGT)
$(TGT)-gen: CDIR := $(RDIR)
$(TGT)-gen: $($(TGT).GENSRCS)
$(TGT)-gen:

###########################
# Install (Extension Point)
###########################
$(TGT)-install: CTGT := $(TGT)
$(TGT)-install: CDIR := $(RDIR)
$(TGT)-install: $(TGT)-package
$(TGT)-install:

###########################
# Package (Extension Point)
###########################
$(TGT)-package: CTGT := $(TGT)
$(TGT)-package: CDIR := $(RDIR)
$(TGT)-package: $(TGT)-$(BUILD)
$(TGT)-package:

###########################
# Package Debug Symbols (Extension Point)
# Note: When PACKAGEDIR is defined this target is not run
###########################
$(TGT)-symbol-package: CTGT := $(TGT)
$(TGT)-symbol-package: CDIR := $(RDIR)
ifeq ($(LD_HAS_BUILD_ID),yes)
$(TGT)-symbol-package: $(TGT)-package
$(TGT)-symbol-package:
ifndef PACKAGEDIR
	$(call YELLOW,"Extracting and packing debug symbols from $(CTGT) \<$(BUILD)/$(BUILD.ARCH)\>...")
	$(PRINT)if [ "$($(CTGT).TYPE)" = "BINARY" ]; then \
		PKGDBGDIR=$(PKGDBGDIR.base) $(SCRIPTS)/symbol-package.sh $(PKGDIR.base)/$(CTGT)-$($(CTGT).SHORTVERSION)/bin/$($(CTGT).BIN); \
	elif [ "$($(CTGT).TYPE)" = "SHARED" ]; then \
		PKGDBGDIR=$(PKGDBGDIR.base) $(SCRIPTS)/symbol-package.sh $(PKGDIR.base)/$(CTGT)-$($(CTGT).SHORTVERSION)/lib/$($(CTGT).BIN); \
	fi
endif
endif

###########################
# Profile
###########################
$(TGT).COBJS.profile := $(addprefix $(OBJDIR.profile)/, $(patsubst %.c, %.o, $($(TGT).CSRCS)))
$(TGT).CDEPS.profile := $(patsubst %.o, %.d, $($(TGT).COBJS.profile))

$(TGT).CXXOBJS.profile := $(addprefix $(OBJDIR.profile)/, $(patsubst %.cpp, %.o, $($(TGT).CXXSRCS)))
$(TGT).CXXDEPS.profile := $(patsubst %.o, %.d, $($(TGT).CXXOBJS.profile))

$(eval $(call CMDS.TGT,$(TGT),profile))

$(TGT)-profile-report: CTGT := $(TGT)
$(TGT)-profile-report: CDIR := $(RDIR)
$(TGT)-profile-report:
	$(call CMDS.profile-report,$(CDIR),$(CTGT))

###########################
# Release
###########################
$(TGT).COBJS.release := $(addprefix $(OBJDIR.release)/, $(patsubst %.c, %.o, $($(TGT).CSRCS)))
$(TGT).CDEPS.release := $(patsubst %.o, %.d, $($(TGT).COBJS.release))

$(TGT).CXXOBJS.release := $(addprefix $(OBJDIR.release)/, $(patsubst %.cpp, %.o, $($(TGT).CXXSRCS)))
$(TGT).CXXDEPS.release := $(patsubst %.o, %.d, $($(TGT).CXXOBJS.release))

$(eval $(call CMDS.TGT,$(TGT),release))

###########################
# RPM (Extension Point)
###########################
$(TGT)-rpm: CTGT := $(TGT)
$(TGT)-rpm: CDIR := $(RDIR)
$(TGT)-rpm:

###########################
# Sources
###########################
$(TGT)-src: CTGT := $(TGT)
$(TGT)-src: CDIR := $(RDIR)
$(TGT)-src:
	$(call YELLOW,"Packing $(CTGT)-$($(CTGT).RELEASE)...")
	$(PRINT)$(RM) $(SRCDIR.base)/$(CTGT)-$($(CTGT).RELEASE)
	$(PRINT)$(MKDIR) $(SRCDIR.base)/$(CTGT)-$($(CTGT).RELEASE)
	$(PRINT)$(CP) `ls | grep -v $(subst ./,,.gradle) | grep -v $(subst ./,,.git) | grep -v $(subst ./,,.svn) | grep -v $(subst ./,,${BUILDDIR.base}) | grep -v $(subst ./,,${GRADLEDIR.base}) | grep -v $(subst ./,,${MAKEDIR.base}) | grep -v $(subst ./,,${LIBSDIR.base})` $(SRCDIR.base)/$(CTGT)-$($(CTGT).RELEASE)/ 2>/dev/null || true
	$(PRINT)$(CP) $(ROOT)/gradle.properties $(SRCDIR.base)/$(CTGT)-$($(CTGT).RELEASE)/ 2>/dev/null || true
	$(PRINT)$(GREP) -qsw "version" $(ROOT)/gradle.properties || echo "version = $(VERSION)" >> $(SRCDIR.base)/$(CTGT)-$($(CTGT).RELEASE)/gradle.properties
	$(call YELLOW,"Compressing $(CTGT)-$($(CTGT).RELEASE).tar.gz...")
	$(PRINT)$(RM) $(SRCDIR.base)/$(CTGT)-$($(CTGT).RELEASE).tar.gz
	$(PRINT)$(TAR) $(SRCDIR.base)/$(CTGT)-$($(CTGT).RELEASE).tar.gz -C $(SRCDIR.base) $(CTGT)-$($(CTGT).RELEASE)

###########################
# Tar
###########################
$(TGT)-tar: CTGT := $(TGT)
$(TGT)-tar: CDIR := $(RDIR)
$(TGT)-tar: $(TGT)-package
$(TGT)-tar:
	$(call YELLOW,"Compressing $(CTGT)-$($(CTGT).RELEASE).tar.gz...")
	$(PRINT)$(RM) $(TARDIR.base)/$(CTGT)-$($(CTGT).RELEASE).tar.gz
	$(PRINT)$(TAR) $(TARDIR.base)/$(CTGT)-$($(CTGT).RELEASE).tar.gz -C $(PKGDIR.base) $(CTGT)-$($(CTGT).SHORTVERSION)

endif
