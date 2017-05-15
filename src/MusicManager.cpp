//
//  MusicManager.cpp
//  VinylARt
//
//  Created by Tomas on 13/03/2017.
//  Music Licenses can be found in github directory
//
//  Most of the sound analysis code is adapted from ofxMaxim addon for openFrameworks:
//  https://github.com/micknoise/Maximilian/blob/master/openFrameworks/openFrameworksExamples/iOS/maximFeatureExtractor0.8.4/src/ofApp.mm
//

#include "MusicManager.hpp"

//-----------------------------------------------------
void MusicManager::setup(){
    
    //Sound setup
    // sampling rate
    sampleRate = 44100;
    //buffer size. we will fill this buffer with sound
    initialBufferSize = 512;
    //left and right audio output sizes
    lAudio = new float[initialBufferSize];
    rAudio = new float[initialBufferSize];
    
    //set up maxim
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
    //load the music files and store their names and ids into 'songs' vector
    //all this is done manually due to me not being able to load up multiple sounds in a maxiSmaple Vector
    //Sound copyright licensing can be found in linceses.txt file
    //album1
    samp1.load(ofToDataPath("sounds/Captive Portal - Throwing EP/Captive_Portal_-_01_-_Jessies_Wisdom_Tooth.wav"));
    songs.push_back(std::make_pair(1,"Captive Portal - Jessies Wisdom Tooth"));
    samp2.load(ofToDataPath("sounds/Captive Portal - Throwing EP/Captive_Portal_-_02_-_Try_feat_Kiah.wav"));
    songs.push_back(std::make_pair(2,"Captive Portal - Try feat Kiah"));
    samp3.load(ofToDataPath("sounds/Captive Portal - Throwing EP/Captive_Portal_-_03_-_Not_Locrian.wav"));
    songs.push_back(std::make_pair(3,"Captive Portal - Not Locrian"));
    
    //album2
    samp4.load(ofToDataPath("sounds/Debmaster - Crevin/Debmaster_-_08_-_Propylne_v2.wav"));
    songs.push_back(std::make_pair(4,"Debmaster - Propylne v2"));
    samp5.load(ofToDataPath("sounds/Debmaster - Crevin/Debmaster_-_10_-_Swim_Like_a_.wav"));
    songs.push_back(std::make_pair(5,"Debmaster - Swim Like a"));
    samp6.load(ofToDataPath("sounds/Debmaster - Crevin/Debmaster_-_11_-_Hart_lAumnier.wav"));
    songs.push_back(std::make_pair(6,"Debmaster - Hart lAumnier"));
    
    //album3
    samp7.load(ofToDataPath("sounds/Spiedkiks - Take off your makeup/Spiedkiks_-_02_-_One_Too_Many_Drinks_Part2.wav"));
    songs.push_back(std::make_pair(7,"Spiedkiks - One Too Many Drinks"));
    samp8.load(ofToDataPath("sounds/Spiedkiks - Take off your makeup/Spiedkiks_-_04_-_Take_Off_Your_Make_Up.wav"));
    songs.push_back(std::make_pair(8,"Spiedkiks - Take Off Your Make Up"));
    samp9.load(ofToDataPath("sounds/Spiedkiks - Take off your makeup/Spiedkiks_-_07_-_Kitchen_Suite.wav"));
    songs.push_back(std::make_pair(9,"Spiedkiks - Kitchen Suite"));
    
    //setup fft
    fftSize = 1024;
    windowSize = 512;
    hopSize =256;
    mfft.setup(fftSize, windowSize, hopSize);
    
    //setup oct analyser
    nAverages = 12;
    oct.setup(sampleRate, fftSize/2, nAverages);
    
    //setup mfcc
    mfccs = (double*) malloc(sizeof(double) * 13);
    mfcc.setup(512, 42, 13, 20, 20000, sampleRate);
    
    //this starts DAC using opeframeworks
    ofSoundStreamSetup(2, 0, sampleRate, initialBufferSize, 4);
    playSound = false;
    
    //boolean variables to begin with
    first = true;
    swapSong = true;
    
    //threshold for beat detection
    threshold = 0.3;
    waitTime = 10;
    
}

