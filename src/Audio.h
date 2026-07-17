#ifndef AUDIO_H
#define AUDIO_H

#include<raylib.h>
#include<string>

class AudioEngine{
    public:
        std::string SoundsFolder;
        AudioEngine(std::string soundsFolder);
        Sound BallBasket;
        Sound BallPeg;
        Sound BallWall;
        Sound Lose;
        Sound Win;
        Sound Cannon;
        Sound Poof;
        Sound Miss;
        Sound Notes[5];

        void Play(Sound);
        void LoadSounds();
};

#endif