// Simple wrapper for Arduino sketch to compilable with cpp in cmake
#include "Arduino.h"
#include "AudioTools.h"
#include "BabyElephantWalk60_mp3.h"

using namespace audio_tools;  


MemoryStream mp3(BabyElephantWalk60_mp3, BabyElephantWalk60_mp3_len);
EncodedAudioStream in(mp3, new MP3DecoderMini()); // MP3 data source
PortAudioStream portaudio_stream;   // Output of sound on desktop 
StreamCopy copier(portaudio_stream, in);            // copy in to out


/// open portaudio with audio inform provided by mp3
void mp3InfoCallback(MP3MiniAudioInfo &info){
    PortAudioConfig pc;
    pc = info;
    pc.is_output = true;
    Serial.print("sample_rate: ");
    Serial.println(pc.sample_rate);
    Serial.print("channels: ");
    Serial.println(pc.channels);

    portaudio_stream.begin(pc);
}

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  
  // this is not really necessary
  in.begin();
}

void loop(){
  if (mp3) {
    copier.copy();
  } else {
    auto info = in.decoder().audioInfo();
    LOGI("The audio rate from the mp3 file is %d", info.sample_rate);
    LOGI("The channels from the mp3 file is %d", info.channels);
    exit(0);
  }
}