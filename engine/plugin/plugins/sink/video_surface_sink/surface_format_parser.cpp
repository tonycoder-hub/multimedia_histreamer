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
#include <string>

namespace OHOS {
namespace Media {
namespace Plugin {
namespace VidSurfaceSinkPlugin {
bool ParseSurfaceFormat(const std::string& text, int32_t& value)
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
} // namespace VidSurfaceSinkPlugin
} // namespace Plugin
} // namespace Media
} // namespace OHOS
