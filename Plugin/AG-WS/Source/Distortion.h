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
	std::unique_ptr<dsp::Oversampling<float>> mOversampling;
	dsp::Gain<float> mInputVolume;

	float mSampleRate = 44100.f;
	uint32 mMaxBlockSize = 512;
	uint32 mNumChannels = 2;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Distortion)
};