//-----------------------------------------------------
//Update function gets called in the main update, its purpose is to check which image has been detected
//so that we would know which sound to play
void MusicManager::update(int imgId){
    
    if(imgId!=detectedImgId){
        swapSong=true;
    }
    
    detectedImgId = imgId;
    
    //img sound choise
    if(detectedImgId!=0){
        
        //this for loop is used to choose the correct song for the correct images
        for(int i =1; i <4; i++ ){
            //here I specify maximum song number for each of the albums
            //for demo purposes each of the albums will only have 3 songs
            int numb;
            if(i==1){
                numb = 0;
            }else if(i==2){
                numb = 3;
            }else if(i==3){
                numb = 6;
            }
            
            //we check which image was been detected
            if(detectedImgId ==i){
                //if nextSound bool is possible
                if(nextSound==true){
                    //we can go to the next song
                    if(currentSong < songs[numb+2].first){
                        currentSong++;
                    }else{
                        swapSong =false;
                    }
                    nextSound = false;
                    //then we also check if we can go back to the previous sound
                }else if(prevSound==true){
                    //if the current song is bigger than the number of songs for the detected image,
                    //we can go back to the previous song
                    if(currentSong > songs[numb].first){
                        currentSong--;
                    }else{
                        swapSong =false;
                    }
                    //once we swap to the previous we set this to false.
                    prevSound =false;
                    
                    //also if this was the very first time we detected the album, we go to the first song
                    //for that specific album
                }else if(first){
                    currentSong = songs[numb].first;
                    //and say that this is not first time anymore
                    first = false;
                }
            }
        }

  
        // very manual way to swap between the songs
        //if the current song number has changed and swapSong bool has been triggered
        //set the sample to be a different one
        if(currentSong==1 && swapSong){
            samp = &samp1;
        }else if(currentSong==2 && swapSong){
            samp = &samp2;
        }else if(currentSong==3 && swapSong){
            samp = &samp3;
        }else if(currentSong==4 && swapSong){
            samp = &samp4;
        }else if(currentSong==5 && swapSong){
            samp = &samp5;
        }else if(currentSong==6 && swapSong){
            samp = &samp6;
        }else if(currentSong==7 && swapSong){
            samp = &samp7;
        }else if(currentSong==8 && swapSong){
            samp = &samp8;
        }else if(currentSong==9 && swapSong){
            samp = &samp9;

        }
        
        swapSong=false;
    }
}

//-----------------------------------------------------
//This function plays the music out loud by filling up the openframeworks sound buffer.
//While doing so we also do some sound analysis
void MusicManager::audioOut(float * output, int bufferSize, int nChannels){
//if playSound has been triggered by image detection, do the following
    if(playSound){
        float sumRms;
        
        for (int i = 0; i < bufferSize; i++){
            //check wheter to play sound. Plays only when the AR marker is detected.
                //play sound
                playingSound = samp->play();
                
                //process sound and convert magnitudes to decibels in the mfft.
                if (mfft.process(playingSound)) {
                    //call this function to do more sound analysis
                    analyseSound();
                }
                //volume
                //set output to left and right channels, also we can set volume here too
                lAudio[i] = output[i * nChannels] = playingSound * 0.1;
                rAudio[i] = output[i * nChannels + 1] = playingSound * 0.1;
            
            //we calculate rms here by adding up the outputs
            sumRms = output[i * nChannels] * output[i * nChannels + 1];
            //and then square rooting the sum
            rms = sqrt(sumRms);

        }
        
        //if not playing set output to zero.
    }else if(!playSound){
        //fill the buffer with 0's
        for (int i = 0; i < bufferSize; i++){
            lAudio[i] = output[i * nChannels] = playingSound * 0.0;
            rAudio[i] = output[i * nChannels + 1] = playingSound * 0.0;
        }
    }
}

//-----------------------------------------------------
//This is where the basic sound analysis happens.
//Most of the calculations were adapted from Maximilian addon examples
void MusicManager::analyseSound(){
    
    //convert magnitudes to decibels in the mfft.
    mfft.magsToDB();
    //calculate octaves
    oct.calculate(mfft.magnitudesDB);
    
    //some variables to do the peakFreq analysis
    float sum = 0;
    float maxFreq = 0;
    int maxBin = 0;
    
    for (int i = 0; i < fftSize/2; i++) {
        sum += mfft.magnitudes[i];
        if (mfft.magnitudes[i] > maxFreq) {
            maxFreq=mfft.magnitudes[i];
            maxBin = i;
        }
    }

    //calculate peakFreq
    peakFreq = (float)maxBin/fftSize * 44100;
    
    mfcc.mfcc(mfft.magnitudes, mfccs);
    //calculate specCentroid2
    specCentroid = mfft.spectralCentroid();
    //calculate specFlatness
    specFlatness = mfft.spectralFlatness();
    //calculate pitchHistogram
    calculatePitchHistogram();
    //detect beats
    detectBeat();
}

