# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ifeq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \>= 28)))
    LOCAL_CFLAGS += -DAVS90
endif

ifeq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \>= 29)))
    LOCAL_CFLAGS += -DAVS10
endif

ifeq (1,$(strip $(shell expr $(PLATFORM_SDK_VERSION) \>= 30)))
    LOCAL_CFLAGS += -DAVS11
    LOCAL_HEADER_LIBRARIES := \
            libmediadrm_headers \
	    libmediametrics_headers \

endif

LOCAL_SRC_FILES := \
	src/AVCPlayer.cpp \
	src/AVCBuffer.cpp \
        main.cpp
LOCAL_SHARED_LIBRARIES := \
	libstagefright libmedia libmedia_omx libutils libbinder libstagefright_foundation \
	libgui libui libcutils liblog libEGL libGLESv2

LOCAL_C_INCLUDES := \
	frameworks/av/media/libstagefright \
	frameworks/av/media/libstagefright/include \
	$(TOP)/frameworks/native/include/media/openmax \

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
#LOCAL_CFLAGS += -UNDEBUG


LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= AVCPlayer

include $(BUILD_EXECUTABLE)
