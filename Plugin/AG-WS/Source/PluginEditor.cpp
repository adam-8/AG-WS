/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AGWSAudioProcessorEditor::AGWSAudioProcessorEditor (AGWSAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), mParameter(p.getState())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(350, 150);
    initialiseGUI();
}

AGWSAudioProcessorEditor::~AGWSAudioProcessorEditor()
{
    // Set the LOF of all sliders to nullptr
    mInputVolumeSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void AGWSAudioProcessorEditor::paint (juce::Graphics& g)
{
	g.fillAll(Colours::chocolate);
}

void AGWSAudioProcessorEditor::resized()
{

	// DIST ==================================
	FlexBox gainBox;
	gainBox.alignContent = FlexBox::AlignContent::center;
	gainBox.justifyContent = FlexBox::JustifyContent::center;
	gainBox.flexDirection = FlexBox::Direction::column;
	gainBox.items.addArray({
							 makeLabel(mInputVolumeLabel),
							 makeRotarySlider(mInputVolumeSlider)
		});

	// MASTER ================================
	FlexBox masterBox;
	masterBox.alignContent = FlexBox::AlignContent::center;
	masterBox.justifyContent = FlexBox::JustifyContent::spaceAround;
	masterBox.flexDirection = FlexBox::Direction::row;
	masterBox.items.addArray({
							   FlexItem(gainBox).withFlex(1)
		});

	masterBox.performLayout(getLocalBounds().reduced(20, 20).toFloat());
}

FlexItem AGWSAudioProcessorEditor::makeRotarySlider(Component & c)
{
	return FlexItem(c).withMinWidth(static_cast<float>(mSliderSize)).withMinHeight(static_cast<float>(mSliderSize));
}

FlexItem AGWSAudioProcessorEditor::makeLabel(Component & c)
{
	return FlexItem(c).withMinWidth(static_cast<float>(c.getWidth())).withMinHeight(20);
}

void AGWSAudioProcessorEditor::initialiseGUI()
{
	// INPUT VOLUME ================================
	// Label
	mInputVolumeLabel.setText("Distortion", dontSendNotification);
	mInputVolumeLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mInputVolumeLabel);
	// Slider
	mInputVolumeSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
	mInputVolumeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mInputVolumeSlider.setLookAndFeel(&knobLookAndFeel);
	mInputVolumeSlider.setTextValueSuffix(" dB");
	mInputVolumeAttachment.reset(new SliderAttachment(mParameter, IDs::inputVolume, mInputVolumeSlider));
	addAndMakeVisible(mInputVolumeSlider);
}