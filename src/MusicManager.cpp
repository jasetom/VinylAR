//
//  MusicManager.cpp
//
//  Created by Tomas on 13/03/2017.
//
// Most of the sound analysis code is adapted from ofxMaxim addon for openFrameworks:
// https://github.com/micknoise/Maximilian/blob/master/openFrameworks/openFrameworksExamples/iOS/maximFeatureExtractor0.8.4/src/ofApp.mm
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
    //all this is done manually due to the bug in maximilians library, which does not currently let
    //to load up and store samples into a vector using a for loop.
    //album1
    samp1.load(ofToDataPath("sounds/album1/album1 beat1.wav"));
    songs.push_back(std::make_pair(1,"album1 beat1"));
    samp2.load(ofToDataPath("sounds/album1/album1 beat2.wav"));
    songs.push_back(std::make_pair(2,"album1 beat2"));
    samp3.load(ofToDataPath("sounds/album1/album1 beat3.wav"));
    songs.push_back(std::make_pair(3,"album1 beat3"));
    
    //album2
    samp4.load(ofToDataPath("sounds/album2/album2 beat1.wav"));
    songs.push_back(std::make_pair(4,"album2 beat1"));
    samp5.load(ofToDataPath("sounds/album2/album2 beat2.wav"));
    songs.push_back(std::make_pair(5,"album2 beat2"));
    samp6.load(ofToDataPath("sounds/album2/album2 beat3.wav"));
    songs.push_back(std::make_pair(6,"album2 beat3"));
    
    //album3
    samp7.load(ofToDataPath("sounds/album3/album3 beat1.wav"));
    songs.push_back(std::make_pair(7,"album3 beat1"));
    samp8.load(ofToDataPath("sounds/album3/album3 beat2.wav"));
    songs.push_back(std::make_pair(8,"album3 beat2"));
    samp9.load(ofToDataPath("sounds/album3/album3 beat3.wav"));
    songs.push_back(std::make_pair(9,"album3 beat3"));
    
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
void MusicManager::update(int imgId){
    detectedImgId = imgId;
    
    //img sound choise
    if(detectedImgId!=0){
        
        for(int i =1; i <3; i++ ){
            
            int numb;
            
            if(i==1){
                numb = 0;
            }else if(i==2){
                numb = 3;
            }else if(i==3){
                numb = 6;
            }
            
            if(detectedImgId ==i){
                if(nextSound==true){
                    if(currentSong < songs[numb+2].first){
                        currentSong++;
                    }else{
                        swapSong =false;
                    }
                    
                    nextSound = false;
                    
                }else if(prevSound==true){
                    
                    if(currentSong > songs[numb].first){
                        currentSong--;
                    }else{
                        swapSong =false;
                    }
                    
                    prevSound =false;
                    
                }else if(first){
                    currentSong = songs[numb].first;
                    first = false;
                }
            }
        }
        
        
        if(currentSong==1 && swapSong){
            samp = samp1;
        }else if(currentSong==2 && swapSong){
            samp = samp2;
        }else if(currentSong==3 && swapSong){
            samp = samp3;
        }else if(currentSong==4 && swapSong){
            samp = samp4;
        }else if(currentSong==5 && swapSong){
            samp = samp5;
        }else if(currentSong==6 && swapSong){
            samp = samp6;
        }else if(currentSong==7 && swapSong){
            samp = samp7;
        }else if(currentSong==8 && swapSong){
            samp = samp8;
        }else if(currentSong==9 && swapSong){
            samp = samp9;
        }
        
        swapSong=false;
    }
}

