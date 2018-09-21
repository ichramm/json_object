###########################
# File: 	target-changelog.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_CHANGELOG.MK
TARGET_CHANGELOG.MK := INCLUDED

.PHONY: changelog
changelog:
	$(PRINT)bash $(SCRIPTS)/changelog.sh $(VERSION) $(REV_BEGIN) $(REV_END)

endif
