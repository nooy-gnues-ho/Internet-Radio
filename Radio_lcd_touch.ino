#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
// tft_lcd
#define TFT_CS 15       // Chip Select (CS) = 15
#define TFT_DC 2        // Data/Command (DC) = 2
#define TFT_MOSI 23     // Master Out Slave In (MOSI) = 23
#define TFT_CLK 18      // Clock (CLK) = 18
#define TFT_RST 4       // Reset (RST) = 4
#define TFT_MISO 19     // Master In Slave Out (MISO) = 19
#define touch1 32       // channel up
#define touch2 33       // channel down
// Radio
#define I2S_DOUT 26 // Define I2S connections
#define I2S_BCLK 27 // Define Bit Clock (BLCK) = 27
#define I2S_LRC 14  // Define LR Clock (LRC) = 14
// Create an object of class Adafruit_ILI9341
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
int chnum =1;
int val1 =0;
int val2 =0;
const int volControl =39;
int volume =10;
Audio audio; // Create audio object
String ssid ="Bon-507(2)";
String password ="00000000";
void wifiset()
{
  WiFi.disconnect();            // Wifi disconnect & Reset
  WiFi.mode(WIFI_STA);          // set Wifi module to Station mode
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {     // Wait until the Wi-Fi connection is successful
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}
// Channel up
void ch_up()
{
  if (val1 ==1) {
    chnum++;
    Serial.println("up");
    if (chnum >6) {
      chnum =1;
    }
    switch (chnum) {
      case 1: ch1(); break;
      case 2: ch2(); break;
      case 3: ch3(); break;
      case 4: ch4(); break;
      case 5: ch5(); break;
      case 6: ch6(); break;    
    }
  }
}
// Channel down
void ch_down()
{
  if (val2 ==1) {
    chnum--;
    Serial.println("down");
    if (chnum <1) {
      chnum =6;
    }
    switch (chnum) {
      case 1: ch1(); break;
      case 2: ch2(); break;
      case 3: ch3(); break;
      case 4: ch4(); break;
      case 5: ch5(); break;
      case 6: ch6(); break;
    }
  }
}
// Channel set
void ch1() {
  screenreset();           // 화면을 재설정하는 함수를 호출
  tft.println("Channel 1"); // TFT 디스플레이에 "Channel 1"을 출력
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);  // 오디오 출력 핀을 설정. BCLK, LRC, DOUT을 사용
  audio.setVolume(volume);  // 오디오 출력의 볼륨을 설정. 'volume' 변수에 저장된 값으로 설정
  audio.connecttohost("https://stream.1a-webradio.de/deutsch/mp3-128/vtuner-1a");
}
// ch1과 동일
void ch2() {
  screenreset();
  tft.println("Channel 2");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume);
  audio.connecttohost("https://us.everestcast.com:1140/zhzokcgz/1/winamp.m3u");  
}
void ch3(){
  screenreset();
  tft.println("Channel 3");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume);
  audio.connecttohost("https://github.com/pgiacalo/audio_test/raw/main/LeftRightCenterTest.mp3"); 
}
void ch4(){
  screenreset(); 
  tft.println("Channel 4");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume); 
  audio.connecttohost("https://stream.1a-webradio.de/deutsch/mp3-128/vtuner-1a"); 
}
void ch5(){
  screenreset(); 
  tft.println("Channel 5");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume);
  audio.connecttohost("https://www.surfmusic.de/m3u/100-5-das-hitradio,4529.m3u"); 
}
void ch6(){
  screenreset(); 
  tft.println("Channel 6");
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume);
  audio.connecttohost("https://vis.media-ice.musicradio.com/CapitalMP3"); 
}
//tft_lcd screen setting
void screenreset(){
  tft.fillRect(0, 0, 480, 320, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(100, 125);
  tft.setTextColor(ILI9341_RED);
}
void setup() {
  Serial.begin(115200);
  wifiset();
  pinMode(touch1, INPUT);
  pinMode(touch2, INPUT);
  tft.begin();
  tft.setRotation(3);
  Serial.begin(9600);
  ch1();
}
void loop() {
  val1 = digitalRead(touch1);
  val2 = digitalRead(touch2);
  audio.loop(); // Run audio player
  volume = map((analogRead(volControl)), 0, 4095, 0, 20);  // volume 아날로그 입력에서 읽기
  audio.setVolume(volume);  // audio set audio volume max
  ch_up();
  ch_down();
}
// Audio status functions
void audio_info(const char *info) {
  Serial.print("info "); Serial.println(info);
}
void audio_id3data(const char *info) { // id3 metadata
  Serial.print("id3data "); Serial.println(info);
}
void audio_eof_mp3(const char *info) { // end of file
  Serial.print("eof_mp3 "); Serial.println(info);
}
void audio_showstation(const char *info) {
  Serial.print("station "); Serial.println(info);
}
void audio_showstreaminfo(const char *info) {
Serial.print("streaminfo "); Serial.println(info); 
}
void audio_showstreamtitle(const char *info) {
Serial.print("streamtitle "); Serial.println(info); 
}
void audio_bitrate(const char *info) {
Serial.print("bitrate "); Serial.println(info); 
}
void audio_commercial(const char *info) { // duration in sec
Serial.print("commercial "); Serial.println(info); 
}
void audio_icyurl(const char *info) { // homepage
Serial.print("icyurl "); Serial.println(info); 
}
void audio_lasthost(const char *info) { // stream URL played
Serial.print("lasthost "); Serial.println(info); 
}
void audio_eof_speech(const char *info) {
Serial.print("eof_speech "); Serial.println(info); 
}