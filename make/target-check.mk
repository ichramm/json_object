###########################
# File: 	target-check.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_CHECK.MK
TARGET_CHECK.MK := INCLUDED

.PHONY: $$(addsuffix -run, $$(TEST.TGTS))

.PHONY: check
check: check-$(BUILD)

.PHONY: check-analysis
check-analysis: override BUILD := analysis
check-analysis: test-analysis
check-analysis: $$(addsuffix -run, $$(TEST.TGTS))

.PHONY: check-coverage
check-coverage: override BUILD := coverage
check-coverage: test-coverage
check-coverage: $$(addsuffix -run, $$(TEST.TGTS))

.PHONY: check-debug
check-debug: override BUILD := debug
check-debug: test-debug
check-debug: $$(addsuffix -run, $$(TEST.TGTS))

.PHONY: check-profile
check-profile: override BUILD := profile
check-profile: test-profile
check-profile: $$(addsuffix -run, $$(TEST.TGTS))

.PHONY: check-release
check-release: override BUILD := release
check-release: test-release
check-release: $$(addsuffix -run, $$(TEST.TGTS))

endif
