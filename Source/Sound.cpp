


#include "Sound.h"
#include <ao/ao.h>
#include <sndfile.h>
#include <memory.h>

//Размер буфера для звукового файла, 10 Мб =)
#define BUF_SIZE 10*1024*1024

//Внешняя функция для pthread, чота сложно давать ссылку на функцию внутри класса. Так что пока так.
static void *play_sync_audio(void *buffer) {
    ao_device *device;
    ao_sample_format format;
    int default_driver;
    //Драйвер по умолчанию
    default_driver = ao_default_driver_id();
    //Настройка формата WAV файла
    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 1;
    format.rate = 8000;
    format.byte_format = AO_FMT_LITTLE;
    //Открыть устройство
    device = ao_open_live(default_driver, &format, NULL);
    if (device == NULL) {
        fprintf(stderr, "Error opening device.\n");
        return NULL;
    }
    //Играть звук
    AUDIO_FILE *audio = (AUDIO_FILE *) buffer;
    ao_play(device, audio->bytes, audio->nbytes);
    //Закрыть устройство
    ao_close(device);
    return NULL;
}

AUDIO_FILE *Sound::open_audio(char *filename) {
    AUDIO_FILE *audio = (AUDIO_FILE*) malloc(sizeof(AUDIO_FILE));

    //Открыть WAV файл
    SF_INFO sfinfo;
    SNDFILE* file = sf_open(filename, SFM_READ, &sfinfo);

    //Считать данные звука в AUDIO_FILE
    audio->bytes = (char*)malloc(sizeof(char)*BUF_SIZE);
    audio->nbytes = sf_read_raw(file, audio->bytes, BUF_SIZE);

    //Закрыть WAV файл
    sf_close(file);
    return audio;
}

void Sound::close_audio(AUDIO_FILE* audio) {
    //Освободить занятые ресурсы звуком
    free(audio->bytes);
    free(audio);
}

void Sound::play_audio(AUDIO_FILE *audio) {
    //Каждый звук играет в своем отдельном треде
    pthread_t thread;
    if(pthread_create(&thread, NULL, play_sync_audio, audio)) {
        fprintf(stderr, "Error creating thread\n");
        return;
    }

    //Здесь мы запоминаем тред, если звук фоновая музыка
    if (audio == menuMusic){
        musicThread = thread;
    }
}

Sound::Sound() {
    //Инициализация устройства вывода звука
    ao_initialize();
    //Загрузка звуковых файлов
    menuMusic = open_audio("resources//MenuMusic.wav");
    impactBoard = open_audio("resources//ImpactPlatform.wav");
    impactBrick = open_audio("resources//ImpactBrick.wav");
}
Sound::~Sound() {
    //Освобождение ресурсов занятых звуковыми файлами
    close_audio(menuMusic);
    close_audio(impactBoard);
    close_audio(impactBrick);
    //Закрытие звукового устройства
    ao_shutdown();
}

//Музыка в меню
void Sound::playMenuMusic() {
    play_audio(menuMusic);
}

//Остановить музыку
void Sound::stopMenuMusic() {
    pthread_cancel(musicThread);
}

//Звук при контакте с кирпичем
void Sound::playImpactBrick() {
    play_audio(impactBrick);
}

//Звук при контакте с доской
void Sound::playImpactBoard() {
    play_audio(impactBoard);
}
