# This file is generated by gyp; do not edit.

include $(CLEAR_VARS)

LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE := third_party_WebKit_Source_core_webcore_dom_gyp
LOCAL_MODULE_SUFFIX := .a
LOCAL_MODULE_TAGS := optional
gyp_intermediate_dir := $(call local-intermediates-dir)
gyp_shared_intermediate_dir := $(call intermediates-dir-for,GYP,shared)

# Make sure our deps are built first.
GYP_TARGET_DEPENDENCIES := \
	$(call intermediates-dir-for,GYP,third_party_WebKit_Source_core_webcore_prerequisites_gyp)/webcore_prerequisites.stamp

GYP_GENERATED_OUTPUTS :=

# Make sure our deps and generated files are built first.
LOCAL_ADDITIONAL_DEPENDENCIES := $(GYP_TARGET_DEPENDENCIES) $(GYP_GENERATED_OUTPUTS)

LOCAL_GENERATED_SOURCES :=

GYP_COPIED_SOURCE_ORIGIN_DIRS :=

LOCAL_SRC_FILES := \
	third_party/WebKit/Source/core/dom/ActiveDOMObject.cpp \
	third_party/WebKit/Source/core/dom/AnimationEvent.cpp \
	third_party/WebKit/Source/core/dom/Attr.cpp \
	third_party/WebKit/Source/core/dom/BeforeTextInsertedEvent.cpp \
	third_party/WebKit/Source/core/dom/BeforeUnloadEvent.cpp \
	third_party/WebKit/Source/core/dom/CDATASection.cpp \
	third_party/WebKit/Source/core/dom/CharacterData.cpp \
	third_party/WebKit/Source/core/dom/CheckedRadioButtons.cpp \
	third_party/WebKit/Source/core/dom/ChildListMutationScope.cpp \
	third_party/WebKit/Source/core/dom/ChildNodeList.cpp \
	third_party/WebKit/Source/core/dom/ClassNodeList.cpp \
	third_party/WebKit/Source/core/dom/ClientRect.cpp \
	third_party/WebKit/Source/core/dom/ClientRectList.cpp \
	third_party/WebKit/Source/core/dom/Clipboard.cpp \
	third_party/WebKit/Source/core/dom/ClipboardEvent.cpp \
	third_party/WebKit/Source/core/dom/Comment.cpp \
	third_party/WebKit/Source/core/dom/CompositionEvent.cpp \
	third_party/WebKit/Source/core/dom/ContainerNode.cpp \
	third_party/WebKit/Source/core/dom/ContainerNodeAlgorithms.cpp \
	third_party/WebKit/Source/core/dom/ContextLifecycleObserver.cpp \
	third_party/WebKit/Source/core/dom/ContextFeatures.cpp \
	third_party/WebKit/Source/core/dom/ContextLifecycleNotifier.cpp \
	third_party/WebKit/Source/core/dom/CustomElement.cpp \
	third_party/WebKit/Source/core/dom/CustomElementCallbackDispatcher.cpp \
	third_party/WebKit/Source/core/dom/CustomElementCallbackInvocation.cpp \
	third_party/WebKit/Source/core/dom/CustomElementCallbackQueue.cpp \
	third_party/WebKit/Source/core/dom/CustomElementCallbackScheduler.cpp \
	third_party/WebKit/Source/core/dom/CustomElementDefinition.cpp \
	third_party/WebKit/Source/core/dom/CustomElementRegistrationContext.cpp \
	third_party/WebKit/Source/core/dom/CustomElementRegistry.cpp \
	third_party/WebKit/Source/core/dom/CustomElementUpgradeCandidateMap.cpp \
	third_party/WebKit/Source/core/dom/CustomEvent.cpp \
	third_party/WebKit/Source/core/dom/DOMError.cpp \
	third_party/WebKit/Source/core/dom/DOMException.cpp \
	third_party/WebKit/Source/core/dom/DOMImplementation.cpp \
	third_party/WebKit/Source/core/dom/DOMNamedFlowCollection.cpp \
	third_party/WebKit/Source/core/dom/DOMStringList.cpp \
	third_party/WebKit/Source/core/dom/DOMStringMap.cpp \
	third_party/WebKit/Source/core/dom/DataTransferItem.cpp \
	third_party/WebKit/Source/core/dom/DatasetDOMStringMap.cpp \
	third_party/WebKit/Source/core/dom/DecodedDataDocumentParser.cpp \
	third_party/WebKit/Source/core/dom/DeviceOrientationController.cpp \
	third_party/WebKit/Source/core/dom/Document.cpp \
	third_party/WebKit/Source/core/dom/DocumentEventQueue.cpp \
	third_party/WebKit/Source/core/dom/DocumentFragment.cpp \
	third_party/WebKit/Source/core/dom/DocumentFullscreen.cpp \
	third_party/WebKit/Source/core/dom/DocumentInit.cpp \
	third_party/WebKit/Source/core/dom/DocumentLifecycleObserver.cpp \
	third_party/WebKit/Source/core/dom/DocumentMarkerController.cpp \
	third_party/WebKit/Source/core/dom/DocumentMarker.cpp \
	third_party/WebKit/Source/core/dom/DocumentOrderedList.cpp \
	third_party/WebKit/Source/core/dom/DocumentOrderedMap.cpp \
	third_party/WebKit/Source/core/dom/DocumentParser.cpp \
	third_party/WebKit/Source/core/dom/DocumentSharedObjectPool.cpp \
	third_party/WebKit/Source/core/dom/DocumentStyleSheetCollection.cpp \
	third_party/WebKit/Source/core/dom/DocumentType.cpp \
	third_party/WebKit/Source/core/dom/Element.cpp \
	third_party/WebKit/Source/core/dom/ElementRareData.cpp \
	third_party/WebKit/Source/core/dom/ErrorEvent.cpp \
	third_party/WebKit/Source/core/dom/Event.cpp \
	third_party/WebKit/Source/core/dom/EventContext.cpp \
	third_party/WebKit/Source/core/dom/EventDispatchMediator.cpp \
	third_party/WebKit/Source/core/dom/EventDispatcher.cpp \
	third_party/WebKit/Source/core/dom/EventRetargeter.cpp \
	third_party/WebKit/Source/core/dom/EventListenerMap.cpp \
	third_party/WebKit/Source/core/dom/EventNames.cpp \
	third_party/WebKit/Source/core/dom/EventPathWalker.cpp \
	third_party/WebKit/Source/core/dom/EventTarget.cpp \
	third_party/WebKit/Source/core/dom/FocusEvent.cpp \
	third_party/WebKit/Source/core/dom/FullscreenElementStack.cpp \
	third_party/WebKit/Source/core/dom/GenericEventQueue.cpp \
	third_party/WebKit/Source/core/dom/GestureEvent.cpp \
	third_party/WebKit/Source/core/dom/IconURL.cpp \
	third_party/WebKit/Source/core/dom/IdTargetObserver.cpp \
	third_party/WebKit/Source/core/dom/IdTargetObserverRegistry.cpp \
	third_party/WebKit/Source/core/dom/KeyboardEvent.cpp \
	third_party/WebKit/Source/core/dom/LiveNodeList.cpp \
	third_party/WebKit/Source/core/dom/MessageChannel.cpp \
	third_party/WebKit/Source/core/dom/MessageEvent.cpp \
	third_party/WebKit/Source/core/dom/MessagePort.cpp \
	third_party/WebKit/Source/core/dom/MessagePortChannel.cpp \
	third_party/WebKit/Source/core/dom/Microtask.cpp \
	third_party/WebKit/Source/core/dom/MouseEvent.cpp \
	third_party/WebKit/Source/core/dom/MouseRelatedEvent.cpp \
	third_party/WebKit/Source/core/dom/MutationEvent.cpp \
	third_party/WebKit/Source/core/dom/MutationObserver.cpp \
	third_party/WebKit/Source/core/dom/MutationObserverInterestGroup.cpp \
	third_party/WebKit/Source/core/dom/MutationObserverRegistration.cpp \
	third_party/WebKit/Source/core/dom/MutationRecord.cpp \
	third_party/WebKit/Source/core/dom/NamedFlow.cpp \
	third_party/WebKit/Source/core/dom/NamedFlowCollection.cpp \
	third_party/WebKit/Source/core/dom/NameNodeList.cpp \
	third_party/WebKit/Source/core/dom/NamedNodeMap.cpp \
	third_party/WebKit/Source/core/dom/NamedNodesCollection.cpp \
	third_party/WebKit/Source/core/dom/Node.cpp \
	third_party/WebKit/Source/core/dom/NodeFilter.cpp \
	third_party/WebKit/Source/core/dom/NodeFilterCondition.cpp \
	third_party/WebKit/Source/core/dom/NodeIterator.cpp \
	third_party/WebKit/Source/core/dom/NodeList.cpp \
	third_party/WebKit/Source/core/dom/NodeRareData.cpp \
	third_party/WebKit/Source/core/dom/NodeRenderingContext.cpp \
	third_party/WebKit/Source/core/dom/NodeRenderingTraversal.cpp \
	third_party/WebKit/Source/core/dom/NodeTraversal.cpp \
	third_party/WebKit/Source/core/dom/Notation.cpp \
	third_party/WebKit/Source/core/dom/OverflowEvent.cpp \
	third_party/WebKit/Source/core/dom/PageTransitionEvent.cpp \
	third_party/WebKit/Source/core/dom/PendingScript.cpp \
	third_party/WebKit/Source/core/dom/PopStateEvent.cpp \
	third_party/WebKit/Source/core/dom/Position.cpp \
	third_party/WebKit/Source/core/dom/PositionIterator.cpp \
	third_party/WebKit/Source/core/dom/ProcessingInstruction.cpp \
	third_party/WebKit/Source/core/dom/ProgressEvent.cpp \
	third_party/WebKit/Source/core/dom/PseudoElement.cpp \
	third_party/WebKit/Source/core/dom/QualifiedName.cpp \
	third_party/WebKit/Source/core/dom/Range.cpp \
	third_party/WebKit/Source/core/dom/ResourceProgressEvent.cpp \
	third_party/WebKit/Source/core/dom/ScopedEventQueue.cpp \
	third_party/WebKit/Source/core/dom/ScriptExecutionContext.cpp \
	third_party/WebKit/Source/core/dom/ScriptLoader.cpp \
	third_party/WebKit/Source/core/dom/ScriptRunner.cpp \
	third_party/WebKit/Source/core/dom/ScriptableDocumentParser.cpp \
	third_party/WebKit/Source/core/dom/ScriptedAnimationController.cpp \
	third_party/WebKit/Source/core/dom/SecurityContext.cpp \
	third_party/WebKit/Source/core/dom/SelectorQuery.cpp \
	third_party/WebKit/Source/core/dom/SpaceSplitString.cpp \
	third_party/WebKit/Source/core/dom/StaticNodeList.cpp \
	third_party/WebKit/Source/core/dom/StringCallback.cpp \
	third_party/WebKit/Source/core/dom/StyleElement.cpp \
	third_party/WebKit/Source/core/dom/StyleSheetCollection.cpp \
	third_party/WebKit/Source/core/dom/StyleSheetScopingNodeList.cpp \
	third_party/WebKit/Source/core/dom/TagNodeList.cpp \
	third_party/WebKit/Source/core/dom/Text.cpp \
	third_party/WebKit/Source/core/dom/TextEvent.cpp \
	third_party/WebKit/Source/core/dom/TextLinkColors.cpp \
	third_party/WebKit/Source/core/dom/Touch.cpp \
	third_party/WebKit/Source/core/dom/TouchEvent.cpp \
	third_party/WebKit/Source/core/dom/TouchList.cpp \
	third_party/WebKit/Source/core/dom/TransformSourceLibxslt.cpp \
	third_party/WebKit/Source/core/dom/TransitionEvent.cpp \
	third_party/WebKit/Source/core/dom/Traversal.cpp \
	third_party/WebKit/Source/core/dom/TreeScope.cpp \
	third_party/WebKit/Source/core/dom/TreeScopeAdopter.cpp \
	third_party/WebKit/Source/core/dom/TreeWalker.cpp \
	third_party/WebKit/Source/core/dom/UIEvent.cpp \
	third_party/WebKit/Source/core/dom/UIEventWithKeyState.cpp \
	third_party/WebKit/Source/core/dom/UserActionElementSet.cpp \
	third_party/WebKit/Source/core/dom/UserGestureIndicator.cpp \
	third_party/WebKit/Source/core/dom/UserTypingGestureIndicator.cpp \
	third_party/WebKit/Source/core/dom/ViewportArguments.cpp \
	third_party/WebKit/Source/core/dom/VisitedLinkState.cpp \
	third_party/WebKit/Source/core/dom/WheelEvent.cpp \
	third_party/WebKit/Source/core/dom/WindowEventContext.cpp \
	third_party/WebKit/Source/core/dom/default/chromium/PlatformMessagePortChannelChromium.cpp \
	third_party/WebKit/Source/core/dom/shadow/ComposedShadowTreeWalker.cpp \
	third_party/WebKit/Source/core/dom/shadow/ContentDistributor.cpp \
	third_party/WebKit/Source/core/dom/shadow/ElementShadow.cpp \
	third_party/WebKit/Source/core/dom/shadow/InsertionPoint.cpp \
	third_party/WebKit/Source/core/dom/shadow/SelectRuleFeatureSet.cpp \
	third_party/WebKit/Source/core/dom/shadow/ShadowRoot.cpp


