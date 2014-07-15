# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE := third_party_WebKit_Source_wtf_wtf_gyp
LOCAL_MODULE_SUFFIX := .a
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_TARGET_ARCH := $(TARGET_$(GYP_VAR_PREFIX)ARCH)
gyp_intermediate_dir := $(call local-intermediates-dir,,$(GYP_VAR_PREFIX))
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared,,,$(GYP_VAR_PREFIX))

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES := \
	$(call intermediates-dir-for,GYP,third_party_WebKit_Source_wtf_wtf_config_gyp,,,$(GYP_VAR_PREFIX))/wtf_config.stamp \
	$(call intermediates-dir-for,GYP,third_party_WebKit_Source_config_gyp,,,$(GYP_VAR_PREFIX))/config.stamp \
	$(call intermediates-dir-for,GYP,third_party_icu_icui18n_gyp,,,$(GYP_VAR_PREFIX))/icui18n.stamp \
	$(call intermediates-dir-for,GYP,third_party_icu_icuuc_gyp,,,$(GYP_VAR_PREFIX))/icuuc.stamp

GYP_GENERATED_OUTPUTS :=

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

$(gyp_intermediate_dir)/bignum-dtoa.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/bignum-dtoa.cc
	mkdir -p $(@D); cp $< $@
$(gyp_intermediate_dir)/bignum.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/bignum.cc
	mkdir -p $(@D); cp $< $@
$(gyp_intermediate_dir)/cached-powers.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/cached-powers.cc
	mkdir -p $(@D); cp $< $@
$(gyp_intermediate_dir)/diy-fp.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/diy-fp.cc
	mkdir -p $(@D); cp $< $@
$(gyp_intermediate_dir)/double-conversion.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/double-conversion.cc
	mkdir -p $(@D); cp $< $@
$(gyp_intermediate_dir)/fast-dtoa.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/fast-dtoa.cc
	mkdir -p $(@D); cp $< $@
$(gyp_intermediate_dir)/fixed-dtoa.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/fixed-dtoa.cc
	mkdir -p $(@D); cp $< $@
