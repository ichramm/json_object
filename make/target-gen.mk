###########################
# File: 	target-gen.mk
#
# Date: 	Nov 20, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_GEN.MK
TARGET_GEN.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: gen $$(addsuffix -gen, $$(TGTS))
gen: $$(addsuffix -gen, $$(TGTS))

endif
