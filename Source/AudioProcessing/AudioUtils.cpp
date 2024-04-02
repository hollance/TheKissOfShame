#include "../shameConfig.h"

juce::AudioBuffer<float>* loadSampleFromFile(const juce::File& audioFile)
{
    juce::AudioBuffer<float>* buffer = nullptr;
    if (audioFile.existsAsFile()) {
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        juce::AudioFormatReader* reader = formatManager.createReaderFor(audioFile);
        if (reader) {
            int channels = int(reader->numChannels);
            int length = int(reader->lengthInSamples);
            buffer = new juce::AudioBuffer<float>(channels, length);
            reader->read(buffer, 0, length, 0, true, true);
            delete reader;
        }
    }
    return buffer;
}

juce::AudioBuffer<float>* loadSampleFromMemory(const char* data, int size)
{
    juce::AudioBuffer<float>* buffer = nullptr;

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto input = std::make_unique<juce::MemoryInputStream>(data, size, false);

    juce::AudioFormatReader* reader = formatManager.createReaderFor(std::move(input));
    if (reader) {
        int channels = int(reader->numChannels);
        int length = int(reader->lengthInSamples);
        buffer = new juce::AudioBuffer<float>(channels, length);
        reader->read(buffer, 0, length, 0, true, true);
        delete reader;
    }
    return buffer;
}
