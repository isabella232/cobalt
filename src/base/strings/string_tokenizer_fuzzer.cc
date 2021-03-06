// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>

#include "base/strings/string_tokenizer.h"
#include "starboard/types.h"

void GetAllTokens(base::StringTokenizer& t) {
  while (t.GetNext()) {
    (void)t.token();
  }
}

// Entry point for LibFuzzer.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  uint8_t size_t_bytes = sizeof(size_t);
  if (size < size_t_bytes + 1) {
    return 0;
  }

  // Calculate pattern size based on remaining bytes, otherwise fuzzing is
  // inefficient with bailouts in most cases.
  size_t pattern_size =
      *reinterpret_cast<const size_t*>(data) % (size - size_t_bytes);

  std::string pattern(reinterpret_cast<const char*>(data + size_t_bytes),
                      pattern_size);
  std::string input(
      reinterpret_cast<const char*>(data + size_t_bytes + pattern_size),
      size - pattern_size - size_t_bytes);

  // Allow quote_chars and options to be set. Otherwise full coverage
  // won't be possible since IsQuote, FullGetNext and other functions
  // won't be called.
  base::StringTokenizer t(input, pattern);
  GetAllTokens(t);

  base::StringTokenizer t_quote(input, pattern);
  t_quote.set_quote_chars("\"");
  GetAllTokens(t_quote);

  base::StringTokenizer t_options(input, pattern);
  t_options.set_options(base::StringTokenizer::RETURN_DELIMS);
  GetAllTokens(t_options);

  base::StringTokenizer t_quote_and_options(input, pattern);
  t_quote_and_options.set_quote_chars("\"");
  t_quote_and_options.set_options(base::StringTokenizer::RETURN_DELIMS);
  GetAllTokens(t_quote_and_options);

  return 0;
}
