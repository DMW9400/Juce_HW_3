/*
  ==============================================================================

    EQBand.h
    Created: 26 Oct 2024 7:41:30pm
    Author:  David Matthew Welch

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class EQBand : public juce::Component, public juce::Slider::Listener {
public:
    EQBand();
    ~EQBand() override;
    void resized() override;
    
    void initializeVTS(juce::AudioProcessorValueTreeState& vts, const juce::String& paramID);
    void sliderValueChanged(juce::Slider* slider) override;
    void prepare(float frequency, int sampleRate, float gain);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();
    
    float getFrequency() const { return frequency; }
    float getGain() const { return gain; }
    void setGain(float newGain);
private:
    std::vector<juce::IIRFilter> filter;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    size_t numChannels;
    int sampleRate;
    float frequency, gain;
    juce::Slider bandSlider;
    juce::Label bandLabel;
};




