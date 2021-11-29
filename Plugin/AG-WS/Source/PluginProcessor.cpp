/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AGWSAudioProcessor::AGWSAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    mParameters(*this, nullptr, Identifier("Distortion"),
        {
          std::make_unique<AudioParameterFloat>(IDs::inputVolume,
                                                "DIST",
                                                NormalisableRange<float>(0.f, 50.f),
                                                0.f,
                                                " dB",
                                                AudioProcessorParameter::genericParameter,
                                                [](float value, int) {return static_cast<String>(round(value * 100.f) / 100.f); },
                                                nullptr
                                                ),
        }),
    mDistortion(mParameters)
#endif
{
}

AGWSAudioProcessor::~AGWSAudioProcessor()
{
}

//==============================================================================
const juce::String AGWSAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AGWSAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AGWSAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AGWSAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AGWSAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AGWSAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AGWSAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AGWSAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AGWSAudioProcessor::getProgramName (int index)
{
    return {};
}

void AGWSAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AGWSAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    mDistortion.prepare(spec);
}

void AGWSAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AGWSAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AGWSAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto numSamples = buffer.getNumSamples();

    for (auto i = jmin(2, totalNumInputChannels); i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    mDistortion.updateParameters();

    dsp::AudioBlock<float> block(buffer);

    if (block.getNumChannels() > 2)
        block = block.getSubsetChannelBlock(0, 2);

    mDistortion.process(dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool AGWSAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AGWSAudioProcessor::createEditor()
{
    return new AGWSAudioProcessorEditor (*this);
}

//==============================================================================
void AGWSAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = mParameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AGWSAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mParameters.state.getType()))
			mParameters.replaceState(ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState& AGWSAudioProcessor::getState()
{
    return mParameters;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AGWSAudioProcessor();
}
