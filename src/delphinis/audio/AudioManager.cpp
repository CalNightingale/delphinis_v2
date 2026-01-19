#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "delphinis/audio/AudioManager.h"
#include <iostream>
#include <unordered_map>

namespace delphinis {

struct AudioManager::Impl {
    ma_engine engine;
    bool initialized = false;
    std::unordered_map<SoundId, ma_sound*> sounds;
    SoundId nextId = 1;
};

AudioManager::AudioManager()
    : m_impl(new Impl())
{
}

AudioManager::~AudioManager() {
    shutdown();
    delete m_impl;
}

bool AudioManager::initialize() {
    if (m_impl->initialized) {
        return true;
    }

    ma_result result = ma_engine_init(nullptr, &m_impl->engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine: " << result << std::endl;
        return false;
    }

    m_impl->initialized = true;
    std::cout << "Audio engine initialized successfully" << std::endl;
    return true;
}

void AudioManager::shutdown() {
    if (!m_impl->initialized) {
        return;
    }

    // Clean up all loaded sounds
    for (auto& pair : m_impl->sounds) {
        ma_sound_uninit(pair.second);
        delete pair.second;
    }
    m_impl->sounds.clear();

    ma_engine_uninit(&m_impl->engine);
    m_impl->initialized = false;
}

SoundId AudioManager::loadSound(const std::string& filepath) {
    if (!m_impl->initialized) {
        std::cerr << "Cannot load sound: Audio engine not initialized" << std::endl;
        return INVALID_SOUND_ID;
    }

    ma_sound* sound = new ma_sound();
    ma_result result = ma_sound_init_from_file(&m_impl->engine, filepath.c_str(), 0, nullptr, nullptr, sound);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load sound: " << filepath << " (error: " << result << ")" << std::endl;
        delete sound;
        return INVALID_SOUND_ID;
    }

    SoundId id = m_impl->nextId++;
    m_impl->sounds[id] = sound;

    std::cout << "Loaded sound: " << filepath << " (id: " << id << ")" << std::endl;
    return id;
}

void AudioManager::playSound(SoundId id) {
    if (!m_impl->initialized) {
        return;
    }

    auto it = m_impl->sounds.find(id);
    if (it == m_impl->sounds.end()) {
        std::cerr << "Cannot play sound: Invalid sound id " << id << std::endl;
        return;
    }

    // Seek to beginning and play
    ma_sound_seek_to_pcm_frame(it->second, 0);
    ma_sound_start(it->second);
}

} // namespace delphinis
