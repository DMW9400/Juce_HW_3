/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PS03_graphicEQAudioProcessorEditor::PS03_graphicEQAudioProcessorEditor (PS03_graphicEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Add and make visible each EQBand component
    for (int bandNum = 0; bandNum < 10; ++bandNum)
    {
        addAndMakeVisible(audioProcessor.eqBands[bandNum]);
    }
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);
}

PS03_graphicEQAudioProcessorEditor::~PS03_graphicEQAudioProcessorEditor()
{
}

//==============================================================================
void PS03_graphicEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill the background with a dark grey color
       g.fillAll (juce::Colours::darkgrey);

       // Optionally, add a title label
       g.setColour (juce::Colours::white);
       g.setFont (15.0f);
       g.drawFittedText ("10-Band Graphic EQ", getLocalBounds().reduced(10), juce::Justification::centredTop, 1);
}

void PS03_graphicEQAudioProcessorEditor::resized()
{
    // Define layout parameters
    int spacing = 60;               // Space between sliders
    int sliderWidth = 40;           // Width of each slider
    int sliderHeight = getHeight() - 60; // Height of sliders (accounting for margins)

    int startX = 20;                // Starting x position
    int startY = 40;                // Starting y position (adjusted for title label)

    for (int i = 0; i < 10; ++i)
    {
        audioProcessor.eqBands[i].setBounds(
            startX + i * spacing,    // x position
            startY,                  // y position
            sliderWidth,             // width
            sliderHeight             // height
        );
    }
}
