#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <tinywav.h>

typedef unsigned int uint;

typedef struct {
    unsigned int buffer;
} Audio;

typedef struct {
    unsigned int source;
    bool camera;
    Object* object;
} AudioSource;

ALCdevice* audioDevice;
ALCcontext* audioContext;

Audio* audios[256];
AudioSource* audioSources[256];

Audio* loadAudio(char* filename) {
    unsigned int buffer;
    alGenBuffers(1, &buffer);

    TinyWav tw;

    tinywav_open_read(&tw, (const char*)filename, TW_INLINE);

    int numSamples = tw.numFramesInHeader * tw.h.NumChannels;
    float* data = malloc(sizeof(float) * numSamples);

    int offset = 0;

    while(offset != tw.numFramesInHeader) {
        if(tw.numFramesInHeader - offset < 100000) {
            tinywav_read_f(&tw, data + offset, tw.numFramesInHeader - offset);
            offset += tw.numFramesInHeader - offset;

        } else {
            tinywav_read_f(&tw, data + offset, 100000);
            offset += 100000;
        }
    }

    short* pcmData = malloc(sizeof(short) * numSamples);
    for (int i = 0; i < numSamples; i++) {
        float clamped = fmaxf(-1.0f, fminf(1.0f, data[i]));
        pcmData[i] = (short)(clamped * 32767.0f);
    }

    alBufferData(
        buffer,
        (tw.h.NumChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
        pcmData,
        numSamples * sizeof(short),
        tw.h.SampleRate / tw.h.NumChannels
    );

    free(data);
    free(pcmData);
    tinywav_close_read(&tw);

    Audio* audio = malloc(sizeof(Audio));
    audio->buffer = buffer;

    for(int i = 0; i < 256; i++) {
        if(audios[i] == 0) {
            audios[i] = audio;
            break;
        }
    }

    return audio;
}

void deleteAudio(Audio* audio) {
    for(int i = 0; i < 256; i++) {
        if(audios[i] == audio) {
            audios[i] = 0;
            break;
        }
    }
    alDeleteBuffers(1, &audio->buffer);
}

AudioSource* createAudioSource(Audio* audio, float pos_x, float pos_y, float pos_z, bool looping) {
    unsigned int source;
    alGenSources(1, &source);

    alSource3f(source, AL_POSITION, pos_x, pos_y, pos_z);
    alSourcei(source, AL_BUFFER, audio->buffer);
    alSourcei(source, AL_LOOPING, looping ? 1 : 0);
    alSourcei(source, AL_REFERENCE_DISTANCE, 8.0f);
    alSourcei(source, AL_MAX_DISTANCE, 32.0f);
    alSourcei(source, AL_MIN_GAIN, 0.0f);
    alSourcei(source, AL_MAX_GAIN, 1.0f);
    alSourcei(source, AL_GAIN, 1.0f);
    alSourcePlay(source);

    AudioSource* audioSource = malloc(sizeof(AudioSource));
    audioSource->source = source;
    audioSource->camera = false;
    audioSource->object = 0;

    for(int i = 0; i < 256; i++) {
        if(audioSources[i] == 0) {
            audioSources[i] = audioSource;
            break;
        }
    }

    return audioSource;
}

void setAudioSourcePos(AudioSource* source, float pos_x, float pos_y, float pos_z) {
    alSource3f(source->source, AL_POSITION, pos_x, pos_y, pos_z);
}

bool getAudioSourceState(AudioSource* source) {
    int state;
    alGetSourcei(source->source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void deleteAudioSource(AudioSource* source) {
    for(int i = 0; i < 256; i++) {
        if(audioSources[i] == source) {
            audioSources[i] = 0;
            break;
        }
    }
    alDeleteSources(1, &source->source);
}