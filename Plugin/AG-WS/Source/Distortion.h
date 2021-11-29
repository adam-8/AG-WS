/*
  ==============================================================================

    Distortion.h
    Created: 20 Nov 2021 7:22:35pm
    Author:  agyu5

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Identifiers.h"

class Distortion
{
public:
	Distortion(AudioProcessorValueTreeState& vt);
	~Distortion();
	void prepare(dsp::ProcessSpec);
	void reset();
	void process(dsp::ProcessContextReplacing<float> block);
	void updateParameters();

private:
	AudioProcessorValueTreeState& mParameters;
	static constexpr size_t numWaveShapers = 1;
	dsp::WaveShaper<float> mWaveShapers[numWaveShapers];
	//dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> mLowPassFilter, mHighPassFilter;
	std::unique_ptr<dsp::Oversampling<float>> mOversampling;
	dsp::Gain<float> mInputVolume, mOutputVolume;

	float mSampleRate = 44100.f;
	uint32 mMaxBlockSize = 512;
	uint32 mNumChannels = 2;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Distortion)
};
