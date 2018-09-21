###########################
# File: 	test-head.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

##
# Clear Target
##
TGT := 

##
# Clear Target Specific Prerequisites
##
# Deprecated: $(TGT.PREREQS)
TGT.PREREQS := 
TGT.PREREQS.LIBS :=
TGT.PREREQS.TGTS :=

##
# Target Specific - Generated Source Files
##
TGT.GENRAWS :=
TGT.GENSRCS :=
TGT.GENOBJS :=

##
# Clear Target Specific - C Files/Flags
##
TGT.CFLAGS :=
TGT.CFLAGS.analysis := 
TGT.CFLAGS.coverage := 
TGT.CFLAGS.debug := 
TGT.CFLAGS.profile := 
TGT.CFLAGS.release := 

TGT.CSRCS :=

##
# Clear Target Specific - C++ Files/Flags
##
TGT.CXXFLAGS :=
TGT.CXXFLAGS.analysis := 
TGT.CXXFLAGS.coverage := 
TGT.CXXFLAGS.debug := 
TGT.CXXFLAGS.profile := 
TGT.CXXFLAGS.release := 

TGT.CXXSRCS :=

##
# Clear Target Specific - Linker Flags
##
TGT.LDFLAGS :=
TGT.LDFLAGS.analysis := 
TGT.LDFLAGS.coverage := 
TGT.LDFLAGS.debug := 
TGT.LDFLAGS.profile := 
TGT.LDFLAGS.release := 

