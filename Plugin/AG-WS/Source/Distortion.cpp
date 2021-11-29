/*
  ==============================================================================

    WaveShaper.cpp
    Created: 20 Nov 2021 7:22:35pm
    Author:  agyu5

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Distortion.h"

//==============================================================================
Distortion::Distortion(AudioProcessorValueTreeState& vt) : mParameters(vt), mWaveShapers{{std::tanh}}
{
	mOversampling.reset(new dsp::Oversampling<float>(2, 3, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, false));
}

Distortion::~Distortion()
{
	// Empty destructor
}

void Distortion::prepare(dsp::ProcessSpec spec)
{
	// Initialize class variables
	mSampleRate = static_cast<float>(spec.sampleRate);
	mMaxBlockSize = spec.maximumBlockSize;
	mNumChannels = spec.numChannels;
	// Prepare
	mInputVolume.prepare(spec);

	mOversampling->initProcessing(static_cast<size_t> (mMaxBlockSize));
	reset();
}

void Distortion::reset()
{
	mOversampling->reset();
}

void Distortion::process(dsp::ProcessContextReplacing<float> context)
{
	ScopedNoDenormals noDenormals;

	mInputVolume.process(context);

	dsp::AudioBlock<float> oversampledBlock = mOversampling->processSamplesUp(context.getInputBlock());
	auto waveshaperContext = dsp::ProcessContextReplacing<float>(oversampledBlock);

	mWaveShapers[0].process(waveshaperContext);
	waveshaperContext.getOutputBlock() *= 0.7f;

	// downsample
	mOversampling->processSamplesDown(context.getOutputBlock());
}

void Distortion::updateParameters()
{
	float inputVolume = *mParameters.getRawParameterValue(IDs::inputVolume);
	auto inputdB = Decibels::decibelsToGain(inputVolume);
	if (mInputVolume.getGainLinear() != inputdB)	mInputVolume.setGainLinear(inputdB);
}
