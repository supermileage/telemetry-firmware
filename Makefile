
IMAGE := ghcr.io/supermileage/particle-v3.0.0:latest
OUTPUT_DIR := output

include src/build.mk
include test/test.mk

$(info CPPSRC is $(CPPSRC))
$(info TEST_SRC is $(TEST_SRC))

# TEST pattern rules for generating object and dependency files
$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(BUILD_DIR)%.o: $(LIB_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(BUILD_DIR)%.o: $(TEST_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(DEP_DIR)%.d:
	@mkdir -p $(@D)

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

pull-image:
	docker pull $(IMAGE)

libwiringgcc :
	@echo ' *** building $@ *** '
	@cd test/external/UnitTestLib && make libwiringgcc.a

clean:
	rm -rf $(OUTPUT_DIR)

clean-test :
	@rm -r test/obj
	@rm -r test/bin
	@rm -r test/dep

define print
	@echo ''
	@echo '*** $(1) ***'
	@echo ''
endef