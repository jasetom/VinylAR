//
//  MusicManager.hpp
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
    void setup();
    void update(int imgId);
    void audioOut(float * output, int bufferSize, int nChannels,int imgId); //audio output function
    void play(bool play);
    void analyseSound();
    void calculatePitchHistogram();
    void nextSong();
    void prevSong();
    void firstDetect();
    void draw(int xPos, int yPos);
    
    //Sound feature extraction:
    //get analysed sound features (single constantly updating values)
    float getSpecCentroid1();
    float getSpecCentroid2();
    float getSpecFlatness();
    float getPeakFreq();
    float getRms();
    //multiple values
    float getFftMagnitudes(int i);
    float getMfccs(int i);
    float getOctaveAvg(int i);
    float getPitchHistogram(int i);
    float getMelBands(int i);
    
    
    string getAlbumSongNames();
    string getCurrentSongName();
    
    int getOctaveAvgN();
    int getFftSize();
    bool isPlay();
    
private:
    //Loading and playing music
    maxiSample samp, samp1,samp2,samp3,samp4,samp5,samp6,samp7,samp8,samp9;
    int	initialBufferSize; //buffer size
    int sampleRate; //sample rate variable
    double playingSound; //variable where we store data when sound is playing
    bool playSound;
    float * lAudio; //left audio output
    float * rAudio; //right audio output
    
    /* FFT */
    ofxMaxiFFT mfft;
    int fftSize, windowSize, hopSize;
    int bins, dataSize;
    
    //other analysation tools
    double wave,sample, ifftVal;
    maxiMix mymix;
    maxiOsc osc;
    
    ofxMaxiFFTOctaveAnalyzer oct;
    int nAverages;
    float *ifftOutput;
    int ifftSize;
    
    float peakFreq = 0;
    float specCentroid1 = 0;
    float specCentroid2 = 0;
    float specFlatness = 0;
    float rms = 0;
    float pitchHistogram[12];
    
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
