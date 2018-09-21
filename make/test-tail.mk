###########################
# File: 	test-tail.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifneq ($(strip $(TGT)),)

##
# Register the Test
##
TEST.TGTS := $(TEST.TGTS) $(RDIR)/$(TGT)

##
# Target Specific Prerequisites - Required Library|Targets
##
# Deprecated: $(TGT.PREREQS)
$(RDIR).$(TGT).PREREQS.LIBS := $(TGT.PREREQS.LIBS) $(TGT.PREREQS)
$(RDIR).$(TGT).PREREQS.TGTS := $(TGT.PREREQS.TGTS)

##
# Target Specific - Generated Source Files
##
$(RDIR).$(TGT).GENRAWS := $(subst ./,,$(TGT.GENRAWS))
$(RDIR).$(TGT).GENSRCS := $(subst ./,,$(TGT.GENSRCS))
$(RDIR).$(TGT).GENOBJS := $(filter %.o, $(patsubst %.c, %.o, $($(RDIR).$(TGT).GENSRCS)) $(patsubst %.cpp, %.o, $($(RDIR).$(TGT).GENSRCS)))

##
# Target Specific - C Files/Flags
##
$(RDIR).$(TGT).CFLAGS := $(TGT.CFLAGS)
$(RDIR).$(TGT).CFLAGS.analysis := $(TGT.CFLAGS.analysis)
$(RDIR).$(TGT).CFLAGS.coverage := $(TGT.CFLAGS.coverage)
$(RDIR).$(TGT).CFLAGS.debug := $(TGT.CFLAGS.debug)
$(RDIR).$(TGT).CFLAGS.profile := $(TGT.CFLAGS.profile)
$(RDIR).$(TGT).CFLAGS.release := $(TGT.CFLAGS.release)

$(RDIR).$(TGT).CSRCS := $(subst ./,,$(TGT.CSRCS))

##
# Target Specific - C++ Files/Flags
##
$(RDIR).$(TGT).CXXFLAGS := $(TGT.CXXFLAGS)
$(RDIR).$(TGT).CXXFLAGS.analysis := $(TGT.CXXFLAGS.analysis)
$(RDIR).$(TGT).CXXFLAGS.coverage := $(TGT.CXXFLAGS.coverage)
$(RDIR).$(TGT).CXXFLAGS.debug := $(TGT.CXXFLAGS.debug)
$(RDIR).$(TGT).CXXFLAGS.profile := $(TGT.CXXFLAGS.profile)
$(RDIR).$(TGT).CXXFLAGS.release := $(TGT.CXXFLAGS.release)

$(RDIR).$(TGT).CXXSRCS := $(subst ./,,$(TGT.CXXSRCS))

##
# Target Specific - Linker Flags
##
$(RDIR).$(TGT).LDFLAGS := $(TGT.LDFLAGS)
$(RDIR).$(TGT).LDFLAGS.analysis := $(TGT.LDFLAGS.analysis)
$(RDIR).$(TGT).LDFLAGS.coverage := $(TGT.LDFLAGS.coverage)
$(RDIR).$(TGT).LDFLAGS.debug := $(TGT.LDFLAGS.debug)
$(RDIR).$(TGT).LDFLAGS.profile := $(TGT.LDFLAGS.profile)
$(RDIR).$(TGT).LDFLAGS.release := $(TGT.LDFLAGS.release)

###########################
# Target
###########################
.PHONY: $(RDIR)/$(TGT)
$(RDIR)/$(TGT): ./$(BUILDDIR.base)/$(RDIR)/$(TGT)-$(BUILD)

###########################
# Analysis
###########################
$(RDIR).$(TGT).COBJS.analysis := $(addprefix $(OBJDIR.analysis)/, $(patsubst %.c, %.o, $($(RDIR).$(TGT).CSRCS)))
$(RDIR).$(TGT).CDEPS.analysis := $(patsubst %.o, %.d, $($(RDIR).$(TGT).COBJS))

$(RDIR).$(TGT).CXXOBJS.analysis := $(addprefix $(OBJDIR.analysis)/, $(patsubst %.cpp, %.o, $($(RDIR).$(TGT).CXXSRCS)))
$(RDIR).$(TGT).CXXDEPS.analysis := $(patsubst %.o, %.d, $($(RDIR).$(TGT).CXXOBJS))

$(eval $(call CMDS.TEST.TGT,$(RDIR),$(TGT),analysis))

###########################
# Coverage
###########################
$(RDIR).$(TGT).COBJS.coverage := $(addprefix $(OBJDIR.coverage)/, $(patsubst %.c, %.o, $($(RDIR).$(TGT).CSRCS)))
$(RDIR).$(TGT).CDEPS.coverage := $(patsubst %.o, %.d, $($(RDIR).$(TGT).COBJS))