//-----------------------------------------------------
//function to calculate pitchHistogram
//adapted from maximilian examples
void MusicManager::calculatePitchHistogram(){
    int j = 0;
    for (int a = 0; a < oct.nAverages; a++) {
        pitchHistogram[j] += oct.averages[a];
        j++;
        j = j % 12;
    }
}




//-----------------------------------------------------
bool MusicManager::detectBeat(){
    
    //simple beat detection using spectral fluxuation
    //adapted from Report of Mark Woulfe - http://doc.gold.ac.uk/~ma901mw/Year%203%20Project/Final%20Report/Report.pdf page 17

    
    //it is a very simple approach of detecting beats in an fft.
    //how it works is we take the difference between of last bin of the fft with
    //current bin of fft and add those differences to a variable called flux.
    //then we set the waitTime, which is just a simple counter and a threshold variable.
    //We detect the beat by assuming that the flux(uation) is greater than the threshold and
    //some time (waitTime) has passed since the last detection.
    float diff;
    specFlux = 0.0;

    
    //check mfft.bins or fftSize/2    
    for(int i=0; i < mfft.bins; i++){
        prevFFT.push_back(mfft.magnitudes[i]);
    }
    
    for(int i=0; i < fftSize/2; i++) {
        //this is how we calculate the difference between lastfft and current fft bin magnitudes
        diff = mfft.magnitudes[i] - prevFFT[i];;
        //if diference is not zero add it to the fluxtuation variable
        if (diff > 0) {
            specFlux += diff;
        }
    }
    //adjust fluxation depending on the number of the fft bins
    specFlux /= fftSize/2;
    //count down waitTime for the next beat detect
    if (waitTime !=0){
            waitTime--;
    }
    
    //this is where we check if the flux variable is higher than manually set threshold
    if (specFlux > threshold && waitTime ==0){
        //this means that the beat has been detected
        beatDetected = true;
        //we also set wait time back up for it to detect another beat
        waitTime = 10;
        return true;
    }else{
        beatDetected = false;
        return false;
    }
}

//Get functions used to animated visuals
//-----------------------------------------------------
bool MusicManager::getIsBeatDetected(){
    return beatDetected;
}

//-----------------------------------------------------
float MusicManager::getSpecFlatness(){
    return specFlatness;
}

//-----------------------------------------------------
float MusicManager::getSpecCentroid(){
    return specCentroid;
}

//-----------------------------------------------------
float MusicManager::getPeakFreq(){
    return peakFreq;
}

//-----------------------------------------------------
float MusicManager::getRms(){
    return rms;
}

//-----------------------------------------------------
//this function is used to start playing the music
void MusicManager::play(bool play){
    playSound = play;
}

//-----------------------------------------------------
//This function is used to get the status of audio
bool MusicManager::isPlay(){
    return playSound;
}

//Functions to interact with GUI
//-----------------------------------------------------
void MusicManager::nextSong(){
    nextSound = true;
    swapSong = true;
}

//-----------------------------------------------------
void MusicManager::prevSong(){
    prevSound =true;
    swapSong = true;
}

//we call this when we first detect album so that we could restart samples
//-----------------------------------------------------
void MusicManager::firstDetect(){
    first =true;
}

//returns current song name so that we could display it in the gui
//-----------------------------------------------------
string MusicManager::getCurrentSongName(){
    return songs[currentSong-1].second;
}
//returns artists and song name depending on the image detected
//-----------------------------------------------------
string MusicManager::getAlbumSongNames(){
    if(detectedImgId==1){
        return songs[0].second + " " + songs[1].second + " " + songs[2].second;
    }else if(detectedImgId==2){
        return songs[3].second + " " + songs[4].second + " " + songs[5].second;
    }else if(detectedImgId==3){
        return songs[6].second + " " + songs[7].second + " " + songs[8].second;
    }
}

//I use these functions to pass pointers of the objects to the visuals class so
//that it would be easier to interact with them there.
//-----------------------------------------------------
ofxMaxiFFT * MusicManager::getFFT(){
    return &mfft;
};

//-----------------------------------------------------
ofxMaxiMFCC * MusicManager::getMFCC(){
    return &mfcc;
};

//-----------------------------------------------------
ofxMaxiFFTOctaveAnalyzer * MusicManager::getFFTOctaveAnalyzer(){
    return &oct;
};

//-----------------------------------------------------
















