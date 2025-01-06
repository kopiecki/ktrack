project   := ktrack
cores     := $(shell nproc)
root_dir  := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
build_dir := _build
bin_dir   := $(build_dir)/app
debug     ?= y
top       := $(abspath $(dir $(firstword $(MAKEFILE_LIST))))

cmake_conf_flags += -DPROJECT_NAME=$(project)
ifeq ($(debug),y)
cmake_conf_flags += -DCMAKE_BUILD_TYPE=DEBUG
else
cmake_conf_flags += -DCMAKE_BUILD_TYPE=RELEASE
endif

cmake_conf_flags += "-DCMAKE_TOOLCHAIN_FILE=$(top)/cmake/toolchain/native.cmake"
cmake_conf_flags += "-GNinja"

cmake_configure  := cmake $(cmake_conf_flags) ..
cmake_build      := cmake --build . $(cmake_build_flags) -- -j$(cores)

include scripts/logger.mk

build: | .mk-build-dir
	$(call log_info,"Building $(project)...")
	$(at) cd $(build_dir) && \
	        [ -f CMakeCache.txt ] \
	            && echo "Building project $(project)" \
	            || $(cmake_configure) && \
	        $(cmake_build)
	$(call log_info,"DONE: $@")
.PHONY: build

run:
	$(call log_info,"Running $(project)")
	$(at) ./$(bin_dir)/$(project)
	$(call log_info,"DONE: $@")
.PHONY: run

.mk-build-dir:
	$(call log_info,"Creating $(project) build directory...")
	$(at) mkdir -p $(build_dir)
	$(call log_info,"DONE: $@")
.PHONY: .mk-build-dir

clean:
	$(call log_info,"Cleaning $(project) build directory...")
	$(at) rm -rf "$(build_dir)"
	$(call log_info,"DONE: $@")
.PHONY: clean
