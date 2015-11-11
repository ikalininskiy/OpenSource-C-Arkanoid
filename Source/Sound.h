//
// 2015 (c) Copyright Victor Borisov
//
// Тут немножка помешана Си и Си++, т.к. работа с потоками и звуковыми устройства имеет API на Си.
// Поддерживаются звуковые файлы типа WAV 16-bit. Герцовка rate и channels могут быть разными

#include <pthread.h>

typedef struct AUDIO_FILE {
    char bits;
    char channels;
    int rate;
    int nbytes;
    char *bytes;
} AUDIO_FILE ;

class Sound {
private:
    AUDIO_FILE *menuMusic;
    AUDIO_FILE *impactBrick;
    AUDIO_FILE *impactBoard;
    pthread_t musicThread;

    AUDIO_FILE *open_audio(char *filename);
    void close_audio(AUDIO_FILE* audio);
    void play_async_audio(AUDIO_FILE *audio);
public:
    Sound();
    ~Sound();
    void playMenuMusic();
    void stopMenuMusic();
    void playImpactBrick();
    void playImpactBoard();
};
