#pragma once

#include "Trade.h"
#include "BookSnapshot.h"

#include <optional>
#include <string>
#include <vector>
#include <algorithm>

std::string formatDebugDisplay(const BookSnapshot &snapshot, const std::optional<Trade> &lastTrade);