void MusicManager::draw(int xPos, int yPos){
    
    ofBeginShape();
    ofSetColor(255,133,133);
    ofFill();
    
    for(int i=0; i < getFftSize() / 8; i++) {
        
        ofSetColor(7*i,133,133);
        //place spheres in a circle using trig functions
        int x = cos(i)*100;
        int y = sin(i)*100;
        ofDrawSphere(xPos+x,yPos+y,150,5+getFftMagnitudes(i)*2);
    }
    ofEndShape();

    
    
    
    //we can extract more features with the examples down below:
    
//    float horizWidth = 500.;
//    float horizOffset = 100;
//    float fftTop = 250;
//    float mfccTop = 350;
//    float chromagramTop = 450;
    
    //    ofSetColor(255, 0, 0,255);
    //
    //    //draw fft output
    //    float xinc = horizWidth / fftSize * 2.0;
    //    for(int i=0; i < fftSize / 2; i++) {
    //        //magnitudesDB took out
    //        float height = mfft.magnitudes[i] * 100;
    //        ofDrawRectangle(horizOffset + (i*xinc),250 - height,2, height);
    //    }
    //
    //    //	cout << "\nMFCCS: ";
    //    ofSetColor(0, 255, 0,200);
    //    xinc = horizWidth / 13;
    //    for(int i=0; i < 13; i++) {
    //        float height = mfccs[i] * 100.0;
    //        ofDrawRectangle(horizOffset + (i*xinc),mfccTop - height,40, height);
    //        //		cout << mfccs[i] << ",";
    //    }
    //
    //
    //    //octave analyser
    //    ofSetColor(255, 0, 255,200);
    //    xinc = horizWidth / oct.nAverages;
    //    for(int i=0; i < oct.nAverages; i++) {
    //        float height = oct.averages[i] / 20.0 * 100;
    //        ofDrawRectangle(horizOffset + (i*xinc),chromagramTop - height,2, height);
    //    }
    //
    
    //some features
//    ofSetColor(255, 255, 255,255);
//    
//    char peakString[255]; // an array of chars
//    sprintf(peakString, "Peak Frequency: %.2f", peakFreq);
//    ofDrawBitmapString(peakString, horizOffset, chromagramTop + 50);
//    
//    char a[255]; // an array of chars
//    sprintf(a, "Spectral Flatness: %f", specFlatness);
//    ofDrawBitmapString(a, horizOffset, chromagramTop + 80);
//    
//    char centroidString1[255]; // an array of chars
//    sprintf(centroidString1, "Spectral Centroid1: %f", specCentroid1);
//    ofDrawBitmapString(centroidString1, horizOffset, chromagramTop + 65);
//    
//    char centroidString2[255]; // an array of chars
//    sprintf(centroidString2, "Spectral Centroid2: %f", specCentroid2);
//    ofDrawBitmapString(centroidString2, horizOffset, chromagramTop + 80);
//    
//    char rmsString[255]; // an array of chars
//    sprintf(rmsString, "RMS: %.2f", rms);
//    ofDrawBitmapString(rmsString, horizOffset, chromagramTop + 95);
    
    
    
    //     ofSetColor(255, 0, 255, 200);
    //     ofDrawBitmapString("Pitch histogram", 50, 650);
    //     xinc = horizWidth / 12.0;
    //     int j = 0;
    //     float pitchHist[12];
    //     for (int i = 0; i < oct.nAverages; i++) {
    //         pitchHist[j] += oct.averages[i];
    //         j++;
    //         j = j % 12;
    //     }
    //     for(int i=0; i < 12; i++) {
    //         float height = pitchHist[i] /40. * 100;
    //         ofDrawRectangle(100 + (i*xinc),chromagramTop - height,50, height);
    //     }
    //
    //
    ////    Mel bands
    //    	cout << "\nMel bands: ";
    //     ofSetColor(255, 0, 255,100);
    //     xinc = 900.0 / 42.0;
    //     for(int i=0; i < 42; i++) {
    //         cout << mfcc.melBands[i] << ",";
    //         float height = mfcc.melBands[i] * 5.0;
    //         ofDrawRectangle(100 + (i*xinc),400 - height,10, height);
    //     }
    
}


//-----------------------------------------------------
void MusicManager::audioOut(float * output, int bufferSize, int nChannels){
  //  float sumRms;
    
    

    //check detected images id, if its 0, don't play anything.
    
//    for (int i = 0; i < bufferSize; i++){
//        output[i] = 0.0;
//    }
    
    
   // if(detectedImgId!=0){
        
        
        
        
        
        for (int i = 0; i < bufferSize; i++){
            //check wheter to play sound. Plays only when the AR marker is detected.
            if(playSound == true){
                
                //play sound
                playingSound = samp.play();
                
                //process sound and convert magnitudes to decibels in the mfft.
                if (mfft.process(playingSound)) {
                    //call this function to do more sound analysis
                    analyseSound();
                }
                //volume
                //set output to left and right channels, also we can set volume here too
                lAudio[i] = output[i * nChannels] = playingSound;// * 0.0;
                rAudio[i] = output[i * nChannels + 1] = playingSound;//* 0.0;
                
            }else{
                //if not playing set output to zero.
                lAudio[i] = output[i * nChannels] = playingSound * 0.0;
                rAudio[i] = output[i * nChannels + 1] = playingSound * 0.0;
            }
        }

  //  }
    
        
        
        
        
        
        
        
        
//        //check wheter to play sound. Plays only when the AR marker is detected as this variable is triggered then.
//        if(playSound == true){
//
//            //audio output forloop.
//            for (int i = 0; i < bufferSize; i++){
//                
//                //start playing  loaded sample
//                playingSound = samp.play();
//            
//                //process sound using fft
//                if (mfft.process(playingSound)) {
//                    //call this function to do more sound analysis
//                    analyseSound();
//                }
//                
//                //set output to left and right channels, also we can set volume here too
//                output[i * nChannels] = playingSound;// * 0.0;
//                output[i * nChannels + 1] = playingSound;//* 0.0;
//                
//                //we calculate rms here by adding up the outputs
//                sumRms = output[i * nChannels] * output[i * nChannels + 1];
//                //and then square rooting the sum
//                rms = sqrt(sumRms);
//            }
//        }
//    }
}

