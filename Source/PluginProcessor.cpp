/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PS03_graphicEQAudioProcessor::PS03_graphicEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
parameters(*this, nullptr, "Parameters",{
    // Add 10 gain parameters for each EQ band
    std::make_unique<juce::AudioParameterFloat>("GainParam_0", "32Hz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_1", "63Hz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_2", "125Hz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_3", "250Hz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_4", "500Hz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_5", "1kHz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_6", "2kHz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_7", "4kHz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_8", "8kHz Gain", 0.0f, 2.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("GainParam_9", "16kHz Gain", 0.0f, 2.0f, 1.0f)
})
{
    // Example center frequencies for the 10 bands
    std::array<float, 10> centerFrequencies = {32.0f, 63.0f, 125.0f, 250.0f, 500.0f,
                                              1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f};

    for (int i = 0; i < 10; ++i)
    {
        // Initialize each EQBand with its center frequency
        eqBands[i].prepare(centerFrequencies[i], 44100, 1.0f); // sampleRate and numChannels will be updated in prepareToPlay

        // Attach each EQBand's slider to its corresponding parameter
        juce::String paramID = "GainParam_" + juce::String(i);
        eqBands[i].initializeVTS(parameters, paramID);
    }

    // Initialize the parameter state
//    parameters.state = juce::ValueTree("Parameters");
}
PS03_graphicEQAudioProcessor::~PS03_graphicEQAudioProcessor()
{
}

//==============================================================================
const juce::String PS03_graphicEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PS03_graphicEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PS03_graphicEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PS03_graphicEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PS03_graphicEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PS03_graphicEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PS03_graphicEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PS03_graphicEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PS03_graphicEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void PS03_graphicEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PS03_graphicEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Update sampleRate and prepare each EQBand with the correct number of channels

    // Example center frequencies for the 10 bands
    std::array<float, 10> centerFrequencies = {32.0f, 63.0f, 125.0f, 250.0f, 500.0f,
                                              1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f};

    for (int i = 0; i < 10; ++i)
    {
        // Retrieve current gain from parameters
        float currentGain = *parameters.getRawParameterValue("GainParam_" + juce::String(i));

        // Re-prepare EQBand with updated sampleRate and numChannels
        eqBands[i].prepare(centerFrequencies[i], static_cast<int>(sampleRate), currentGain);
    }
}

void PS03_graphicEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PS03_graphicEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PS03_graphicEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    // Create an AudioBlock from the buffer
    juce::dsp::AudioBlock<float> audioBlock(buffer);

    // Process each EQBand
    for (int i = 0; i < 10; ++i)
    {
        eqBands[i].process(audioBlock);
    }
}

//==============================================================================
bool PS03_graphicEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PS03_graphicEQAudioProcessor::createEditor()
{
    return new PS03_graphicEQAudioProcessorEditor (*this);
}

//==============================================================================
void PS03_graphicEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Save the parameters state
    juce::MemoryOutputStream stream(destData, true);
    parameters.state.writeToStream(stream);
}

void PS03_graphicEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // Restore the parameters state
       juce::ValueTree tree = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));

       if (tree.isValid())
       {
           parameters.state = tree;

           // Update EQBands with restored parameters
           for (int i = 0; i < 10; ++i)
           {
               float currentGain = *parameters.getRawParameterValue("GainParam_" + juce::String(i));
               eqBands[i].setGain(currentGain);
           }
       }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PS03_graphicEQAudioProcessor();
}
