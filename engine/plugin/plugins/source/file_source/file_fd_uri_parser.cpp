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
#include <regex>
#include <string>

namespace OHOS {
namespace Media {
namespace Plugin {
namespace FileFdSource {
namespace {
bool ParseInt32(const std::string& text, int32_t& value)
{
    if (text.empty()) {
        return false;
    }
    try {
        size_t idx = 0;
        int parsed = std::stoi(text, &idx, 10);
        if (idx != text.size()) {
            return false;
        }
        value = static_cast<int32_t>(parsed);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool ParseInt64(const std::string& text, int64_t& value)
{
    if (text.empty()) {
        return false;
    }
    try {
        size_t idx = 0;
        int64_t parsed = std::stoll(text, &idx, 10);
        if (idx != text.size()) {
            return false;
        }
        value = parsed;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

// Literal '?' before offset/size. The old pattern ^fd://(.*)?offset= treated '?' as
// an optional-group quantifier, so fd://42?offset=1&size=2 captured "42?" and stoi threw.
const std::regex FD_URI_RE("^fd://([^?]+)(?:\\?offset=([^&]*)&size=(.*))?$");
} // namespace

bool ParseFdUriFields(const std::string& uri, FdUriFields& out)
{
    if (uri.empty()) {
        return false;
    }
    std::smatch match;
    if (!std::regex_match(uri, match, FD_URI_RE)) {
        return false;
    }
    FdUriFields parsed;
    if (!ParseInt32(match[1].str(), parsed.fd)) {
        return false;
    }
    if (match[2].matched) {
        if (!ParseInt64(match[2].str(), parsed.offset) || !ParseInt64(match[3].str(), parsed.size)) {
            return false;
        }
        parsed.hasRange = true;
    }
    out = parsed;
    return true;
}
} // namespace FileFdSource
} // namespace Plugin
} // namespace Media
} // namespace OHOS
