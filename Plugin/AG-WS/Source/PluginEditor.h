/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knobs.h"
#include "Identifiers.h"

//==============================================================================
/**
*/
class AGWSAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AGWSAudioProcessorEditor (AGWSAudioProcessor&);
    ~AGWSAudioProcessorEditor() override;

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    FlexItem makeRotarySlider(Component& c);
    FlexItem makeLabel(Component& c);
    void initialiseGUI();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AGWSAudioProcessor& audioProcessor;

    AudioProcessorValueTreeState& mParameter;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    // Sliders
    Slider mInputVolumeSlider;

    // Labels
    Label mInputVolumeLabel;

    // LAF 
    KnobLookAndFeel knobLookAndFeel;

    int mSliderSize = 70;
    int mTextBoxHeight = 14;
    int mTextBoxWidth = 120;

    std::unique_ptr<SliderAttachment> mInputVolumeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AGWSAudioProcessorEditor)
};
