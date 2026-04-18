# --------------------------------------------------------------------
#
# !\file makefile
# !\brief polyconf project configuration for makeshift.
# !\author Colin J.D. Stewart
#
# --------------------------------------------------------------------

PROJECT ?= polyconf

BIN_NAME ?= $(PROJECT)
LIB_BASENAME ?= polyconf

BUILD_EXE ?= 0
BUILD_STATIC ?= 1
BUILD_SHARED ?= 1

BUILD_M32 ?= 0
BUILD_M64 ?= 1

CXX_STD ?= c++20
MODE ?= debug

USER_CPPFLAGS :=
USER_CXXFLAGS := 
USER_LDFLAGS :=
USER_LDLIBS :=

include dep/makeshift/makeshift.mk