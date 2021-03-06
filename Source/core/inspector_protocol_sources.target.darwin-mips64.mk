# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := GYP
LOCAL_MODULE := third_party_WebKit_Source_core_inspector_protocol_sources_gyp
LOCAL_MODULE_STEM := inspector_protocol_sources
LOCAL_MODULE_SUFFIX := .stamp
LOCAL_MODULE_TARGET_ARCH := $(TARGET_$(GYP_VAR_PREFIX)ARCH)
gyp_intermediate_dir := $(call local-intermediates-dir,,$(GYP_VAR_PREFIX))
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared,,,$(GYP_VAR_PREFIX))

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES := \
	$(call intermediates-dir-for,GYP,third_party_WebKit_Source_core_generate_inspector_protocol_version_gyp,,,$(GYP_VAR_PREFIX))/generate_inspector_protocol_version.stamp

### Rules for action "generateInspectorProtocolBackendSources":
$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp: gyp_local_path := $(LOCAL_PATH)
$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp: gyp_var_prefix := $(GYP_VAR_PREFIX)
$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp: gyp_intermediate_dir := $(abspath $(gyp_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp: gyp_shared_intermediate_dir := $(abspath $(gyp_shared_intermediate_dir))
$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp: export PATH := $(subst $(ANDROID_BUILD_PATHS),,$(PATH))
$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/core/inspector/CodeGeneratorInspector.py $(LOCAL_PATH)/third_party/WebKit/Source/core/inspector/CodeGeneratorInspectorStrings.py $(LOCAL_PATH)/third_party/WebKit/Source/devtools/protocol.json $(GYP_TARGET_DEPENDENCIES)
	@echo "Gyp action: Generating Inspector protocol backend sources from protocol.json ($@)"
	$(hide)cd $(gyp_local_path)/third_party/WebKit/Source/core; mkdir -p $(gyp_shared_intermediate_dir)/blink/core; python inspector/CodeGeneratorInspector.py ../devtools/protocol.json --output_dir "$(gyp_shared_intermediate_dir)/blink/core"

$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.h: $(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp ;
$(gyp_shared_intermediate_dir)/blink/core/InspectorFrontend.cpp: $(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp ;
$(gyp_shared_intermediate_dir)/blink/core/InspectorFrontend.h: $(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp ;
$(gyp_shared_intermediate_dir)/blink/core/InspectorTypeBuilder.cpp: $(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp ;
$(gyp_shared_intermediate_dir)/blink/core/InspectorTypeBuilder.h: $(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp ;


GYP_GENERATED_OUTPUTS := \
	$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.cpp \
	$(gyp_shared_intermediate_dir)/blink/core/InspectorBackendDispatcher.h \
	$(gyp_shared_intermediate_dir)/blink/core/InspectorFrontend.cpp \
	$(gyp_shared_intermediate_dir)/blink/core/InspectorFrontend.h \
	$(gyp_shared_intermediate_dir)/blink/core/InspectorTypeBuilder.cpp \
	$(gyp_shared_intermediate_dir)/blink/core/InspectorTypeBuilder.h

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

### Rules for final target.
# Add target alias to "gyp_all_modules" target.
.PHONY: gyp_all_modules
gyp_all_modules: third_party_WebKit_Source_core_inspector_protocol_sources_gyp

# Alias gyp target name.
.PHONY: inspector_protocol_sources
inspector_protocol_sources: third_party_WebKit_Source_core_inspector_protocol_sources_gyp

LOCAL_MODULE_PATH := $(PRODUCT_OUT)/gyp_stamp
LOCAL_UNINSTALLABLE_MODULE := true
LOCAL_2ND_ARCH_VAR_PREFIX := $(GYP_VAR_PREFIX)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(LOCAL_ADDITIONAL_DEPENDENCIES)
	$(hide) echo "Gyp timestamp: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@

LOCAL_2ND_ARCH_VAR_PREFIX :=