# Flags passed to both C and C++ files.
MY_CFLAGS_Debug := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	-fno-exceptions \
	-fno-strict-aliasing \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wno-format \
	-fno-tree-sra \
	-fuse-ld=gold \
	-Wno-psabi \
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
	-Wno-address \
	-Wno-format-security \
	-Wno-return-type \
	-Wno-sequence-point \
	-Os \
	-g \
	-fomit-frame-pointer \
	-fdata-sections \
	-ffunction-sections

MY_DEFS_Debug := \
	'-DANGLE_DX11' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DNO_TCMALLOC' \
	'-DDISCARDABLE_MEMORY_ALWAYS_SUPPORTED_NATIVELY' \
	'-DSYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE' \
	'-DDISABLE_NACL' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DUSE_PROPRIETARY_CODECS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DENABLE_GPU=1' \
	'-DUSE_OPENSSL=1' \
	'-DENABLE_EGLIMAGE=1' \
	'-DENABLE_PRINTING=1' \
	'-DWEBCORE_NAVIGATOR_VENDOR="Google Inc."' \
	'-DWEBKIT_IMPLEMENTATION=1' \
	'-DINSIDE_WEBKIT' \
	'-DENABLE_CSS3_TEXT=0' \
	'-DENABLE_CSS_EXCLUSIONS=1' \
	'-DENABLE_CSS_REGIONS=1' \
	'-DENABLE_CUSTOM_SCHEME_HANDLER=0' \
	'-DENABLE_ENCRYPTED_MEDIA_V2=1' \
	'-DENABLE_SVG_FONTS=1' \
	'-DENABLE_TOUCH_ICON_LOADING=1' \
	'-DENABLE_GDI_FONTS_ON_WINDOWS=1' \
	'-DWTF_USE_CONCATENATED_IMPULSE_RESPONSES=1' \
	'-DENABLE_CALENDAR_PICKER=0' \
	'-DENABLE_FAST_MOBILE_SCROLLING=1' \
	'-DENABLE_INPUT_SPEECH=0' \
	'-DENABLE_LEGACY_NOTIFICATIONS=0' \
	'-DENABLE_MEDIA_CAPTURE=1' \
	'-DENABLE_NOTIFICATIONS=0' \
	'-DENABLE_ORIENTATION_EVENTS=1' \
	'-DENABLE_NAVIGATOR_CONTENT_UTILS=0' \
	'-DWTF_USE_NATIVE_FULLSCREEN_VIDEO=1' \
	'-DENABLE_OPENTYPE_VERTICAL=1' \
	'-DWTF_USE_HARFBUZZ=1' \
	'-DU_USING_ICU_NAMESPACE=0' \
	'-DCHROME_PNG_WRITE_SUPPORT' \
	'-DPNG_USER_CONFIG' \
	'-DLIBXML_STATIC' \
	'-DLIBXSLT_STATIC' \
	'-DUSE_SYSTEM_LIBJPEG' \
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
	$(gyp_shared_intermediate_dir)/shim_headers/skia_library/target \
	$(gyp_shared_intermediate_dir)/shim_headers/ashmem/target \
	$(gyp_shared_intermediate_dir)/shim_headers/icuuc/target \
	$(gyp_shared_intermediate_dir)/shim_headers/icui18n/target \
	$(LOCAL_PATH)/third_party/khronos \
	$(LOCAL_PATH)/gpu \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/third_party/WebKit \
	$(LOCAL_PATH)/third_party/WebKit/Source \
	$(gyp_shared_intermediate_dir)/blink \
	$(gyp_shared_intermediate_dir)/blink/bindings \
	$(LOCAL_PATH)/third_party/angle_dx11/include/GLSLANG \
	$(PWD)/external/icu4c/common \
	$(PWD)/external/icu4c/i18n \
	$(LOCAL_PATH)/third_party/skia/src/core \
	$(LOCAL_PATH)/skia/ext \
	$(LOCAL_PATH)/third_party/iccjpeg \
	$(LOCAL_PATH)/third_party/libpng \
	$(LOCAL_PATH)/third_party/libwebp \
	$(LOCAL_PATH)/third_party/libxml/linux/include \
	$(LOCAL_PATH)/third_party/libxml/src/include \
	$(LOCAL_PATH)/third_party/libxslt \
	$(LOCAL_PATH)/third_party/npapi \
	$(LOCAL_PATH)/third_party/npapi/bindings \
	$(LOCAL_PATH)/third_party/ots/include \
	$(LOCAL_PATH)/third_party/qcms/src \
	$(LOCAL_PATH)/third_party/sqlite \
	$(LOCAL_PATH)/third_party/zlib \
	$(LOCAL_PATH)/v8/include \
	$(PWD)/external/jpeg \
	$(PWD)/frameworks/wilhelm/include \
	$(PWD)/bionic \
	$(PWD)/external/stlport/stlport


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Debug := \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wno-c++0x-compat \
	-Wno-deprecated \
	-Wno-abi \
	-Wno-error=c++0x-compat \
	-Wno-non-virtual-dtor \
	-Wno-sign-promo \
	-Wno-non-virtual-dtor


