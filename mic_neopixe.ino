#include <driver/i2s.h>
#define I2S_WS 25 // LRCLK
#define I2S_SD 32 // SDATA, SDIN, SDOUT
#define I2S_SCK 33 // BCLK
#define I2S_PORT I2S_NUM_0 // Use I2S Processor 0
#define bufferLen 64 // Define input buffer length
int16_t sBuffer[bufferLen];
void i2s_install() {
    // I2S 프로세서 구성 설정
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), 
        // I2S를 마스터 모드로 설정하여 데이터 수신
        .sample_rate =44100,                               
        // 샘플률을 44.1 kHz로 설정
        .bits_per_sample = i2s_bits_per_sample_t(16),       
        // 각 오디오 샘플에 16비트를 사용
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,        
        // 왼쪽 채널만 사용
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S), 
        // 표준 I2S 통신 형식을 사용
        .intr_alloc_flags =0,                              
        // 특별한 인터럽트 할당 플래그 없음
        .dma_buf_count =8,                                 
        // DMA 버퍼 풀 내의 버퍼 수
        .dma_buf_len = bufferLen,                           
        // 각 DMA 버퍼의 길이 (
        .use_apll =false                                   
        // 오디오 PLL을 사용하지 않음
    };
    // 지정된 설정으로 I2S 드라이버를 설치
    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}
void i2s_setpin() {
    // I2S 핀 설정 구성
    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,       
        // BCLK (Bit Clock) 핀을 I2S_SCK로 설정
        .ws_io_num = I2S_WS,         
        // LRCLK (Word/Frame Select) 핀을 I2S_WS로 설정
        .data_out_num =-1,          
        // 데이터 출력이 없으므로 -1로 설정
        .data_in_num = I2S_SD        
        // 데이터 입력 핀을 I2S_SD로 설정
    };
    // I2S 포트에 핀 구성을 설정
    i2s_set_pin(I2S_PORT, &pin_config); 
}
void setup() {
  Serial.begin(115200);
  Serial.println(" ");
  delay(1000);
  i2s_setpin();   
  // I2S 포트의 핀을 설정
  i2s_start(I2S_PORT);  
  // I2S 포트를 시작하여 데이터 수신을 준비
  delay(500);  
  // 500 밀리초 동안 대기
}
void loop() {
  int rangelimit =3000;
  
  // 출력: -3000 3000 
  Serial.print(rangelimit *-1);
  Serial.print(" ");
  Serial.print(rangelimit);
  Serial.print(" ");
  // I2S에서 데이터를 읽어옵니다.
  size_t bytesIn =0;
  esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);
  // I2S에서 데이터 읽기가 성공했을 경우
  if (result == ESP_OK) {
    // 읽어온 샘플의 개수 계산
    int16_t samples_read = bytesIn /8; 
    // I2S 데이터 버퍼에서 읽은 샘플 수
    // 적어도 하나 이상의 샘플이 읽혔을 때
    if (samples_read >0) {
      float mean =0;
      for (int16_t i =0; i < samples_read; ++i) {
        mean += (sBuffer[i]);
      }
      mean /= samples_read; // Calculate average of read data
      Serial.println(mean); // Output average value to serial monitor
    } 
  }
}