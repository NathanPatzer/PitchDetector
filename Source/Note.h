/*
  ==============================================================================

    Note.h
    Created: 29 Jun 2022 11:06:03am
    Author:  natep

  ==============================================================================
  Handels all musical notes and frequencies for outputting to the screen
*/

#pragma once
#include <string>
#include <map>
#include <vector>
class Note
{
public:
    Note(int sampleRate, int fftOrder)
    {
        m_sampleRate = sampleRate;
        m_fftSize = pow(2, fftOrder);
        initializeMap(-57, 50);
    }

    std::string getNote(int fftIndex);

private:
    void initializeMap(int lower, int upper);
    void insertNote(double freq, std::string note);

    std::map<double, std::string> noteMap;
    std::vector<double> noteVec;
    int m_sampleRate;
    double m_fftSize;
};