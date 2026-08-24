#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${TMPDIR:-/tmp}/content_range_parser_test"
g++ -std=c++17 -O0 -Wall -Wextra \
  -I"${ROOT}/engine/plugin/plugins/source/http_source/download" \
  "${ROOT}/hosttest/content_range_parser_test.cpp" \
  "${ROOT}/engine/plugin/plugins/source/http_source/download/content_range_parser.cpp" \
  -o "${OUT}"
"${OUT}"
