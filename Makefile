IMAGE := ghcr.io/supermileage/particle-v3.0.0:latest
OUTPUT_DIR := output

.PHONY: all pull-image clean

all: clean pull-image
	$(call print, COMPILING TELEMETRY FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=boron APPDIR=/app/ TARGET_DIR=/$(OUTPUT_DIR)
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
