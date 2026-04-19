#
# makefile for shocktest unit tests
#
UT_MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
repo_root_has_project_shape = $(if $(wildcard $(1)/src/inc),$(1))
REPO_ROOT_CANDIDATES := $(abspath $(UT_MAKEFILE_DIR)/../..) $(abspath $(UT_MAKEFILE_DIR)/../../..)
DETECTED_REPO_ROOT := $(firstword $(foreach dir,$(REPO_ROOT_CANDIDATES),$(if $(call repo_root_has_project_shape,$(dir)),$(dir))))
REPO_ROOT := $(if $(DETECTED_REPO_ROOT),$(DETECTED_REPO_ROOT),$(firstword $(REPO_ROOT_CANDIDATES)))

X86CPPTARGETSOL ?=

# ------------------------------
# SUT targets
# ------------------------------

# X86CPPTARGETSOL += session_state_SUT.cpp.so
# session_state_SUT.cpp.so,SRCS := $(REPO_ROOT)/src/lib/session_state.cpp
# session_state_SUT.cpp.so,DEPS :=
# session_state_SUT.cpp.so,CPPFLAGS = $(GLOBAL_CPPFLAGS)

# ------------------------------
# Test targets
# ------------------------------
X86CPPTARGET += TEST_POLYCONF_MVP
TEST_POLYCONF_MVP,SRCS := test_polyconf_mvp.cpp
TEST_POLYCONF_MVP,USRLIBS :=
TEST_POLYCONF_MVP,CPPFLAGS = $(GLOBAL_CPPFLAGS)
TEST_POLYCONF_MVP,LDFLAGS =

# Include shocktest makefile from dep/shocktest symlink (no absolute path).
include $(REPO_ROOT)/dep/shocktest/src/tst/ut/makefile.shocktest.mk
