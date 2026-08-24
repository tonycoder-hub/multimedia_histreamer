/*
 * Copyright (c) 2021-2026 Huawei Device Co., Ltd.
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

#include "file_fd_uri_parser.h"

#include <exception>
#include <iostream>
#include <string>

using OHOS::Media::Plugin::FileFdSource::FdUriFields;
using OHOS::Media::Plugin::FileFdSource::ParseFdUriFields;

namespace {
int g_failures = 0;

void ExpectOk(const char* label, const std::string& uri, int32_t fd, bool hasRange,
              int64_t offset = 0, int64_t size = 0)
{
    try {
        FdUriFields fields;
        if (!ParseFdUriFields(uri, fields)) {
            std::cerr << "FAIL " << label << ": expected OK for " << uri << std::endl;
            ++g_failures;
            return;
        }
        if (fields.fd != fd || fields.hasRange != hasRange ||
            (hasRange && (fields.offset != offset || fields.size != size))) {
            std::cerr << "FAIL " << label << ": unexpected fields fd=" << fields.fd
                      << " hasRange=" << fields.hasRange << " offset=" << fields.offset
                      << " size=" << fields.size << std::endl;
            ++g_failures;
            return;
        }
        std::cout << "PASS " << label << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "FAIL " << label << ": threw " << e.what() << std::endl;
        ++g_failures;
    }
}

void ExpectError(const char* label, const std::string& uri)
{
    try {
        FdUriFields fields;
        if (ParseFdUriFields(uri, fields)) {
            std::cerr << "FAIL " << label << ": expected error for " << uri << std::endl;
            ++g_failures;
            return;
        }
        std::cout << "PASS " << label << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "FAIL " << label << ": threw instead of error: " << e.what() << std::endl;
        ++g_failures;
    }
}
} // namespace

int main()
{
    ExpectOk("query-fd", "fd://42?offset=1&size=2", 42, true, 1, 2);
    ExpectOk("bare-fd", "fd://42", 42, false);
    ExpectOk("zero-range", "fd://0?offset=0&size=0", 0, true, 0, 0);
    ExpectError("non-numeric-fd", "fd://abc");
    ExpectError("empty", "");
    ExpectError("missing-fd", "fd://");
    ExpectError("malformed-no-size", "fd://42?offset=1");
    ExpectError("malformed-offset", "fd://42?offset=x&size=2");
    ExpectError("overflow-fd", "fd://999999999999999999999");
    ExpectError("wrong-scheme", "file://42?offset=1&size=2");

    if (g_failures != 0) {
        std::cerr << g_failures << " test(s) failed" << std::endl;
        return 1;
    }
    std::cout << "All host tests passed" << std::endl;
    return 0;
}
