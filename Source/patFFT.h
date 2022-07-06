#pragma once

#include "Note.h"




class patFFT : private juce::Timer
{
public:

    patFFT(float sampleRate)
            : forwardFFT(fftOrder)
    {
        m_sampleRate = sampleRate;
        startTimerHz(60);
        note = new Note(m_sampleRate, fftOrder);
        
    }

    ~patFFT()
    {
        stopTimer();
    }

    void pushNextSampleIntoFifo(float sample) noexcept
    {
        // if the fifo contains enough data, set a flag to say
        // that the next line should now be rendered..

        if (fifoIndex == fftSize)
        {
            if (!nextFFTBlockReady)
            {
                std::fill(fftData.begin(), fftData.end(), 0.0f);
                std::copy(fifo.begin(), fifo.end(), fftData.begin());
                nextFFTBlockReady = true;
                
                
            }
            fifoIndex = 0;
        }
        fifo[(size_t)fifoIndex++] = sample;
    }

    std::string getNote()
    {
        return processorNote;
    }

    bool isBufferFull()
    {
        return nextFFTBlockReady;
    }

private:

    void getFundamentalFrequency()
    {
        //--------------------------------------------------------------
        forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());
        float max = 0.0f;
        int index = 0;
        //DBG(fftData.size());
        for (int i = 0; i <= fftData.size() / 2; i++)
        {
            if (fftData[i] > max)
            {
                max = fftData[i];
                index = i;
            }
        }
        //---------------------------------------------------------------
        processorNote = note->getNote(index);
    }

    void timerCallback() override
    {
        if (nextFFTBlockReady)
        {
            getFundamentalFrequency();
            nextFFTBlockReady = false;
        }
    }

    //Member Variables
    static constexpr auto fftOrder = 13;
    static constexpr auto fftSize = 1 << fftOrder;
    juce::dsp::FFT forwardFFT;
    std::array<float, fftSize> fifo;
    std::array<float, fftSize * 2> fftData;
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float m_sampleRate;
    Note* note;
    std::string processorNote = "";
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(patFFT)



};
