/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PAutoAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
                                   
{
public:
    PAutoAudioProcessorEditor (PAutoAudioProcessor&);
    ~PAutoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void setNoteLabel(std::string note);


    

private:

    //used to call setNote every 60hz
    void timerCallback() override
    {
        std::string processorNote = audioProcessor.getNote();
        setNoteLabel(processorNote);
    }

    //note that is displayed on the screen
    std::string m_Note = "";

    //label that is tied to m_Note
    juce::Label noteLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PAutoAudioProcessor& audioProcessor;

    //master slider
    juce::Slider master_Slider;

    //attaches slider to audio processor
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PAutoAudioProcessorEditor)
};
