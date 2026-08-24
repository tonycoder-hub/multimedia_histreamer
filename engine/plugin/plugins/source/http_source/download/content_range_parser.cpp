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

#include "content_range_parser.h"

#include <limits>

#if defined(__has_include)
#if __has_include("securec.h")
#include "securec.h"
#define HISTREAMER_CONTENT_RANGE_SSCANF sscanf_s
#endif
#endif

#ifndef HISTREAMER_CONTENT_RANGE_SSCANF
#include <cstdio>
#define HISTREAMER_CONTENT_RANGE_SSCANF sscanf
#endif

namespace OHOS {
namespace Media {
namespace Plugin {
namespace HttpPlugin {
namespace {
bool FitsSizeT(long long value)
{
    if (value < 0) {
        return false;
    }
    return static_cast<unsigned long long>(value) <=
           static_cast<unsigned long long>(std::numeric_limits<size_t>::max());
}
} // namespace

bool ParseContentRange(const char* strRange, ContentRangeFields& out)
{
    if (strRange == nullptr || strRange[0] == '\0') {
        return false;
    }
    // Scan into long long first: "%ld" vs size_t* is a type mismatch, and a
    // gate of != -1 accepts partial conversions (0/1/2) leaving later fields
    // uninitialized. Require all three fields.
    long long start = 0;
    long long end = 0;
    long long fileLen = 0;
    int matched = HISTREAMER_CONTENT_RANGE_SSCANF(strRange, "bytes %lld-%lld/%lld", &start, &end, &fileLen);
    if (matched != 3) { // 3: start, end, fileLen
        return false;
    }
    if (!FitsSizeT(start) || !FitsSizeT(end) || !FitsSizeT(fileLen)) {
        return false;
    }
    out.start = static_cast<size_t>(start);
    out.end = static_cast<size_t>(end);
    out.fileLen = static_cast<size_t>(fileLen);
    return true;
}
} // namespace HttpPlugin
} // namespace Plugin
} // namespace Media
} // namespace OHOS
