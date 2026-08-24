#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${TMPDIR:-/tmp}/surface_format_parser_test"
g++ -std=c++17 -O0 -Wall -Wextra \
  -I"${ROOT}/engine/plugin/plugins/sink/video_surface_sink" \
  "${ROOT}/hosttest/surface_format_parser_test.cpp" \
  "${ROOT}/engine/plugin/plugins/sink/video_surface_sink/surface_format_parser.cpp" \
  -o "${OUT}"
"${OUT}"