$(gyp_intermediate_dir)/strtod.cpp: $(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa/strtod.cc
	mkdir -p $(@D); cp $< $@
LOCAL_GENERATED_SOURCES := \
	$(gyp_intermediate_dir)/bignum-dtoa.cpp \
	$(gyp_intermediate_dir)/bignum.cpp \
	$(gyp_intermediate_dir)/cached-powers.cpp \
	$(gyp_intermediate_dir)/diy-fp.cpp \
	$(gyp_intermediate_dir)/double-conversion.cpp \
	$(gyp_intermediate_dir)/fast-dtoa.cpp \
	$(gyp_intermediate_dir)/fixed-dtoa.cpp \
	$(gyp_intermediate_dir)/strtod.cpp

GYP_COPIED_SOURCE_ORIGIN_DIRS := \
	$(LOCAL_PATH)/third_party/WebKit/Source/wtf/dtoa

LOCAL_SRC_FILES := \
	third_party/WebKit/Source/wtf/ArrayBuffer.cpp \
	third_party/WebKit/Source/wtf/ArrayBufferBuilder.cpp \
	third_party/WebKit/Source/wtf/ArrayBufferContents.cpp \
	third_party/WebKit/Source/wtf/ArrayBufferView.cpp \
	third_party/WebKit/Source/wtf/ArrayPiece.cpp \
	third_party/WebKit/Source/wtf/Assertions.cpp \
	third_party/WebKit/Source/wtf/BitVector.cpp \
	third_party/WebKit/Source/wtf/CryptographicallyRandomNumber.cpp \
	third_party/WebKit/Source/wtf/CurrentTime.cpp \
	third_party/WebKit/Source/wtf/DataLog.cpp \
	third_party/WebKit/Source/wtf/DateMath.cpp \
	third_party/WebKit/Source/wtf/DefaultAllocator.cpp \
	third_party/WebKit/Source/wtf/DynamicAnnotations.cpp \
	third_party/WebKit/Source/wtf/FastMalloc.cpp \
	third_party/WebKit/Source/wtf/FilePrintStream.cpp \
	third_party/WebKit/Source/wtf/HashTable.cpp \
	third_party/WebKit/Source/wtf/InstanceCounter.cpp \
	third_party/WebKit/Source/wtf/MainThread.cpp \
	third_party/WebKit/Source/wtf/NullPtr.cpp \
	third_party/WebKit/Source/wtf/PageAllocator.cpp \
	third_party/WebKit/Source/wtf/PartitionAlloc.cpp \
	third_party/WebKit/Source/wtf/PrintStream.cpp \
	third_party/WebKit/Source/wtf/RefCountedLeakCounter.cpp \
	third_party/WebKit/Source/wtf/SizeLimits.cpp \
	third_party/WebKit/Source/wtf/ThreadIdentifierDataPthreads.cpp \
	third_party/WebKit/Source/wtf/Threading.cpp \
	third_party/WebKit/Source/wtf/ThreadingPthreads.cpp \
	third_party/WebKit/Source/wtf/TypeTraits.cpp \
	third_party/WebKit/Source/wtf/WTF.cpp \
	third_party/WebKit/Source/wtf/WTFThreadData.cpp \
	third_party/WebKit/Source/wtf/dtoa.cpp \
	third_party/WebKit/Source/wtf/text/AtomicString.cpp \
	third_party/WebKit/Source/wtf/text/Base64.cpp \
	third_party/WebKit/Source/wtf/text/CString.cpp \
	third_party/WebKit/Source/wtf/text/StringBuilder.cpp \
	third_party/WebKit/Source/wtf/text/StringImpl.cpp \
	third_party/WebKit/Source/wtf/text/StringStatics.cpp \
	third_party/WebKit/Source/wtf/text/TextCodec.cpp \
	third_party/WebKit/Source/wtf/text/TextCodecICU.cpp \
	third_party/WebKit/Source/wtf/text/TextCodecLatin1.cpp \
	third_party/WebKit/Source/wtf/text/TextCodecReplacement.cpp \
	third_party/WebKit/Source/wtf/text/TextCodecUTF16.cpp \
	third_party/WebKit/Source/wtf/text/TextCodecUTF8.cpp \
	third_party/WebKit/Source/wtf/text/TextCodecUserDefined.cpp \
	third_party/WebKit/Source/wtf/text/TextEncoding.cpp \
	third_party/WebKit/Source/wtf/text/TextEncodingRegistry.cpp \
	third_party/WebKit/Source/wtf/text/TextPosition.cpp \
	third_party/WebKit/Source/wtf/text/WTFString.cpp \
	third_party/WebKit/Source/wtf/unicode/UTF8.cpp \
	third_party/WebKit/Source/wtf/unicode/icu/CollatorICU.cpp


# Flags passed to both C and C++ files.
MY_CFLAGS_Debug := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	 \
	-fno-exceptions \
	-fno-strict-aliasing \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wno-unused-local-typedefs \
	-EL \
	-mhard-float \
	-ffunction-sections \
	-funwind-tables \
	-g \
	-fstack-protector \
	-fno-short-enums \
	-finline-limit=64 \
	-Wa,--noexecstack \
	-U_FORTIFY_SOURCE \
	-Wno-extra \
	-Wno-ignored-qualifiers \
	-Wno-type-limits \
	-Wno-unused-but-set-variable \
	-Os \
	-g \
	-fdata-sections \
	-ffunction-sections \
	-fomit-frame-pointer \
	-funwind-tables

MY_DEFS_Debug := \
	'-DV8_DEPRECATION_WARNINGS' \
	'-DBLINK_SCALE_FILTERS_AT_RECORD_TIME' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DNO_TCMALLOC' \
	'-DDISABLE_NACL' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DENABLE_WEBRTC=1' \
	'-DUSE_PROPRIETARY_CODECS' \
	'-DENABLE_BROWSER_CDMS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DDISCARDABLE_MEMORY_ALWAYS_SUPPORTED_NATIVELY' \
	'-DSYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE' \
	'-DENABLE_EGLIMAGE=1' \
	'-DCLD_VERSION=1' \
	'-DENABLE_PRINTING=1' \
	'-DENABLE_MANAGED_USERS=1' \
	'-DDATA_REDUCTION_FALLBACK_HOST="http://compress.googlezip.net:80/"' \
	'-DDATA_REDUCTION_DEV_HOST="http://proxy-dev.googlezip.net:80/"' \
	'-DSPDY_PROXY_AUTH_ORIGIN="https://proxy.googlezip.net:443/"' \
	'-DDATA_REDUCTION_PROXY_PROBE_URL="http://check.googlezip.net/connect"' \
	'-DDATA_REDUCTION_PROXY_WARMUP_URL="http://www.gstatic.com/generate_204"' \
	'-DVIDEO_HOLE=1' \
	'-DWTF_IMPLEMENTATION=1' \
	'-DENABLE_CUSTOM_SCHEME_HANDLER=0' \
	'-DENABLE_SVG_FONTS=1' \
	'-DWTF_USE_CONCATENATED_IMPULSE_RESPONSES=1' \
	'-DENABLE_MEDIA_CAPTURE=1' \
	'-DWTF_USE_WEBAUDIO_OPENMAX_DL_FFT=1' \
	'-DENABLE_WEB_AUDIO=1' \
	'-DENABLE_OPENTYPE_VERTICAL=1' \
	'-DU_USING_ICU_NAMESPACE=0' \
	'-DUSE_OPENSSL=1' \
	'-DUSE_OPENSSL_CERTS=1' \
	'-D__STDC_CONSTANT_MACROS' \
	'-D__STDC_FORMAT_MACROS' \
	'-DANDROID' \
	'-D__GNU_SOURCE=1' \
	'-DUSE_STLPORT=1' \
	'-D_STLP_USE_PTR_SPECIALIZATIONS=1' \
	'-DCHROME_BUILD_ID=""' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=1' \
	'-DWTF_USE_DYNAMIC_ANNOTATIONS=1' \
	'-D_DEBUG'


# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES_Debug := \
	$(gyp_shared_intermediate_dir)/shim_headers/icuuc/target \
	$(gyp_shared_intermediate_dir)/shim_headers/icui18n/target \
	$(gyp_shared_intermediate_dir) \
	$(LOCAL_PATH)/third_party/WebKit/Source \
	$(LOCAL_PATH)/third_party/WebKit \
	$(PWD)/external/icu4c/common \
	$(PWD)/external/icu4c/i18n \
	$(PWD)/frameworks/wilhelm/include \
	$(PWD)/bionic \
	$(PWD)/external/stlport/stlport


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Debug := \
	-Wno-c++0x-compat \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wsign-compare \
	-Wno-c++0x-compat \
	-Wno-uninitialized \
	-Wno-non-virtual-dtor \
	-Wno-sign-promo


# Flags passed to both C and C++ files.
MY_CFLAGS_Release := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	 \
	-fno-exceptions \
	-fno-strict-aliasing \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wno-unused-local-typedefs \
	-EL \
	-mhard-float \
	-ffunction-sections \
	-funwind-tables \
	-g \
	-fstack-protector \
	-fno-short-enums \
	-finline-limit=64 \
	-Wa,--noexecstack \
	-U_FORTIFY_SOURCE \
	-Wno-extra \
	-Wno-ignored-qualifiers \
	-Wno-type-limits \
	-Wno-unused-but-set-variable \
	-Os \
	-fno-ident \
	-fdata-sections \
	-ffunction-sections \
	-fomit-frame-pointer \
	-funwind-tables

MY_DEFS_Release := \
	'-DV8_DEPRECATION_WARNINGS' \
	'-DBLINK_SCALE_FILTERS_AT_RECORD_TIME' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DNO_TCMALLOC' \
	'-DDISABLE_NACL' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DENABLE_WEBRTC=1' \
	'-DUSE_PROPRIETARY_CODECS' \
	'-DENABLE_BROWSER_CDMS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DDISCARDABLE_MEMORY_ALWAYS_SUPPORTED_NATIVELY' \
	'-DSYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE' \
	'-DENABLE_EGLIMAGE=1' \
	'-DCLD_VERSION=1' \
	'-DENABLE_PRINTING=1' \
	'-DENABLE_MANAGED_USERS=1' \
	'-DDATA_REDUCTION_FALLBACK_HOST="http://compress.googlezip.net:80/"' \
	'-DDATA_REDUCTION_DEV_HOST="http://proxy-dev.googlezip.net:80/"' \
	'-DSPDY_PROXY_AUTH_ORIGIN="https://proxy.googlezip.net:443/"' \
	'-DDATA_REDUCTION_PROXY_PROBE_URL="http://check.googlezip.net/connect"' \
	'-DDATA_REDUCTION_PROXY_WARMUP_URL="http://www.gstatic.com/generate_204"' \
	'-DVIDEO_HOLE=1' \
	'-DWTF_IMPLEMENTATION=1' \
	'-DENABLE_CUSTOM_SCHEME_HANDLER=0' \
	'-DENABLE_SVG_FONTS=1' \
	'-DWTF_USE_CONCATENATED_IMPULSE_RESPONSES=1' \
	'-DENABLE_MEDIA_CAPTURE=1' \
	'-DWTF_USE_WEBAUDIO_OPENMAX_DL_FFT=1' \
	'-DENABLE_WEB_AUDIO=1' \
	'-DENABLE_OPENTYPE_VERTICAL=1' \
	'-DU_USING_ICU_NAMESPACE=0' \
	'-DUSE_OPENSSL=1' \
	'-DUSE_OPENSSL_CERTS=1' \
	'-D__STDC_CONSTANT_MACROS' \
	'-D__STDC_FORMAT_MACROS' \
	'-DANDROID' \
	'-D__GNU_SOURCE=1' \
	'-DUSE_STLPORT=1' \
	'-D_STLP_USE_PTR_SPECIALIZATIONS=1' \
	'-DCHROME_BUILD_ID=""' \
	'-DNDEBUG' \
	'-DNVALGRIND' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=0' \
	'-D_FORTIFY_SOURCE=2'


# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES_Release := \
	$(gyp_shared_intermediate_dir)/shim_headers/icuuc/target \
	$(gyp_shared_intermediate_dir)/shim_headers/icui18n/target \
	$(gyp_shared_intermediate_dir) \
	$(LOCAL_PATH)/third_party/WebKit/Source \
	$(LOCAL_PATH)/third_party/WebKit \
	$(PWD)/external/icu4c/common \
	$(PWD)/external/icu4c/i18n \
	$(PWD)/frameworks/wilhelm/include \
	$(PWD)/bionic \
	$(PWD)/external/stlport/stlport


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Release := \
	-Wno-c++0x-compat \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wsign-compare \
	-Wno-c++0x-compat \
	-Wno-uninitialized \
	-Wno-non-virtual-dtor \
	-Wno-sign-promo


LOCAL_CFLAGS := $(MY_CFLAGS_$(GYP_CONFIGURATION)) $(MY_DEFS_$(GYP_CONFIGURATION))
LOCAL_C_INCLUDES := $(GYP_COPIED_SOURCE_ORIGIN_DIRS) $(LOCAL_C_INCLUDES_$(GYP_CONFIGURATION))
LOCAL_CPPFLAGS := $(LOCAL_CPPFLAGS_$(GYP_CONFIGURATION))
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)
### Rules for final target.

