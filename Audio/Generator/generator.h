#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include <onidev/sound/generator.h>

class Generator : public od::SoundGenerator
{
    int lastOffset = 0;
    
    float transpose = 2;
    float bpm = 240; // Beats per minute
    float spb = 60/bpm; // Second per beat
    int m_sampleRate;
    
    float melodies[4][8];
    float bassline[4][8];
    
    float note(float n, float octave);
    float sin(float t, float f, float a);
    float sqr(float t, float f, float a);
    
public:
    Generator(int sampleRate);
    
    float dsp(float t);
    
    int sampleRate() const { return m_sampleRate; }
    int channelCount()  const { return 1; }
    int bitsPerSample() const { return 16; }
    
    void fillBuffer(Chunk & chunk);
    
    // Lecture
    void restart();
};

#endif // GENERATOR_H_INCLUDED
