###########################
# File: 	target-profile.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_PROFILE.MK
TARGET_PROFILE.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: profile $$(addsuffix -profile, $$(TGTS))
profile: override BUILD := profile
profile: $$(addsuffix -profile, $$(TGTS))

.PHONY: profile-report $$(addsuffix -profile-report, $$(TGTS))
profile: override BUILD := profile
profile-report: $$(addsuffix -profile-report, $$(TGTS))

endif
