IMAGE := ghcr.io/supermileage/particle-v1.5.2:latest
OUTPUT_DIR := output

.PHONY: urban proto pull-image clean

urban: pull-image
	$(call print, COMPILING URBAN FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=electron APPDIR=/app/urban TARGET_DIR=/$(OUTPUT_DIR)
	$(call print, TAKING OWNERSHIP OF FILES - YOU MAY NEED YOUR PASSWORD)
	sudo chown -R $(shell id -u):$(shell id -g) $(OUTPUT_DIR)

proto: pull-image
	$(call print, COMPILING PROTO FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=electron APPDIR=/app/proto TARGET_DIR=/$(OUTPUT_DIR)
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