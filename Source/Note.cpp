/*
  ==============================================================================

    Note.cpp
    Created: 29 Jun 2022 11:06:16am
    Author:  natep

  ==============================================================================
*/

#include "Note.h"

/*
given the maximum index from the fft, finds the musical note
that corresponds to the fundamental frequency and returns it as a string
*/
std::string Note::getNote(int fftIndex)
{
    double discreteFreq = (fftIndex / m_fftSize) * m_sampleRate;
    double minDistance = 1000000.0;
    double realFreq = 0.0;
    for (int i = 0; i < noteVec.size(); i++)
    {
        if (std::abs(discreteFreq - noteVec[i]) < minDistance)
        {
            realFreq = noteVec[i];
            minDistance = std::abs(discreteFreq - noteVec[i]);
        }
    }
    
    if (noteMap[realFreq] == "C0")
    {
        return "NA";
    }
    else
    {
        return noteMap[realFreq];
    }
    
}

/*
initializes notes from C0, B0 - B8
lower = lowest note: -57 = C0
upper = highest note: 50 = B8
*/

void Note::initializeMap(int lower, int upper)
{
    std::vector < std::string> noteNames;
    noteNames.push_back("C");
    noteNames.push_back("C#");
    noteNames.push_back("D");
    noteNames.push_back("D#");
    noteNames.push_back("E");
    noteNames.push_back("F");
    noteNames.push_back("F#");
    noteNames.push_back("G");
    noteNames.push_back("G#");
    noteNames.push_back("A");
    noteNames.push_back("A#");
    noteNames.push_back("B");

    double frequency = 0;
    int noteIndex = 0;
    int noteChar = 0;
    std::string noteString = "";
    std::string tempString = "";
    double frequency_Exponent = 0.0;
    for (int i = lower; i <= upper; i++)
    {
        tempString = noteNames[noteIndex];
        noteString = tempString.append(std::to_string(noteChar));
        frequency_Exponent = (double)i / 12.0;
        frequency = 440.0 * pow(2, frequency_Exponent);
        insertNote(frequency, noteString);

        if (noteIndex == 11)
        {
            noteIndex = -1;
            noteChar++;
        }
        noteIndex++;
    }
}

//inserts a note into the map and into the note vectors
void Note::insertNote(double freq, std::string note)
{
    noteMap.insert(std::pair<double, std::string>(freq, note));
    noteVec.push_back(freq);
}