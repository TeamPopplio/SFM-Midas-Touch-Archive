#pragma once

#include <SPTLib\memutils.hpp>
#include <SPTLib\patterns.hpp>
#include <array>
#include <cstddef>

namespace patterns
{
	namespace ifm
	{
		// TODO: Find a better pattern for this.
		PATTERNS(SFMLoaded,
			"5841",
			"A1 ?? ?? ?? ?? 85 C0 75 ?? 68 58 02 00 00 E8 ?? ?? ?? ?? 83 C4 04 85 C0 74 ?? 8B C8 E8 ?? ?? ?? ?? 50");
	} // namespace ifm
} // namespace patterns
