.PHONY: all host ta clean

HOST_DIR=./ree_host
HOST_MAKEFILE=$(HOST_DIR)/host.mk
TA_DIR=./ta
TA_MAKEFILE=$(TA_DIR)/ta.mk
OUT_DIR=out

all: host ta

host:
	@make -f $(HOST_MAKEFILE) OUT_DIR=out

ta:
	@make -f $(TA_MAKEFILE) OUT_DIR=out

clean:
	@rm -rf $(OUT_DIR)