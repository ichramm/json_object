###########################
# File: 	target-version.mk
#
# Date: 	Apr 02, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_VERSION.MK
TARGET_VERSION.MK := INCLUDED

.PHONY: version
version:
	@echo "Intraway Build System"
	@echo "Version 1.3.23"

endif
