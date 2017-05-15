//
//  MusicManager.hpp
//  VinylARt
//
//  Created by Tomas on 13/03/2017.
//
//

#ifndef MusicManager_hpp
#define MusicManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxMaxim.h"

class MusicManager {
public:
    //function declarations
    void setup();
    void update(int imgId);
    void audioOut(float * output, int bufferSize, int nChannels); //audio output function
    void play(bool play);
    void analyseSound();
    void calculatePitchHistogram();
    bool detectBeat();
    void nextSong();
    void prevSong();
    void firstDetect();
    
    //Sound feature extraction:
    //get analysed sound features (single constantly updating values)
    float getSpecCentroid();
    float getSpecFlatness();
    float getPeakFreq();
    float getRms();
    bool getIsBeatDetected();
    
    //functions used to get pointers to fft, mfcc and octave analyser
    ofxMaxiFFT * getFFT();
    ofxMaxiMFCC * getMFCC();
    ofxMaxiFFTOctaveAnalyzer * getFFTOctaveAnalyzer();

    //funtions to get names of the music played
    string getAlbumSongNames();
    string getCurrentSongName();

    //more getters
    int getOctaveAvgN();
    int getFftSize();
    bool isPlay();
    
private:
    //Loading and playing music
    maxiSample *samp;
    maxiSample samp1,samp2,samp3,samp4,samp5,samp6,samp7,samp8,samp9;
    int	initialBufferSize; //buffer size
    int sampleRate; //sample rate variable
    double playingSound; //variable where we store data when sound is playing
    bool playSound;
    float * lAudio; //left audio output
    float * rAudio; //right audio output
    
    //fft variables
    ofxMaxiFFT mfft;
    int fftSize, windowSize, hopSize;
    int bins, dataSize;
    
    //beat detection(spectracl flux) variables
    vector <float> prevFFT;
    float specFlux;
    int waitTime;
    float threshold;
    bool beatDetected;
    
    //ocatve analyser variables
    ofxMaxiFFTOctaveAnalyzer oct;
    int nAverages;

    //variables for sound analysis
    float peakFreq = 0;
    float specCentroid = 0;
    float specFlatness = 0;
    float rms = 0;
    float pitchHistogram[12];
    //mfcc variables
    maxiMFCC mfcc;
    double *mfccs;
    
    //music control
    int currentSong;
    //we create a vector of ints(ids for songs) and strings(names of the songs);
    vector < pair<int, string> > songs;
    bool nextSound;
    bool prevSound;
    bool first;
    bool swapSong;
    
    //we use this variable to store detected image id comming from OrbTracker class.
    int detectedImgId;
    
};

#endif /* MusicManager_hpp */
