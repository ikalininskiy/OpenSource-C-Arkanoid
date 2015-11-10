
#include <ao/ao.h>
#include <sndfile.h>

#define BUF_SIZE 10*1024*1024

typedef struct AUDIO_FILE {
    int nbytes;
    char *bytes;
} AUDIO_FILE ;

void *play_sync_audio(void *buffer)
{
	ao_device *device;
	ao_sample_format format;
	int default_driver;

	default_driver = ao_default_driver_id();
    memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = 1;
	format.rate = 8000;
	format.byte_format = AO_FMT_LITTLE;
	device = ao_open_live(default_driver, &format, NULL);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return NULL;
	}

    AUDIO_FILE *audio = (AUDIO_FILE *) buffer;
    
    ao_play(device, audio->bytes, audio->nbytes);
	ao_close(device);
    return NULL;
}

void play_audio(AUDIO_FILE *audio)
{
    pthread_t thread;
    if(pthread_create(&thread, NULL, play_sync_audio, audio)) {
        fprintf(stderr, "Error creating thread\n");
        return;
    }
}


AUDIO_FILE *open_audio(char *filename){
    AUDIO_FILE *audio = (AUDIO_FILE*) malloc(sizeof(AUDIO_FILE));
 
    SF_INFO sfinfo;
    SNDFILE* file = sf_open(filename, SFM_READ, &sfinfo);
    
    audio->bytes = (char*)malloc(sizeof(char)*BUF_SIZE);
    audio->nbytes = sf_read_raw(file, audio->bytes, BUF_SIZE);
    sf_close(file);
    return audio;
}

void close_audio(AUDIO_FILE* audio)
{
    free(audio->bytes);
    free(audio);
}
