#include "Audio.h"
#include<raylib.h>

AudioEngine::AudioEngine(std::string soundsFolder)
    : SoundsFolder(soundsFolder)
{  
}

void AudioEngine::Play(Sound s){
    PlaySound(s);
}

void AudioEngine::LoadSounds(){
    BallBasket = LoadSound((SoundsFolder + "/ballBasket.mp3").c_str());
    BallPeg = LoadSound((SoundsFolder + "/ballPeg2.mp3").c_str());
    BallWall = LoadSound((SoundsFolder + "/ballWall.mp3").c_str());
    Lose = LoadSound((SoundsFolder + "/lose.mp3").c_str());
    Win = LoadSound((SoundsFolder + "/win.mp3").c_str());
    Cannon = LoadSound((SoundsFolder + "/cannon.mp3").c_str());
    Poof = LoadSound((SoundsFolder + "/poof.mp3").c_str());
    Miss = LoadSound((SoundsFolder + "/miss.mp3").c_str());
}