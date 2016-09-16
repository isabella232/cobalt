/*
 * Copyright 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MEDIA_FILTERS_SHELL_RAW_VIDEO_DECODER_LINUX_H_
#define MEDIA_FILTERS_SHELL_RAW_VIDEO_DECODER_LINUX_H_

#include "base/memory/scoped_ptr.h"
#include "media/base/shell_video_data_allocator.h"
#include "media/base/video_decoder_config.h"
#include "media/filters/shell_video_decoder_impl.h"

namespace media {

scoped_ptr<ShellRawVideoDecoder> CreateShellRawVideoDecoderLinux(
    ShellVideoDataAllocator* allocator,
    const VideoDecoderConfig& config,
    Decryptor* decryptor,
    bool was_encrypted);

}  // namespace media

#endif  // MEDIA_FILTERS_SHELL_RAW_VIDEO_DECODER_LINUX_H_