# Flags passed to both C and C++ files.
MY_CFLAGS_Release := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	-fno-exceptions \
	-fno-strict-aliasing \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wno-format \
	-fno-tree-sra \
	-fuse-ld=gold \
	-Wno-psabi \
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
	-Wno-address \
	-Wno-format-security \
	-Wno-return-type \
	-Wno-sequence-point \
	-Os \
	-fno-ident \
	-fdata-sections \
	-ffunction-sections \
	-fomit-frame-pointer

MY_DEFS_Release := \
	'-DANGLE_DX11' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DNO_TCMALLOC' \
	'-DDISCARDABLE_MEMORY_ALWAYS_SUPPORTED_NATIVELY' \
	'-DSYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE' \
	'-DDISABLE_NACL' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DUSE_PROPRIETARY_CODECS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DENABLE_GPU=1' \
	'-DUSE_OPENSSL=1' \
	'-DENABLE_EGLIMAGE=1' \
	'-DENABLE_PRINTING=1' \
	'-DWEBCORE_NAVIGATOR_VENDOR="Google Inc."' \
	'-DWEBKIT_IMPLEMENTATION=1' \
	'-DINSIDE_WEBKIT' \
	'-DENABLE_CSS3_TEXT=0' \
	'-DENABLE_CSS_EXCLUSIONS=1' \
	'-DENABLE_CSS_REGIONS=1' \
	'-DENABLE_CUSTOM_SCHEME_HANDLER=0' \
	'-DENABLE_ENCRYPTED_MEDIA_V2=1' \
	'-DENABLE_SVG_FONTS=1' \
	'-DENABLE_TOUCH_ICON_LOADING=1' \
	'-DENABLE_GDI_FONTS_ON_WINDOWS=1' \
	'-DWTF_USE_CONCATENATED_IMPULSE_RESPONSES=1' \
	'-DENABLE_CALENDAR_PICKER=0' \
	'-DENABLE_FAST_MOBILE_SCROLLING=1' \
	'-DENABLE_INPUT_SPEECH=0' \
	'-DENABLE_LEGACY_NOTIFICATIONS=0' \
	'-DENABLE_MEDIA_CAPTURE=1' \
	'-DENABLE_NOTIFICATIONS=0' \
	'-DENABLE_ORIENTATION_EVENTS=1' \
	'-DENABLE_NAVIGATOR_CONTENT_UTILS=0' \
	'-DWTF_USE_NATIVE_FULLSCREEN_VIDEO=1' \
	'-DENABLE_OPENTYPE_VERTICAL=1' \
	'-DWTF_USE_HARFBUZZ=1' \
	'-DU_USING_ICU_NAMESPACE=0' \
	'-DCHROME_PNG_WRITE_SUPPORT' \
	'-DPNG_USER_CONFIG' \
	'-DLIBXML_STATIC' \
	'-DLIBXSLT_STATIC' \
	'-DUSE_SYSTEM_LIBJPEG' \
	'-DANDROID' \
	'-D__GNU_SOURCE=1' \
	'-DUSE_STLPORT=1' \
	'-D_STLP_USE_PTR_SPECIALIZATIONS=1' \
	'-DCHROME_BUILD_ID=""' \
	'-DNDEBUG' \
	'-DNVALGRIND' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=0'


# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES_Release := \
	$(gyp_shared_intermediate_dir)/shim_headers/skia_library/target \
	$(gyp_shared_intermediate_dir)/shim_headers/ashmem/target \
	$(gyp_shared_intermediate_dir)/shim_headers/icuuc/target \
	$(gyp_shared_intermediate_dir)/shim_headers/icui18n/target \
	$(LOCAL_PATH)/third_party/khronos \
	$(LOCAL_PATH)/gpu \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/third_party/WebKit \
	$(LOCAL_PATH)/third_party/WebKit/Source \
	$(gyp_shared_intermediate_dir)/blink \
	$(gyp_shared_intermediate_dir)/blink/bindings \
	$(LOCAL_PATH)/third_party/angle_dx11/include/GLSLANG \
	$(PWD)/external/icu4c/common \
	$(PWD)/external/icu4c/i18n \
	$(LOCAL_PATH)/third_party/skia/src/core \
	$(LOCAL_PATH)/skia/ext \
	$(LOCAL_PATH)/third_party/iccjpeg \
	$(LOCAL_PATH)/third_party/libpng \
	$(LOCAL_PATH)/third_party/libwebp \
	$(LOCAL_PATH)/third_party/libxml/linux/include \
	$(LOCAL_PATH)/third_party/libxml/src/include \
	$(LOCAL_PATH)/third_party/libxslt \
	$(LOCAL_PATH)/third_party/npapi \
	$(LOCAL_PATH)/third_party/npapi/bindings \
	$(LOCAL_PATH)/third_party/ots/include \
	$(LOCAL_PATH)/third_party/qcms/src \
	$(LOCAL_PATH)/third_party/sqlite \
	$(LOCAL_PATH)/third_party/zlib \
	$(LOCAL_PATH)/v8/include \
	$(PWD)/external/jpeg \
	$(PWD)/frameworks/wilhelm/include \
	$(PWD)/bionic \
	$(PWD)/external/stlport/stlport


# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS_Release := \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wno-c++0x-compat \
	-Wno-deprecated \
	-Wno-abi \
	-Wno-error=c++0x-compat \
	-Wno-non-virtual-dtor \
	-Wno-sign-promo \
	-Wno-non-virtual-dtor


LOCAL_CFLAGS := $(MY_CFLAGS_$(GYP_CONFIGURATION)) $(MY_DEFS_$(GYP_CONFIGURATION))
LOCAL_C_INCLUDES := $(GYP_COPIED_SOURCE_ORIGIN_DIRS) $(LOCAL_C_INCLUDES_$(GYP_CONFIGURATION))
LOCAL_CPPFLAGS := $(LOCAL_CPPFLAGS_$(GYP_CONFIGURATION))
### Rules for final target.

LOCAL_LDFLAGS_Debug := \
	-Wl,-z,now \
	-Wl,-z,relro \
	-Wl,-z,noexecstack \
	-fPIC \
	-Wl,-z,relro \
	-Wl,-z,now \
	-fuse-ld=gold \
	-nostdlib \
	-Wl,--no-undefined \
	-Wl,--exclude-libs=ALL \
	-Wl,--icf=safe \
	-Wl,--fatal-warnings \
	-Wl,--gc-sections \
	-Wl,--warn-shared-textrel \
	-Wl,-O1 \
	-Wl,--as-needed


