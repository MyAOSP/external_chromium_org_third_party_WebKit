# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := GYP
LOCAL_MODULE := third_party_WebKit_Source_devtools_supported_css_properties_gyp
LOCAL_MODULE_STEM := supported_css_properties
LOCAL_MODULE_SUFFIX := .stamp
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_TARGET_ARCH := $(TARGET_$(GYP_VAR_PREFIX)ARCH)
gyp_intermediate_dir := $(call local-intermediates-dir,,$(GYP_VAR_PREFIX))
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared,,,$(GYP_VAR_PREFIX))

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES :=

### Rules for action "generateSupportedCSSProperties":
$(gyp_shared_intermediate_dir)/blink/SupportedCSSProperties.js: gyp_local_path := $(LOCAL_PATH)
$(gyp_shared_intermediate_dir)/blink/SupportedCSSProperties.js: gyp_intermediate_dir := $(abspath $(gyp_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/SupportedCSSProperties.js: gyp_shared_intermediate_dir := $(abspath $(gyp_shared_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/SupportedCSSProperties.js: export PATH := $(subst $(ANDROID_BUILD_PATHS),,$(PATH))
$(gyp_shared_intermediate_dir)/blink/SupportedCSSProperties.js: $(LOCAL_PATH)/third_party/WebKit/Source/devtools/scripts/generate_supported_css.py $(LOCAL_PATH)/third_party/WebKit/Source/core/css/CSSPropertyNames.in $(LOCAL_PATH)/third_party/WebKit/Source/core/css/SVGCSSPropertyNames.in $(LOCAL_PATH)/third_party/WebKit/Source/core/css/CSSShorthands.in $(GYP_TARGET_DEPENDENCIES)
	@echo "Gyp action: Generating supported CSS properties for front end ($@)"
	$(hide)cd $(gyp_local_path)/third_party/WebKit/Source/devtools; mkdir -p $(gyp_shared_intermediate_dir)/blink; python scripts/generate_supported_css.py ../core/css/CSSPropertyNames.in ../core/css/SVGCSSPropertyNames.in ../core/css/CSSShorthands.in "$(gyp_shared_intermediate_dir)/blink/SupportedCSSProperties.js"



GYP_GENERATED_OUTPUTS := \
	$(gyp_shared_intermediate_dir)/blink/SupportedCSSProperties.js

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

### Rules for final target.
# Add target alias to "gyp_all_modules" target.
.PHONY: gyp_all_modules
gyp_all_modules: third_party_WebKit_Source_devtools_supported_css_properties_gyp

# Alias gyp target name.
.PHONY: supported_css_properties
supported_css_properties: third_party_WebKit_Source_devtools_supported_css_properties_gyp

LOCAL_MODULE_PATH := $(PRODUCT_OUT)/gyp_stamp
LOCAL_UNINSTALLABLE_MODULE := true
LOCAL_2ND_ARCH_VAR_PREFIX := $(GYP_VAR_PREFIX)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(LOCAL_ADDITIONAL_DEPENDENCIES)
	$(hide) echo "Gyp timestamp: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@

LOCAL_2ND_ARCH_VAR_PREFIX :=
