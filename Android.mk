# Copyright 2013 The Android Open Source Project
#
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

LOCAL_SRC_FILES := \
	src/AVCPlayer.cpp \
	src/AVCBuffer.cpp \
        main.cpp
LOCAL_SHARED_LIBRARIES := \
	libstagefright libmedia libutils libbinder libstagefright_foundation \
	libgui libcutils liblog libEGL libGLESv2

LOCAL_C_INCLUDES := \
	frameworks/av/media/libstagefright \
	frameworks/av/media/libstagefright/include \
	$(TOP)/frameworks/native/include/media/openmax \

LOCAL_CFLAGS += -Wno-multichar
#LOCAL_CFLAGS += -UNDEBUG

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= AVCPlayer

include $(BUILD_EXECUTABLE)