###########################
# File: 	targets.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGETS.MK
TARGETS.MK := INCLUDED

##
# Load all targets
##
include $(sort $(wildcard $(MK)/target-*.mk))

endif
