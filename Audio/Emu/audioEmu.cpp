#include "audioEmu.h"
#include <gme/Music_Emu.h>
#include <stdlib.h>
#include <stdio.h>

static void handle_error( const char* str )
{
	if(str)
	{
		printf( "Error: %s\n", str ); getchar();
		exit( EXIT_FAILURE );
	}
}

AudioEmu::AudioEmu(const std::string & fname, int track, int samplerate) :
    nbSamples(2 * samplerate * 16 / 8),
    rest(0),
    _samplerate(samplerate),
    _track(track)
{
    //printf("Start game audio emulation.\n");
    // Determine file type
	gme_type_t file_type;
	handle_error( gme_identify_file( fname.c_str(), &file_type ) );
	if ( !file_type )
		handle_error( "Unsupported music type" );
	
	// Create emulator and set sample rate
	emu = file_type->new_emu();
	if ( !emu )
		handle_error( "Out of memory" );
	handle_error( emu->set_sample_rate( _samplerate ) );
	
	// Load music file into emulator
	handle_error( emu->load_file( fname.c_str() ) );
	
	// Start track
	handle_error( emu->start_track( track ) );
}

AudioEmu::~AudioEmu()
{
    delete emu;
}

int AudioEmu::track() const
{
    return _track;
}

void AudioEmu::changeTrack(int track)
{
    _track = track;
}

void AudioEmu::fillBuffer(Chunk & chunk)
{
    chunk.data.resize(nbSamples * sizeof(short));
    
    // si readBuff n'a pas été vidé, on copie ce qu'il reste dans data
    long dataPos = 0;
    if(rest != 0)
    {
        int i=readBuffSize - rest;
        for(; i<readBuffSize; ++i)
        {
            chunk.data[dataPos] = readBuff[i];
            dataPos++;
        }
        rest = 0;
    }
    
    while(dataPos < nbSamples)
    {
        // on re remplis readBuff
        handle_error( emu->play( readBuffSize / sizeof(short), reinterpret_cast<short*>(readBuff) ) );
        
        // on regarde si on peut le vider entierrement dans data
        if(dataPos + readBuffSize < nbSamples)
        {
            for(int i=0; i<readBuffSize; ++i)
            {
                chunk.data[dataPos + i] = readBuff[i];
            }
            rest = 0;
            dataPos += readBuffSize;
        }
        // sinon, on termine de remplir data et on arrete
        else
        {
            int fill = nbSamples - dataPos;
            //printf("%d\n", fill);
            for(int i=0; i<fill; ++i)
            {
                chunk.data[dataPos + i] = readBuff[i];
            }
            rest = readBuffSize - fill;
            dataPos = nbSamples;
        }
    }
    
    chunk.nbSamples = nbSamples;
}

void AudioEmu::restart()
{
    emu->start_track( _track );
}



