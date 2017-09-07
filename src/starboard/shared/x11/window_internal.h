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

#ifndef STARBOARD_SHARED_X11_WINDOW_INTERNAL_H_
#define STARBOARD_SHARED_X11_WINDOW_INTERNAL_H_

#include <X11/Xlib.h>

#include "starboard/configuration.h"
#include "starboard/shared/internal_only.h"
#include "starboard/shared/starboard/player/video_frame_internal.h"
#include "starboard/window.h"

#if SB_TRUE || SB_IS(PLAYER_PUNCHED_OUT)
#include <X11/extensions/Xrender.h>
#endif  // SB_TRUE ||
        // SB_IS(PLAYER_PUNCHED_OUT)

struct SbWindowPrivate {
  SbWindowPrivate(Display* display, const SbWindowOptions* options);
  ~SbWindowPrivate();

  Window window;

#if SB_TRUE || SB_IS(PLAYER_PUNCHED_OUT)
  typedef ::starboard::shared::starboard::player::VideoFrame VideoFrame;

  // Composites graphics and the given video frame video for this window. In
  // kSbPlayerOutputModePunchOut mode, this is the only way any graphics or
  // video is presented in the window.  The video frame will be rendered
  // according to boundaries specified by the parameters.
  void Composite(int bounds_x,
                 int bounds_y,
                 int bounds_width,
                 int bounds_height,
                 const starboard::scoped_refptr<VideoFrame>& frame);

  // The cached XRender Picture that represents the window that is the
  // destination of the composition.
  Picture window_picture;

  // This pixmap is used to composite video and graphics before rendering onto
  // the actual window. This is necessary to avoid horrible flickering.
  Pixmap composition_pixmap;

  // A cached XRender Picture wrapper for |composition_pixmap|.
  Picture composition_picture;

  // This pixmap is used to load the video frame before stretching it onto the
  // |composition_pixmap|. This is done so that we can stretch it with XRender.
  Pixmap video_pixmap;

  // The width of the |video_pixmap|.
  int video_pixmap_width;

  // The height of the allocated |video_pixmap|.
  int video_pixmap_height;

  // A cached GC for the |video_pixmap| to upload the frame.
  GC video_pixmap_gc;

  // A cached XRender Picture wrapper for |video_pixmap|.
  Picture video_picture;

  // This mapped, but invisible window is bound to EGL and used to get the GL
  // graphics output into an XRender Picture that can be manually composited
  // with video.
  Window gl_window;

  // A cached XRender Picture wrapper for |gl_window|.
  Picture gl_picture;
#endif  // SB_TRUE ||
        // SB_IS(PLAYER_PUNCHED_OUT)

  // The display that this window was created from.
  Display* display;

  // The width of this window.
  int width;

  // The height of this window.
  int height;
};

#endif  // STARBOARD_SHARED_X11_WINDOW_INTERNAL_H_
