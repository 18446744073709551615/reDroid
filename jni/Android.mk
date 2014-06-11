# Copyright (C) 2009 The Android Open Source Project
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
#
LOCAL_PATH := $(call my-dir)

## this is how we can build multiple .so libraries
#include $(CLEAR_VARS)
#LOCAL_MODULE := xlog
#LOCAL_SRC_FILES += xlog/xlog.c
#LOCAL_LDLIBS := -llog
#include $(BUILD_SHARED_LIBRARY)    # this builds libxlog.so



include $(CLEAR_VARS)
LOCAL_MODULE    := example
#LOCAL_SHARED_LIBRARIES := xlog    # this makes libmod1.so dependent on libmod2.so
LOCAL_SRC_FILES := \
  reDroid/nowchart.c reDroid/dump.cpp reDroid/re_qobj.c reDroid/re_rtti.c reDroid/re_tools.c reDroid/re_encounter.c re_mprot.c
# linking with a library in NDK
LOCAL_LDLIBS := -llog
# a hack to link against Qt
LOCAL_LDLIBS += -L$(LOCAL_PATH)/lib -lQtCore
LOCAL_CPPFLAGS += -fexceptions
include $(BUILD_SHARED_LIBRARY)    # this actually builds libexample.so

# ... add more such libraries ...