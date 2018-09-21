###########################
# File: 	target-test.mk
#
# Date: 	Jan 09, 2015
#
# Author: 	Tomás Kelly <tomas.kelly@intraway.com>
# Owner: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_TEST.MK
TARGET_TEST.MK := INCLUDED

.SECONDEXPANSION:

.PHONY: test
test: test-$(BUILD)

.PHONY: test-analysis
test-analysis: override BUILD := analysis
test-analysis: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -analysis, $$(TEST.TGTS)))

.PHONY: test-coverage
test-coverage: override BUILD := coverage
test-coverage: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -coverage, $$(TEST.TGTS)))

.PHONY: test-debug
test-debug: override BUILD := debug
test-debug: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -debug, $$(TEST.TGTS)))

.PHONY: test-profile
test-profile: override BUILD := profile
test-profile: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -profile, $$(TEST.TGTS)))

.PHONY: test-release
test-release: override BUILD := release
test-release: $$(addprefix ./$(BUILDDIR.base)/, $$(addsuffix -release, $$(TEST.TGTS)))

endif
