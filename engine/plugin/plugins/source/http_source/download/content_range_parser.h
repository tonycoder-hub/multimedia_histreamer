/*
 * Copyright (c) 2022-2026 Huawei Device Co., Ltd.
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

#ifndef HISTREAMER_CONTENT_RANGE_PARSER_H
#define HISTREAMER_CONTENT_RANGE_PARSER_H

#include <cstddef>

namespace OHOS {
namespace Media {
namespace Plugin {
namespace HttpPlugin {
/**
 * Parsed fields from a Content-Range value: bytes <start>-<end>/<fileLen>
 * Host-testable without the rest of histreamer.
 */
struct ContentRangeFields {
    size_t start {0};
    size_t end {0};
    size_t fileLen {0};
};

/**
 * Parse a Content-Range header value (after the colon).
 * Requires a full match of start, end, and fileLen (sscanf return == 3).
 * Rejects negatives and values that do not fit in size_t.
 * On failure, out is left unchanged.
 */
bool ParseContentRange(const char* strRange, ContentRangeFields& out);
} // namespace HttpPlugin
} // namespace Plugin
} // namespace Media
} // namespace OHOS

#endif // HISTREAMER_CONTENT_RANGE_PARSER_H
