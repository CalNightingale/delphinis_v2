#pragma once

#include <string>
#include <cstdint>

namespace delphinis {

using SoundId = uint32_t;
constexpr SoundId INVALID_SOUND_ID = 0;

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Disable copying
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool initialize();
    void shutdown();

    SoundId loadSound(const std::string& filepath);
    void playSound(SoundId id);

    void setMuted(bool muted);
    bool isMuted() const;
    void toggleMute();

private:
    struct Impl;
    Impl* m_impl;
};

} // namespace delphinis
