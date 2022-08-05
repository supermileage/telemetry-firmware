
IMAGE := ghcr.io/supermileage/particle-v3.0.0:latest
OUTPUT_DIR := output

include src/build.mk
include test/test.mk

CPPSRC += $(TEST_CPP)

$(info TEST_CPP is $(TEST_CPP))
$(info TEST_OBJ is $(TEST_OBJ))
$(info SRC_DIR is $(SRC_DIR))
$(info TEST_DIR is $(TEST_DIR))
$(info OBJ_DIR is $(OBJ_DIR))
$(info BIN_DIR is $(BIN_DIR))
$(info DEP_DIR is $(DEP_DIR))

.PHONY: urban proto pull-image clean clean-test

urban: clean pull-image
	$(call print, COMPILING URBAN FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=boron APPDIR=/app/ TARGET_DIR=/$(OUTPUT_DIR) EXTRA_CFLAGS+='-DURBAN $(EXTRA_CFLAGS)'
	$(call print, TAKING OWNERSHIP OF FILES - YOU MAY NEED YOUR PASSWORD)
	sudo chown -R $(shell id -u):$(shell id -g) $(OUTPUT_DIR)

proto: clean pull-image
	$(call print, COMPILING PROTO FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=boron APPDIR=/app/ TARGET_DIR=/$(OUTPUT_DIR) EXTRA_CFLAGS+='-DPROTO $(EXTRA_CFLAGS)'
	$(call print, TAKING OWNERSHIP OF FILES - YOU MAY NEED YOUR PASSWORD)
	sudo chown -R $(shell id -u):$(shell id -g) $(OUTPUT_DIR)

fc: clean pull-image
	$(call print, COMPILING FC FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=boron APPDIR=/app/ TARGET_DIR=/$(OUTPUT_DIR) EXTRA_CFLAGS+='-DFC $(EXTRA_CFLAGS)'
	$(call print, TAKING OWNERSHIP OF FILES - YOU MAY NEED YOUR PASSWORD)
	sudo chown -R $(shell id -u):$(shell id -g) $(OUTPUT_DIR)

test : $(TEST_OBJ) $(BIN_DIR) libwiringgcc
	@echo ' *** Building $@ *** '
	@g++ $(LFLAGS) $(TEST_OBJ) $(INCLUDE_FLAGS) -o $(BIN_DIR)$@

run-tests:
	@chmod +x ./test/bin/test
	./test/bin/test

pull-image:
	docker pull $(IMAGE)

libwiringgcc:
	@echo ' *** building $@ *** '
	@cd test/external/UnitTestLib && make libwiringgcc.a

clean:
	rm -rf $(OUTPUT_DIR)

clean-test:
	@rm -rf test/obj
	@rm -rf test/bin
	@rm -rf test/dep

define print
	@echo ''
	@echo '*** $(1) ***'
	@echo ''
endef