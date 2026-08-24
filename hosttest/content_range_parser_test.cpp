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

#include <iostream>
#include <string>

using OHOS::Media::Plugin::HttpPlugin::ContentRangeFields;
using OHOS::Media::Plugin::HttpPlugin::ParseContentRange;

namespace {
int g_failures = 0;

void ExpectOk(const char* label, const char* strRange, size_t start, size_t end, size_t fileLen)
{
    ContentRangeFields fields;
    fields.start = 111; // sentinel: must be overwritten on success
    fields.end = 222;
    fields.fileLen = 333;
    if (!ParseContentRange(strRange, fields)) {
        std::cerr << "FAIL " << label << ": expected OK for " << strRange << std::endl;
        ++g_failures;
        return;
    }
    if (fields.start != start || fields.end != end || fields.fileLen != fileLen) {
        std::cerr << "FAIL " << label << ": got start=" << fields.start << " end=" << fields.end
                  << " fileLen=" << fields.fileLen << std::endl;
        ++g_failures;
        return;
    }
    std::cout << "PASS " << label << std::endl;
}

void ExpectError(const char* label, const char* strRange)
{
    ContentRangeFields fields;
    const size_t sentinelStart = 111;
    const size_t sentinelEnd = 222;
    const size_t sentinelLen = 333;
    fields.start = sentinelStart;
    fields.end = sentinelEnd;
    fields.fileLen = sentinelLen;
    if (ParseContentRange(strRange, fields)) {
        std::cerr << "FAIL " << label << ": expected error for "
                  << (strRange == nullptr ? "(null)" : strRange) << std::endl;
        ++g_failures;
        return;
    }
    if (fields.start != sentinelStart || fields.end != sentinelEnd || fields.fileLen != sentinelLen) {
        std::cerr << "FAIL " << label << ": wrote outputs on failure start=" << fields.start
                  << " end=" << fields.end << " fileLen=" << fields.fileLen << std::endl;
        ++g_failures;
        return;
    }
    std::cout << "PASS " << label << std::endl;
}
} // namespace

int main()
{
    ExpectOk("full-range", "bytes 0-499/1234", 0, 499, 1234);
    ExpectOk("nonzero-start", "bytes 100-199/200", 100, 199, 200);
    ExpectOk("zero-file", "bytes 0-0/0", 0, 0, 0);

    // Old gate (ret != -1) accepted these partial/zero matches and then
    // assigned uninitialized fileLen to HeaderInfo::fileContentLen.
    ExpectError("partial-start-end", "bytes 0-499");
    ExpectError("partial-start", "bytes 0");
    ExpectError("no-numbers", "bytes abc");
    ExpectError("unknown-total", "bytes 0-499/*");
    ExpectError("star-range", "bytes */1234");
    ExpectError("empty", "");
    ExpectError("null", nullptr);
    ExpectError("wrong-unit", "items 0-1/2");
    ExpectError("negative-start", "bytes -1-10/20");
    ExpectError("negative-end", "bytes 0--1/10");

    if (g_failures != 0) {
        std::cerr << g_failures << " test(s) failed" << std::endl;
        return 1;
    }
    std::cout << "All host tests passed" << std::endl;
    return 0;
}
