#
# Copyright (C) 2011-2016 Intel Corporation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#   * Neither the name of Intel Corporation nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#

TOP_DIR  = ../..
include $(TOP_DIR)/buildenv.mk

CFLAGS   += -Werror
CXXFLAGS += -Werror

INCLUDE += -I$(COMMON_DIR)/inc/tlibc    \
           -I$(COMMON_DIR)/inc/internal \
           -I$(COMMON_DIR)/inc

INCLUDE += -I$(LINUX_PSW_DIR)/ae/inc    \
           -I$(LINUX_PSW_DIR)/ae/inc/internal \
           -I$(LINUX_PSW_DIR)/ae/common \
           -I$(LINUX_PSW_DIR)/ae/

INCLUDE += -I$(LINUX_EXTERNAL_DIR)/crypto_px/include     \
           -I$(LINUX_EXTERNAL_DIR)/epid/inc

INCLUDE += -I$(LINUX_SDK_DIR)/tlibstdcxx/stlport

vpath %.cpp $(COMMON_DIR)/src:$(LINUX_PSW_DIR)/ae/common
vpath %.edl $(COMMON_DIR)/inc

CXXFLAGS += $(ENCLAVE_CXXFLAGS)

SRC := $(wildcard *.cpp)
SRC += ecp.cpp

OBJ := $(SRC:.cpp=.o)

EDGER8R = $(LINUX_SDK_DIR)/edger8r/linux/_build/Edger8r.native
LIBNAME := libsgx_tkey_exchange.a

.PHONY: all
all: $(LIBNAME) | $(BUILD_DIR)
	@$(CP) $< $|
	@$(CP) sgx_tkey_exchange_t.h sgx_tkey_exchange_u.h $(COMMON_DIR)/inc/
	@$(CP) sgx_tkey_exchange_t.c sgx_tkey_exchange_u.c $(COMMON_DIR)/src/

$(LIBNAME): $(OBJ)
	$(AR) rcsD $@ $^

tkey_exchange.o: $(COMMON_DIR)/inc/sgx_tkey_exchange_t.h

$(COMMON_DIR)/inc/sgx_tkey_exchange_t.h: $(COMMON_DIR)/inc/sgx_tkey_exchange.edl
	$(EDGER8R) --search-path $(TOP_DIR)/common/inc sgx_tkey_exchange.edl

$(OBJ): %.o :%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE)  -c $< -o $@

$(BUILD_DIR):
	@$(MKDIR) $(BUILD_DIR)

.PHONY: clean
clean:
	@$(RM) $(OBJ)
	@$(RM) $(LIBNAME) $(BUILD_DIR)/$(LIBNAME)
	@$(RM) $(COMMON_DIR)/inc/sgx_tkey_exchange_t.h $(COMMON_DIR)/inc/sgx_tkey_exchange_u.h
	@$(RM) $(COMMON_DIR)/src/sgx_tkey_exchange_t.c $(COMMON_DIR)/src/sgx_tkey_exchange_u.c
	@$(RM) sgx_tkey_exchange_t.* sgx_tkey_exchange_u.*

.PHONY: rebuild
rebuild: clean all
