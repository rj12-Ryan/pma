#pragma once
#include <cstdint>

void DrawStatusBar(const int WindowY, const int WindowX, const int StatusBarHeight);

using BallID = std::uint32_t;
using PegID  = std::uint32_t;
using WallID = std::uint32_t;