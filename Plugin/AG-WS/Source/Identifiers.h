#pragma once

namespace IDs {

	#define DECLARE_ID(name) const String name (#name);
	DECLARE_ID(inputVolume)			// Input volume of the effects chain
	#undef DECLARE_ID

}
