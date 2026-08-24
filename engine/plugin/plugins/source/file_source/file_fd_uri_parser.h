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

#ifndef HISTREAMER_FILE_FD_URI_PARSER_H
#define HISTREAMER_FILE_FD_URI_PARSER_H

#include <cstdint>
#include <string>

namespace OHOS {
namespace Media {
namespace Plugin {
namespace FileFdSource {
/**
 * Parsed fields from an fd URI: fd://<fd> or fd://<fd>?offset=<off>&size=<sz>
 * Host-testable without the rest of histreamer.
 */
struct FdUriFields {
    int32_t fd {-1};
    int64_t offset {0};
    int64_t size {0};
    bool hasRange {false};
};

/**
 * Parse fd URI fields. Never throws; returns false on empty, malformed, or
 * non-numeric input (caller maps that to Status::ERROR_INVALID_PARAMETER).
 */
bool ParseFdUriFields(const std::string& uri, FdUriFields& out);
} // namespace FileFdSource
} // namespace Plugin
} // namespace Media
} // namespace OHOS

#endif // HISTREAMER_FILE_FD_URI_PARSER_H
