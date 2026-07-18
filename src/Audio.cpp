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
    BallPeg = LoadSound((SoundsFolder + "/notes/D.mp3").c_str());
    BallWall = LoadSound((SoundsFolder + "/ballWall.mp3").c_str());
    Lose = LoadSound((SoundsFolder + "/lose.mp3").c_str());
    Win = LoadSound((SoundsFolder + "/win.mp3").c_str());
    Cannon = LoadSound((SoundsFolder + "/cannon.mp3").c_str());
    Poof = LoadSound((SoundsFolder + "/poof.mp3").c_str());
    Miss = LoadSound((SoundsFolder + "/miss.mp3").c_str());
    OneLeft = LoadSound((SoundsFolder + "/oneLeft.mp3").c_str());
    
    Background = LoadMusicStream((SoundsFolder + "/background.mp3").c_str());
    Background.looping = true;
    PlayMusicStream(Background);


    Notes[0] = LoadSound((SoundsFolder + "/notes/A.mp3").c_str());
    Notes[1] = LoadSound((SoundsFolder + "/notes/C.mp3").c_str());
    Notes[2] = LoadSound((SoundsFolder + "/notes/D.mp3").c_str());
    Notes[3] = LoadSound((SoundsFolder + "/notes/E.mp3").c_str());
    Notes[4] = LoadSound((SoundsFolder + "/notes/G.mp3").c_str());

}