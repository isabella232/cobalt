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

#include <vector>

#include "starboard/blitter.h"
#include "starboard/nplb/blitter_helpers.h"
#include "starboard/window.h"
#include "testing/gtest/include/gtest/gtest.h"

#if SB_HAS(BLITTER)

namespace starboard {
namespace nplb {
namespace {

TEST(SbBlitterCreatePixelDataTest, SunnyDay) {
  SbBlitterDevice device = SbBlitterCreateDefaultDevice();
  ASSERT_TRUE(SbBlitterIsDeviceValid(device));

  const int kWidth = 128;
  const int kHeight = 128;

  // Test that we can successfully create pixel data in all supported formats.
  std::vector<SbBlitterPixelDataFormat> supported_formats =
      GetAllSupportedPixelFormatsForPixelData(device);
  for (std::vector<SbBlitterPixelDataFormat>::const_iterator iter =
           supported_formats.begin();
       iter != supported_formats.end(); ++iter) {
    SbBlitterPixelData pixel_data =
        SbBlitterCreatePixelData(device, kWidth, kHeight, *iter);
    EXPECT_TRUE(SbBlitterIsPixelDataValid(pixel_data));
    EXPECT_TRUE(SbBlitterDestroyPixelData(pixel_data));
  }

  EXPECT_TRUE(SbBlitterDestroyDevice(device));
}

TEST(SbBlitterCreatePixelDataTest, RainyDayInvalidDevice) {
  const int kWidth = 128;
  const int kHeight = 128;

  // If an invalid device is provided, then false should always be returned.
  for (int i = 0; i < kSbBlitterNumPixelDataFormats; ++i) {
    SbBlitterPixelData pixel_data =
        SbBlitterCreatePixelData(kSbBlitterInvalidDevice, kWidth, kHeight,
                                 static_cast<SbBlitterPixelDataFormat>(i));
    EXPECT_FALSE(SbBlitterIsPixelDataValid(pixel_data));
  }
}

TEST(SbBlitterCreatePixelDataTest, RainyDayUnsupportedFormats) {
  SbBlitterDevice device = SbBlitterCreateDefaultDevice();
  ASSERT_TRUE(SbBlitterIsDeviceValid(device));

  const int kWidth = 128;
  const int kHeight = 128;

  // Test that we are unsuccessful at creating pixel data objects in all
  // unsupported formats.
  std::vector<SbBlitterPixelDataFormat> supported_formats =
      GetAllUnsupportedPixelFormatsForPixelData(device);
  for (std::vector<SbBlitterPixelDataFormat>::const_iterator iter =
           supported_formats.begin();
       iter != supported_formats.end(); ++iter) {
    SbBlitterPixelData pixel_data =
        SbBlitterCreatePixelData(device, kWidth, kHeight, *iter);
    EXPECT_FALSE(SbBlitterIsPixelDataValid(pixel_data));
  }

  EXPECT_TRUE(SbBlitterDestroyDevice(device));
}

}  // namespace
}  // namespace nplb
}  // namespace starboard

#endif  // SB_HAS(BLITTER)
