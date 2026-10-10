#pragma once

#include "Trade.h"
#include "BookSnapshot.h"

#include <optional>
#include <string>

std::string formatDebugDisplay(const BookSnapshot& snapshot, const std::optional<Trade>& lastTrade);