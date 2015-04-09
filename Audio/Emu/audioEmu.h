#ifndef AUDIOEMU_H_INCLUDED
#define AUDIOEMU_H_INCLUDED

#include <string>
#include <onidev/typedef.h>
#include <onidev/sound/generator.h>

class AudioEmu : public od::SoundGenerator
{
    static const int readBuffSize = 2048;
    od::byte readBuff[readBuffSize];
    long nbSamples;
    int rest;
    int _samplerate;
    int _track;
    
    class Music_Emu * emu;
    
public:
    AudioEmu(const std::string & fname, int track, int sampleRate = 44100);
    ~AudioEmu();
    
    int track() const;
    void changeTrack(int track);
    
    // Reimplement od::SoundGenerator
    int sampleRate() const { return _samplerate; }
    int channelCount() const { return 2; }
    int bitsPerSample() const { return 16; }
    
    void fillBuffer(Chunk & chunk);
    void restart();
    
};

#endif // AUDIOEMU_H_INCLUDED
