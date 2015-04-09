#include "generator.h"
#include <cmath>

// melody from http://wavepot.com/

static const float m_pi = 3.14159265;

Generator::Generator(int sampleRate):
    m_sampleRate(sampleRate)
{
    // Melodie
    float n1[] = {3, 7, 10, 2, 3, 7, 3, 2, 0, 3, 7, 10, 0, 7, 3, 7, 8, 0, 3, 7, 8, 7, 8, 7, 5, 10, 2, 5, 10, 2, 5, 2};
    float o1[] = {2, 2,  2, 3, 3, 3, 3, 3, 2, 2, 2,  2, 3, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1,  2, 2, 2,  3, 3, 3, 3};
    float n2[] = {3, 3, 3, 3, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0,-2, 8, 8, 8, 8, 8, 8, 8, 7, 5, 5, 5, 5, 8, 5, 8, 2};
    float o2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    
    for(int i=0; i<32; ++i)
    {
        melodies[i/8][i%8] = note(n1[i], o1[i]);
        bassline[i/8][i%8] = note(n2[i], o2[i]);
    }
}

float Generator::note(float n, float octave)
{
    n += transpose;
    return powf(2, (n - 33 + (12 * octave)) / 12) * 440;
}

float Generator::sin(float t, float f, float a)
{
    return a * sinf( 2 * m_pi * t * f);
}

float Generator::sqr(float t, float f, float a)
{
    return ( (sin(t,f,a) > 0.f) * 2 - 1 ) * a;
}

float Generator::dsp(float t)
{
    int counter = t/spb; //How many beats have passed
    return sqr(t,melodies[(counter/16)%4][counter%8],0.11)
    +      sqr(t,bassline[(counter/16)%4][counter%8],0.11); //Changes note every 8beat, alter chord every 16 beats
}

void Generator::restart()
{
    lastOffset = 0;
}

void Generator::fillBuffer(Chunk & chunk)
{
    chunk.nbSamples = m_sampleRate * 2;
    chunk.data.resize(chunk.nbSamples * sizeof(short));
    
    // ajouter condition en fonction du codate 8 bits ou 16 bits
    short * samples = reinterpret_cast<short*>( &chunk.data[0] );
    for(int i=0; i<chunk.nbSamples; i++)
    {
        float t = (i + lastOffset) / static_cast<float>(sampleRate());
        *samples++ = 32760 * dsp(t);
    }
    lastOffset += chunk.nbSamples / 2;
}
