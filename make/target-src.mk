###########################
# File: 	target-src.mk
#
# Date: 	Oct 14, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_SRC.MK
TARGET_SRC.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: src $$(addsuffix -src, $$(TGTS))
src: $$(addsuffix -src, $$(TGTS))

endif
