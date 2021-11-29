/*
  ==============================================================================

    Identifiers.h
    Created: 20 Nov 2021 7:23:03pm
    Author:  agyu5

  ==============================================================================
*/

#pragma once

namespace IDs {

	#define DECLARE_ID(name) const String name (#name);
	DECLARE_ID(inputVolume)			// Input volume of the effects chain
	#undef DECLARE_ID

}