LOCAL_LDFLAGS_Release := \
	-Wl,-z,now \
	-Wl,-z,relro \
	-Wl,-z,noexecstack \
	-fPIC \
	-Wl,-z,relro \
	-Wl,-z,now \
	-fuse-ld=gold \
	-nostdlib \
	-Wl,--no-undefined \
	-Wl,--exclude-libs=ALL \
	-Wl,--icf=safe \
	-Wl,-O1 \
	-Wl,--as-needed \
	-Wl,--gc-sections \
	-Wl,--fatal-warnings \
	-Wl,--warn-shared-textrel


LOCAL_LDFLAGS := $(LOCAL_LDFLAGS_$(GYP_CONFIGURATION))

LOCAL_STATIC_LIBRARIES :=

# Enable grouping to fix circular references
LOCAL_GROUP_STATIC_LIBRARIES := true

LOCAL_SHARED_LIBRARIES := \
	libskia \
	libstlport \
	libdl

# Add target alias to "gyp_all_modules" target.
.PHONY: gyp_all_modules
gyp_all_modules: third_party_WebKit_Source_core_webcore_dom_gyp

# Alias gyp target name.
.PHONY: webcore_dom
webcore_dom: third_party_WebKit_Source_core_webcore_dom_gyp

include $(BUILD_STATIC_LIBRARY)
