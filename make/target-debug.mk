###########################
# File: 	target-debug.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_DEBUG.MK
TARGET_DEBUG.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: debug $$(addsuffix -debug, $$(TGTS))
debug: override BUILD := debug
debug: $$(addsuffix -debug, $$(TGTS))

endif
