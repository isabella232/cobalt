// Copyright 2016 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "starboard/blitter.h"
#include "testing/gtest/include/gtest/gtest.h"

#if SB_HAS(BLITTER)

namespace starboard {
namespace nplb {
namespace {

const int kMultipleTimes = 3;

TEST(SbBlitterCreateDefaultDeviceTest, SunnyDay) {
  SbBlitterDevice device = SbBlitterCreateDefaultDevice();
  EXPECT_TRUE(SbBlitterIsDeviceValid(device));
  EXPECT_TRUE(SbBlitterDestroyDevice(device));
}

TEST(SbBlitterCreateDefaultDeviceTest, SunnyDayMultipleTimes) {
  for (int i = 0; i < kMultipleTimes; ++i) {
    SbBlitterDevice device = SbBlitterCreateDefaultDevice();
    EXPECT_TRUE(SbBlitterIsDeviceValid(device));
    EXPECT_TRUE(SbBlitterDestroyDevice(device));
  }
}

TEST(SbBlitterCreateDefaultDeviceTest, RainyDayOnlyOneDefaultDeviceAtATime) {
  SbBlitterDevice device = SbBlitterCreateDefaultDevice();
  EXPECT_TRUE(SbBlitterIsDeviceValid(device));

  SbBlitterDevice device2 = SbBlitterCreateDefaultDevice();
  EXPECT_TRUE(!SbBlitterIsDeviceValid(device2));

  EXPECT_TRUE(SbBlitterDestroyDevice(device));
}

}  // namespace
}  // namespace nplb
}  // namespace starboard

#endif  // SB_HAS(BLITTER)
