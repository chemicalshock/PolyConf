#
# makefile for shocktest unit tests
#
UT_MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
REPO_ROOT := $(abspath $(UT_MAKEFILE_DIR)/../../..)

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
# X86CPPTARGET += TEST_CONNECTION_BUFFER
# TEST_CONNECTION_BUFFER,SRCS = $(UT_MAKEFILE_DIR)/test_connection_buffer.cpp
# TEST_CONNECTION_BUFFER,USRLIBS = connection_buffer_SUT.cpp.so
# TEST_CONNECTION_BUFFER,CPPFLAGS = $(GLOBAL_CPPFLAGS)
# TEST_CONNECTION_BUFFER,LDFLAGS =

# Include shocktest makefile from dep/shocktest symlink (no absolute path).
include $(REPO_ROOT)/dep/shocktest/src/tst/ut/makefile.shocktest.mk