//-----------------------------------------------------
void MusicManager::analyseSound(){
    
    //convert magnitudes to decibels in the mfft.
    mfft.magsToDB();
    //calculate octaves
    oct.calculate(mfft.magnitudesDB);
    
    //some variables to do the analysis
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
    //calculate specControid1
    specCentroid1 = sum / (fftSize / 2);
    //calculate peakFreq
    peakFreq = (float)maxBin/fftSize * 44100;
    
    mfcc.mfcc(mfft.magnitudes, mfccs);
    //calculate specCentroid2
    specCentroid2 = mfft.spectralCentroid();
    //calculate specFlatness
    specFlatness = mfft.spectralFlatness();
    //calculate pitchHistogram
    calculatePitchHistogram();
    
    //detect beats
    detectBeat();
}

//-----------------------------------------------------
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
//    float prevFFT[windowSize/2+1];
    
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

//-----------------------------------------------------
bool MusicManager::getIsBeatDetected(){
    return beatDetected;
}

//-----------------------------------------------------
float MusicManager::getMelBands(int i){
    //usage:  for(int i=0; i < 42; i++) {
    //         float a = mfcc.melBands[i] * 5.0;
    //        }
    return mfcc.melBands[i];
}

//-----------------------------------------------------
float MusicManager::getSpecFlatness(){
    //    "Spectral Centroid: %f" = +mfft.maginutes[i] / (fftSize / 2)
    return specFlatness;
}

//-----------------------------------------------------
float MusicManager::getSpecCentroid1(){
    //    "Spectral Centroid: %f" = +mfft.maginutes[i] / (fftSize / 2)
    return specCentroid1;
}

//-----------------------------------------------------
float MusicManager::getSpecCentroid2(){
    //    "Spectral Centroid: %f" = mfft.spectralCentroid()
    return specCentroid2;
}

//-----------------------------------------------------
float MusicManager::getPeakFreq(){
    //    "Peak Frequency: %.2f"
    return peakFreq;
}

//-----------------------------------------------------
float MusicManager::getRms(){
    //    "rms: %.2f"
    return rms;
}

//-----------------------------------------------------
float MusicManager::getFftMagnitudes(int i){
    //  usage: float a = mfft.magnitutes[i]*2;
    return mfft.magnitudes[i];
}

//-----------------------------------------------------
float MusicManager::getMfccs(int i){
    //   usage: float a = mfccs[i] * 100.0;
    return mfccs[i];
}
//-----------------------------------------------------
float MusicManager::getOctaveAvg(int i){
    //   usage: for(int i=0; i < getOctaveAvgN; i++) {
    //              float a = oct.averages[i] / 20.0 * 100;
    //           }
    return oct.averages[i];
}

//-----------------------------------------------------
float MusicManager::getPitchHistogram(int i){
    //  usage: for(int i=0; i < 12; i++) {
    //          float a = pitchHist[i] /40. * 100;
    //          }
    return pitchHistogram[i];
    
}

//-----------------------------------------------------
int MusicManager::getOctaveAvgN(){
    return oct.nAverages;
}

//-----------------------------------------------------
int MusicManager::getFftSize(){
    return fftSize;
}

//-----------------------------------------------------
void MusicManager::play(bool play){
    playSound = play;
}

//-----------------------------------------------------
bool MusicManager::isPlay(){
    return playSound;
}

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

//-----------------------------------------------------
void MusicManager::firstDetect(){
    first =true;
}

//-----------------------------------------------------
string MusicManager::getCurrentSongName(){
    return songs[currentSong-1].second;
}

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
//-----------------------------------------------------














