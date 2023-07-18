#include "Mp3Notify.h"

void Mp3Notify::OnError(DFMiniMp3<HardwareSerial, Mp3Notify>&, uint16_t errorCode) {
  // see DfMp3_Error for code meaning
  //LOG(mp3_log, s_error, F("DfPlayer Error: "), errorCode);
   Serial.println(F("ERROR: "));
   Serial.println(errorCode);
}
void Mp3Notify::OnPlaySourceOnline  (DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source) { Serial.println(F("online")); }
void Mp3Notify::OnPlaySourceInserted(DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source) { Serial.println(F("bereit")); }
void Mp3Notify::OnPlaySourceRemoved (DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source) { Serial.println(F("entfernt")); }
void Mp3Notify::PrintlnSourceAction(DfMp3_PlaySources source, const __FlashStringHelper* action) {
  // if (source & DfMp3_PlaySources_Sd   ) LOG(mp3_log, s_debug, F("SD Karte "), action);
  // if (source & DfMp3_PlaySources_Usb  ) LOG(mp3_log, s_debug, F("USB "     ), action);
  // if (source & DfMp3_PlaySources_Flash) LOG(mp3_log, s_debug, F("Flash "   ), action);
}

void Mp3Notify::OnPlayFinished(DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source, uint16_t track) {
  //LOG(mp3_l//og, s_debug, F("Track beendet: "), track);
  // if (track == lastTrackFinished)
  //   return;
  // else
  //   lastTrackFinished = track;
  // delay(1);
  //Tonuino::getTonuino().nextTrack(1/*tracks*/, true/*fromOnPlayFinished*/);
  Serial.println(F("onPlayFinished"));
}