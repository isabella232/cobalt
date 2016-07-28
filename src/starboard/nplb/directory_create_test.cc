// Copyright 2015 Google Inc. All Rights Reserved.
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

#include <string>

#include "starboard/directory.h"
#include "starboard/file.h"
#include "starboard/nplb/file_helpers.h"
#include "starboard/system.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace starboard {
namespace nplb {
namespace {

const char* kManyFileSeparators =
    SB_FILE_SEP_STRING SB_FILE_SEP_STRING SB_FILE_SEP_STRING SB_FILE_SEP_STRING;

// NOTE: There is a test missing here, for creating a directory right off of the
// root. But, this is likely to fail due to permissions, so we can't make a
// reliable test for this.

TEST(SbDirectoryCreateTest, SunnyDay) {
  ScopedRandomFile dir(ScopedRandomFile::kDontCreate);

  const std::string& path = dir.filename();

  EXPECT_FALSE(SbDirectoryCanOpen(path.c_str()));
  EXPECT_TRUE(SbDirectoryCreate(path.c_str()));
  EXPECT_TRUE(SbDirectoryCanOpen(path.c_str()));

  // Should return true if called redundantly.
  EXPECT_TRUE(SbDirectoryCreate(path.c_str()));
  EXPECT_TRUE(SbDirectoryCanOpen(path.c_str()));
}

TEST(SbDirectoryCreateTest, SunnyDayTrailingSeparators) {
  ScopedRandomFile dir(ScopedRandomFile::kDontCreate);

  std::string path = dir.filename() + kManyFileSeparators;

  EXPECT_FALSE(SbDirectoryCanOpen(path.c_str()));
  EXPECT_TRUE(SbDirectoryCreate(path.c_str()));
  EXPECT_TRUE(SbDirectoryCanOpen(path.c_str()));
}

TEST(SbDirectoryCreateTest, SunnyDayRoot) {
  const char* kRootPath = SB_FILE_SEP_STRING;
  EXPECT_TRUE(SbDirectoryCanOpen(kRootPath));
  EXPECT_TRUE(SbDirectoryCreate(kRootPath));
  EXPECT_TRUE(SbDirectoryCanOpen(kRootPath));

  EXPECT_TRUE(SbDirectoryCanOpen(kManyFileSeparators));
  EXPECT_TRUE(SbDirectoryCreate(kManyFileSeparators));
  EXPECT_TRUE(SbDirectoryCanOpen(kManyFileSeparators));
}

TEST(SbDirectoryCreateTest, FailureNullPath) {
  EXPECT_FALSE(SbDirectoryCreate(NULL));
}

TEST(SbDirectoryCreateTest, FailureEmptyPath) {
  EXPECT_FALSE(SbDirectoryCreate(""));
}

TEST(SbDirectoryCreateTest, FailureNonexistentParent) {
  ScopedRandomFile dir(ScopedRandomFile::kDontCreate);
  std::string path = dir.filename() + SB_FILE_SEP_STRING "test";

  EXPECT_FALSE(SbDirectoryCanOpen(path.c_str()));
  EXPECT_FALSE(SbDirectoryCreate(path.c_str()));
  EXPECT_FALSE(SbDirectoryCanOpen(path.c_str()));
}

TEST(SbDirectoryCreateTest, FailureNotAbsolute) {
  const char* kPath = "hallo";

  EXPECT_FALSE(SbDirectoryCanOpen(kPath));
  EXPECT_FALSE(SbDirectoryCreate(kPath));
  EXPECT_FALSE(SbDirectoryCanOpen(kPath));
}

}  // namespace
}  // namespace nplb
}  // namespace starboard
