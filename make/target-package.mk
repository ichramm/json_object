###########################
# File: 	target-package.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_PACKAGE.MK
TARGET_PACKAGE.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: package $$(addsuffix -package, $$(TGTS))
package: $$(addsuffix -package, $$(TGTS))

endif
