red    := $(shell /bin/echo -e "\033[0;31m")
yellow := $(shell /bin/echo -e "\033[0;33m")
green  := $(shell /bin/echo -e "\033[0;32m")
end    := $(shell /bin/echo -e "\033[0m")

timestamp            = $(shell date '+%X.%N' | cut -c-12)
define log_info
	@echo "[$(timestamp)] $(green)INFO: $(1)$(end)"
endef

sev ?= 4

ifeq ($(sev),0)
fmt := >/dev/null
else ifeq ($(sev),1)
fmt := | grep 'ERROR'
else ifeq ($(sev),2)
fmt := | grep 'ERROR\|WARNING'
else ifeq ($(sev),3)
fmt := | grep 'ERROR\|WARNING\|INFO'
else
fmt :=
endif

ifeq ($(verbose),y)
at :=
else
at := @
endif