$(RDIR).$(TGT).CXXOBJS.coverage := $(addprefix $(OBJDIR.coverage)/, $(patsubst %.cpp, %.o, $($(RDIR).$(TGT).CXXSRCS)))
$(RDIR).$(TGT).CXXDEPS.coverage := $(patsubst %.o, %.d, $($(RDIR).$(TGT).CXXOBJS))

$(eval $(call CMDS.TEST.TGT,$(RDIR),$(TGT),coverage))

###########################
# Clean
###########################
$(RDIR)/$(TGT)-clean: CTGT := $(RDIR).$(TGT)
$(RDIR)/$(TGT)-clean: CBIN := $(TGT)
$(RDIR)/$(TGT)-clean: CDIR := $(RDIR)
$(RDIR)/$(TGT)-clean:
	$(call RED,"Removing $(BUILDDIR.base)/$(CDIR)/$(CBIN)-analysis...")
	$(PRINT)$(RM) $(BUILDDIR.base)/$(CDIR)/$(CBIN)-analysis
	$(call RED,"Removing $(BUILDDIR.base)/$(CDIR)/$(CBIN)-coverage...")
	$(PRINT)$(RM) $(BUILDDIR.base)/$(CDIR)/$(CBIN)-coverage
	$(call RED,"Removing $(BUILDDIR.base)/$(CDIR)/$(CBIN)-debug...")
	$(PRINT)$(RM) $(BUILDDIR.base)/$(CDIR)/$(CBIN)-debug
	$(call RED,"Removing $(BUILDDIR.base)/$(CDIR)/$(CBIN)-profile...")
	$(PRINT)$(RM) $(BUILDDIR.base)/$(CDIR)/$(CBIN)-profile
	$(call RED,"Removing $(BUILDDIR.base)/$(CDIR)/$(CBIN)-release...")
	$(PRINT)$(RM) $(BUILDDIR.base)/$(CDIR)/$(CBIN)-release

###########################
# Debug
###########################
$(RDIR).$(TGT).COBJS.debug := $(addprefix $(OBJDIR.debug)/, $(patsubst %.c, %.o, $($(RDIR).$(TGT).CSRCS)))
$(RDIR).$(TGT).CDEPS.debug := $(patsubst %.o, %.d, $($(RDIR).$(TGT).COBJS))

$(RDIR).$(TGT).CXXOBJS.debug := $(addprefix $(OBJDIR.debug)/, $(patsubst %.cpp, %.o, $($(RDIR).$(TGT).CXXSRCS)))
$(RDIR).$(TGT).CXXDEPS.debug := $(patsubst %.o, %.d, $($(RDIR).$(TGT).CXXOBJS))

$(eval $(call CMDS.TEST.TGT,$(RDIR),$(TGT),debug))

###########################
# Generate Source
############################
$(RDIR).$(TGT)-gen: CTGT := $(RDIR).$(TGT)
$(RDIR).$(TGT)-gen: CBIN := $(TGT)
$(RDIR).$(TGT)-gen: CDIR := $(RDIR)
$(RDIR).$(TGT)-gen: $($(RDIR).$(TGT).GENSRCS)
$(RDIR).$(TGT)-gen:

###########################
# Profile
###########################
$(RDIR).$(TGT).COBJS.profile := $(addprefix $(OBJDIR.profile)/, $(patsubst %.c, %.o, $($(RDIR).$(TGT).CSRCS)))
$(RDIR).$(TGT).CDEPS.profile := $(patsubst %.o, %.d, $($(RDIR).$(TGT).COBJS))

$(RDIR).$(TGT).CXXOBJS.profile := $(addprefix $(OBJDIR.profile)/, $(patsubst %.cpp, %.o, $($(RDIR).$(TGT).CXXSRCS)))
$(RDIR).$(TGT).CXXDEPS.profile := $(patsubst %.o, %.d, $($(RDIR).$(TGT).CXXOBJS))

$(eval $(call CMDS.TEST.TGT,$(RDIR),$(TGT),profile))

###########################
# Release
###########################
$(RDIR).$(TGT).COBJS.release := $(addprefix $(OBJDIR.release)/, $(patsubst %.c, %.o, $($(RDIR).$(TGT).CSRCS)))
$(RDIR).$(TGT).CDEPS.release := $(patsubst %.o, %.d, $($(RDIR).$(TGT).COBJS))

$(RDIR).$(TGT).CXXOBJS.release := $(addprefix $(OBJDIR.release)/, $(patsubst %.cpp, %.o, $($(RDIR).$(TGT).CXXSRCS)))
$(RDIR).$(TGT).CXXDEPS.release := $(patsubst %.o, %.d, $($(RDIR).$(TGT).CXXOBJS))

$(eval $(call CMDS.TEST.TGT,$(RDIR),$(TGT),release))

###########################
# Run (For Check)
###########################
$(RDIR)/$(TGT)-run: CTGT := $(RDIR).$(TGT)
$(RDIR)/$(TGT)-run: CBIN := $(TGT)
$(RDIR)/$(TGT)-run: CDIR := $(RDIR)
$(RDIR)/$(TGT)-run:

endif
