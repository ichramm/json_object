###########################
# File: 	target-tar.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_TAR.MK
TARGET_TAR.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: tar $$(addsuffix -tar, $$(TGTS))
tar: $$(addsuffix -tar, $$(TGTS))

endif
