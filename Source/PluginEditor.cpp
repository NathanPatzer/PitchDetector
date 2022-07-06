/*
  ==============================================================================

    GUI Side of Plugin

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PAutoAudioProcessorEditor::PAutoAudioProcessorEditor (PAutoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    startTimerHz(60);
    
    noteLabel.setFont(juce::Font(50.0f, juce::Font::bold));
    noteLabel.setText("note", juce::dontSendNotification);
    noteLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
    noteLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(noteLabel);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    //master slider
    master_Slider.setSliderStyle(juce::Slider::LinearHorizontal);
    master_Slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
    addAndMakeVisible(master_Slider);
    masterSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"MASTERVOLUME",master_Slider);

    setSize(800, 500);
}

PAutoAudioProcessorEditor::~PAutoAudioProcessorEditor()
{
}

//==============================================================================
void PAutoAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void PAutoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::Rectangle<int> masterBounds = getLocalBounds();
    
    masterBounds.setSize(100,50);
    noteLabel.setBounds(getLocalBounds());
    
    master_Slider.setBounds(masterBounds);
}

void PAutoAudioProcessorEditor::setNoteLabel(std::string note)
{
    noteLabel.setText(note,juce::dontSendNotification);
}