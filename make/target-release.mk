###########################
# File: 	target-release.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_RELEASE.MK
TARGET_RELEASE.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: release $$(addsuffix -release, $$(TGTS))
release: override BUILD := release
release: $$(addsuffix -release, $$(TGTS))

endif
