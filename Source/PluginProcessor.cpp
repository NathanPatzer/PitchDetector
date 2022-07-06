/*
  ==============================================================================

    Audio Side of Plugin

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PAutoAudioProcessor::PAutoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),apvts (*this,nullptr,"Parameters",createParameters())
#endif
{
    pat = new patFFT(44100);
}

PAutoAudioProcessor::~PAutoAudioProcessor()
{
}

//==============================================================================
const juce::String PAutoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PAutoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PAutoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PAutoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PAutoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PAutoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PAutoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PAutoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PAutoAudioProcessor::getProgramName (int index)
{
    return {};
}

void PAutoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PAutoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PAutoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PAutoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PAutoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (totalNumInputChannels > 0)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            
                for (int i = 0; i < buffer.getNumSamples(); i++)
                {
                    if (channel == 0) 
                    {
                        pat->pushNextSampleIntoFifo(channelData[i]);
                        if (pat->isBufferFull())
                        {
                            m_Note = pat->getNote();
                        }
                    }
                    channelData[i] = buffer.getSample(channel, i) * getMasterVolume();
                }
        }
    }
}

//==============================================================================
bool PAutoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PAutoAudioProcessor::createEditor()
{
    return new PAutoAudioProcessorEditor (*this);
}

//==============================================================================
void PAutoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PAutoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PAutoAudioProcessor();
}

//allows for parameters to be updated on the editor from the audio processor
juce::AudioProcessorValueTreeState::ParameterLayout PAutoAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERVOLUME", "MasterVolueme", -48.0f, 0.0f, -1.0f));

    return { params.begin(),params.end() };
}


//returns master volume in decibels using the equation
//10^(rawMaster / 20)
float PAutoAudioProcessor::getMasterVolume()
{
    auto g = apvts.getRawParameterValue("MASTERVOLUME");
    float rawMaster = g->load();

    if (rawMaster == -48.0f)
    {
        return 0.0f;
    }
    else
    {
        return pow(10, rawMaster / 20.0f);
    }
    
}