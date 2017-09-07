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

#include "starboard/player.h"

#include "starboard/log.h"
#include "starboard/shared/starboard/player/player_internal.h"

void SbPlayerWriteSample(SbPlayer player,
                         SbMediaType sample_type,
#if SB_API_VERSION >= SB_PLAYER_WRITE_SAMPLE_EXTRA_CONST_API_VERSION
                         const void* const* sample_buffers,
                         const int* sample_buffer_sizes,
#else   // SB_API_VERSION >= SB_PLAYER_WRITE_SAMPLE_EXTRA_CONST_API_VERSION
                         const void** sample_buffers,
                         int* sample_buffer_sizes,
#endif  // SB_API_VERSION >= SB_PLAYER_WRITE_SAMPLE_EXTRA_CONST_API_VERSION
                         int number_of_sample_buffers,
                         SbMediaTime sample_pts,
                         const SbMediaVideoSampleInfo* video_sample_info,
                         const SbDrmSampleInfo* sample_drm_info) {
  if (!SbPlayerIsValid(player)) {
    SB_DLOG(WARNING) << "player is invalid.";
    return;
  }

  if (number_of_sample_buffers < 1) {
    SB_DLOG(WARNING) << "SbPlayerWriteSample() doesn't support"
                     << " |number_of_sample_buffers| less than one.";
    return;
  }

  if (sample_buffers == NULL) {
    SB_DLOG(WARNING) << "|sample_buffers| cannot be NULL";
    return;
  }

  if (sample_buffer_sizes == NULL) {
    SB_DLOG(WARNING) << "|sample_buffer_sizes| cannot be NULL";
    return;
  }

  player->WriteSample(sample_type, sample_buffers, sample_buffer_sizes,
                      number_of_sample_buffers, sample_pts, video_sample_info,
                      sample_drm_info);
}
