
IMAGE := ghcr.io/supermileage/particle-v3.0.0:latest
OUTPUT_DIR := output

include src/build.mk

.PHONY: urban proto pull-image clean

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

pull-image:
	docker pull $(IMAGE)

clean:
	rm -rf $(OUTPUT_DIR)


define print
	@echo ''
	@echo '*** $(1) ***'
	@echo ''
endef