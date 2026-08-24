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

#include "surface_format_parser.h"

#include <exception>
#include <iostream>
#include <string>

using OHOS::Media::Plugin::VidSurfaceSinkPlugin::ParseSurfaceFormat;

namespace {
int g_failures = 0;

void ExpectOk(const char* label, const std::string& text, int32_t expected)
{
    try {
        int32_t value = -1;
        if (!ParseSurfaceFormat(text, value)) {
            std::cerr << "FAIL " << label << ": expected OK for \"" << text << "\"" << std::endl;
            ++g_failures;
            return;
        }
        if (value != expected) {
            std::cerr << "FAIL " << label << ": got " << value << " expected " << expected << std::endl;
            ++g_failures;
            return;
        }
        std::cout << "PASS " << label << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "FAIL " << label << ": threw " << e.what() << std::endl;
        ++g_failures;
    }
}

void ExpectError(const char* label, const std::string& text)
{
    try {
        int32_t value = -1;
        if (ParseSurfaceFormat(text, value)) {
            std::cerr << "FAIL " << label << ": expected error for \"" << text << "\"" << std::endl;
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
    ExpectOk("rgba", "12", 12);
    ExpectOk("zero", "0", 0);
    ExpectOk("negative", "-1", -1);
    ExpectError("empty", "");
    ExpectError("non-numeric", "abc");
    ExpectError("trailing", "12abc");
    ExpectError("overflow", "999999999999999999999");

    if (g_failures != 0) {
        std::cerr << g_failures << " test(s) failed" << std::endl;
        return 1;
    }
    std::cout << "All host tests passed" << std::endl;
    return 0;
}
