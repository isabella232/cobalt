/*
 * Copyright 2015 Google Inc. All Rights Reserved.
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

#include "cobalt/loader/loader.h"

#include "base/bind.h"
#include "base/file_util.h"
#include "base/message_loop.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "cobalt/loader/file_fetcher.h"
#include "cobalt/loader/text_decoder.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::_;

namespace cobalt {
namespace loader {
namespace {

//////////////////////////////////////////////////////////////////////////
// Callbacks
//////////////////////////////////////////////////////////////////////////

class TextDecoderCallback {
 public:
  explicit TextDecoderCallback(base::RunLoop* run_loop) : run_loop_(run_loop) {}

  void OnDone(const std::string& text) {
    text_ = text;
    MessageLoop::current()->PostTask(FROM_HERE, run_loop_->QuitClosure());
  }

  std::string text() { return text_; }

 private:
  base::RunLoop* run_loop_;
  std::string text_;
};

class LoaderCallback {
 public:
  LoaderCallback() : run_loop_(NULL) {}
  explicit LoaderCallback(base::RunLoop* run_loop) : run_loop_(run_loop) {}

  void OnError(const std::string& text) {
    DLOG(ERROR) << text;
    if (run_loop_)
      MessageLoop::current()->PostTask(FROM_HERE, run_loop_->QuitClosure());
  }

 private:
  base::RunLoop* run_loop_;
};

//////////////////////////////////////////////////////////////////////////
// Mocks & Stubs
//////////////////////////////////////////////////////////////////////////

class StubFetcherError : public Fetcher {
 public:
  explicit StubFetcherError(Handler* handler) : Fetcher(handler) {
    handler->OnError(this, "Fake error for test");
  }

  static scoped_ptr<Fetcher> Create(Handler* handler) {
    return scoped_ptr<Fetcher>(new StubFetcherError(handler));
  }
};

class StubFetcherReceivedDone : public Fetcher {
 public:
  explicit StubFetcherReceivedDone(Handler* handler) : Fetcher(handler) {
    handler->OnReceived(this, NULL, 0);
    handler->OnDone(this);
  }

  static scoped_ptr<Fetcher> Create(Handler* handler) {
    return scoped_ptr<Fetcher>(new StubFetcherReceivedDone(handler));
  }
};

class MockDecoder : public Decoder {
 public:
  MOCK_METHOD2(DecodeChunk, void(const char*, size_t));
  MOCK_METHOD0(Finish, void());
};

class MockLoaderCallback : public LoaderCallback {
 public:
  MockLoaderCallback() {
    ON_CALL(*this, OnError(_))
        .WillByDefault(Invoke(&real_, &LoaderCallback::OnError));
  }

  MOCK_METHOD1(OnError, void(const std::string&));

 private:
  LoaderCallback real_;
};

}  // namespace

//////////////////////////////////////////////////////////////////////////
// LoaderTest
//////////////////////////////////////////////////////////////////////////

class LoaderTest : public ::testing::Test {
 protected:
  LoaderTest();
  ~LoaderTest() OVERRIDE {}

  FilePath data_dir_;
  MessageLoop message_loop_;
};

LoaderTest::LoaderTest() : message_loop_(MessageLoop::TYPE_DEFAULT) {
  data_dir_ = data_dir_.Append(FILE_PATH_LITERAL("cobalt"))
                  .Append(FILE_PATH_LITERAL("loader"))
                  .Append(FILE_PATH_LITERAL("testdata"));
}

TEST_F(LoaderTest, FetcherError) {
  MockLoaderCallback mock_loader_callback;
  MockDecoder* mock_decoder = new MockDecoder();  // To be owned by loader.
  EXPECT_CALL(*mock_decoder, DecodeChunk(_, _)).Times(0);
  EXPECT_CALL(*mock_decoder, Finish()).Times(0);
  EXPECT_CALL(mock_loader_callback, OnError(_));

  Loader loader(base::Bind(&StubFetcherError::Create),
                scoped_ptr<Decoder>(mock_decoder),
                base::Bind(&MockLoaderCallback::OnError,
                           base::Unretained(&mock_loader_callback)));
}

TEST_F(LoaderTest, FetcherReceiveDone) {
  InSequence dummy;

  MockLoaderCallback mock_loader_callback;
  MockDecoder* mock_decoder = new MockDecoder();  // To be owned by loader.
  EXPECT_CALL(mock_loader_callback, OnError(_)).Times(0);
  EXPECT_CALL(*mock_decoder, DecodeChunk(_, _));
  EXPECT_CALL(*mock_decoder, Finish());

  Loader loader(base::Bind(&StubFetcherReceivedDone::Create),
                scoped_ptr<Decoder>(mock_decoder),
                base::Bind(&MockLoaderCallback::OnError,
                           base::Unretained(&mock_loader_callback)));
}

// Typical usage of Loader.
TEST_F(LoaderTest, ValidFileEndToEndTest) {
  // Create a RunLoop that helps us use the MessageLoop, which is in the test
  // fixture object.
  base::RunLoop run_loop;

  // Create a loader, using a FileFetcher that loads from disk, and a
  // TextDecoder that sees the received bytes as plain text.
  const FilePath file_path = data_dir_.Append("performance-spike.html");
  FileFetcher::Options fetcher_options;
  TextDecoderCallback text_decoder_callback(&run_loop);
  LoaderCallback loader_callback(&run_loop);
  Loader loader(
      base::Bind(&FileFetcher::Create, file_path, fetcher_options),
      scoped_ptr<Decoder>(new TextDecoder(
          base::Bind(&TextDecoderCallback::OnDone,
                     base::Unretained(&text_decoder_callback)))),
      base::Bind(&LoaderCallback::OnError, base::Unretained(&loader_callback)));

  // When the message loop runs, the loader will start loading. It'll quit when
  // loading is finished.
  run_loop.Run();

  // Get the loaded result from the decoder callback.
  std::string loaded_text = text_decoder_callback.text();

  // Compare the result with that of other API's.
  std::string expected_text;
  FilePath dir_source_root;
  EXPECT_TRUE(PathService::Get(base::DIR_SOURCE_ROOT, &dir_source_root));
  EXPECT_TRUE(file_util::ReadFileToString(dir_source_root.Append(file_path),
                                          &expected_text));
  EXPECT_EQ(expected_text, loaded_text);
}

}  // namespace loader
}  // namespace cobalt
