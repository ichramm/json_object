###########################
# File: 		target-help.mk
#
# Date: 		Jan 12, 2015
#
# Author: 		Tomás Kelly <tomas.kelly@intraway.com>
# Maintainer: 	Tomás Kelly <tomas.kelly@intraway.com>
#
###########################

ifndef TARGET_HELP.MK
TARGET_HELP.MK := INCLUDED

.PHONY : ?
?: help

.PHONY : help
help:
	@echo "Usage:"
	@echo "  make [options] <targets>"
	@echo ""
	@echo "Options:"
	@echo "  BUILD              - Build mode for target 'all' [BUILD = $(BUILD)]"
	@echo "  CCCC.PARAMS        - Additional parameters for 'cccc'"
	@echo "  CPPCHECK.PARAMS    - Additional parameters for 'cppcheck'"
	@echo "  OUTDIR             - Output directory for <analysis|coverage|profile> targets [OUTDIR = $(OUTDIR.base)]"
	@echo "  REV_BEGIN          - Revision from where to update the Changlog"
	@echo "  REV_END            - Revision to where update the Changlog"
	@echo "  VERAPP.PARAMS      - Additional parameters for 'vera++'"
	@echo "  VERBOSE            - ON: Show compiler output messages. OFF: Hide compiler output messages [VERBOSE = $(VERBOSE)]"
	@echo "  VERSION            - Version to be used when updating the Changlog"
	@echo ""
	@echo "Targets:"
	@echo "  all                - Build all the registered modules [BUILD = $(BUILD)]"
	@echo "  analysis           - Build all the registered modules [BUILD = analysis]"
	@echo "  changelog          - Generate Changelog entries [REV_BEGIN=N] [REV_END=N] [VERSION=C.M.m.p] "
	@echo "  check              - Build and run all the registered tests [BUILD = $(BUILD)]"
	@echo "  check-analysis     - Build and run all the registered tests [BUILD = analysis]"
	@echo "  check-coverage     - Build and run all the registered tests [BUILD = coverage]"
	@echo "  check-debug        - Build and run all the registered tests [BUILD = debug]"
	@echo "  check-profile      - Build and run all the registered tests [BUILD = profile]"
	@echo "  check-release      - Build and run all the registered tests [BUILD = release]"
	@echo "  clean              - Remove all generated files for the registered modules and tests"
	@echo "  coverage           - Build all the registered modules [BUILD = coverage]"
	@echo "  coverage-report    - Generate the coverage report for all registered modules"
	@echo "  debug              - Build all the registered modules [BUILD = debug]"
	@echo "  doc                - Generate the documentations for the project"
	@echo "  help               - Print the list of common targets for the Makefile"
	@echo "  install            - Install all the registered modules that allow for installation"
	@echo "  package            - Generate packages for all the registered modules that allow packaging"
	@echo "  profile            - Build all the registered modules [BUILD = profile]"
	@echo "  profile-report     - Generate the profile report for all registered modules"
	@echo "  project            - Create project directory structure"
	@echo "  release            - Build all the registered modules [BUILD = release]"
	@echo "  src                - Generate Tar package of source files"
	@echo "  tar                - Generate Tar package for installation"
	@echo "  test               - Build all the registered tests [BUILD = $(BUILD)]"
	@echo "  test-analysis      - Build all the registered tests [BUILD = analysis]"
	@echo "  test-coverage      - Build all the registered tests [BUILD = coverage]"
	@echo "  test-debug         - Build all the registered tests [BUILD = debug]"
	@echo "  test-profile       - Build all the registered tests [BUILD = profile]"
	@echo "  test-release       - Build all the registered tests [BUILD = release]"
	@echo "  version            - Print Intraway Build System's version"
	@echo ""

endif