LOCAL_LDFLAGS_Debug := \
	-Wl,-z,now \
	-Wl,-z,relro \
	-Wl,--fatal-warnings \
	-Wl,-z,noexecstack \
	-fPIC \
	-EL \
	-Wl,--no-keep-memory \
	-nostdlib \
	-Wl,--no-undefined \
	-Wl,--exclude-libs=ALL \
	-Wl,--warn-shared-textrel \
	-Wl,-O1 \
	-Wl,--as-needed


LOCAL_LDFLAGS_Release := \
	-Wl,-z,now \
	-Wl,-z,relro \
	-Wl,--fatal-warnings \
	-Wl,-z,noexecstack \
	-fPIC \
	-EL \
	-Wl,--no-keep-memory \
	-nostdlib \
	-Wl,--no-undefined \
	-Wl,--exclude-libs=ALL \
	-Wl,-O1 \
	-Wl,--as-needed \
	-Wl,--gc-sections \
	-Wl,--warn-shared-textrel


LOCAL_LDFLAGS := $(LOCAL_LDFLAGS_$(GYP_CONFIGURATION))

LOCAL_STATIC_LIBRARIES :=

# Enable grouping to fix circular references
LOCAL_GROUP_STATIC_LIBRARIES := true

LOCAL_SHARED_LIBRARIES := \
	libstlport \
	libdl

# Add target alias to "gyp_all_modules" target.
.PHONY: gyp_all_modules
gyp_all_modules: third_party_WebKit_Source_wtf_wtf_gyp

# Alias gyp target name.
.PHONY: wtf
wtf: third_party_WebKit_Source_wtf_wtf_gyp

include $(BUILD_STATIC_LIBRARY)
