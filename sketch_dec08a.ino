#include <Humidity.h> // 습도센서 라이브러리를 사용하기 위한 헤더파일 추가
#include <Temperature.h> //온도센서 라이브러리를 사용하기 위한 헤더파일 추가
#include <Wire.h> //I2C통신을 쉽게 하기 위한 Wire 객체 제공
#include <LiquidCrystal_I2C.h> // LCD 라이브러리 사용을 위한 헤더 파일 추가
#include <UltraSonic3.h> // 초음파 센서 라이브러리 사용을 위한 헤더 파일 추가
#include <Buzzer.h>    //- 부저 라이브러리 포함
#include <Light.h> // 조도센서 라이브러리 사용을 위한 헤더 파일 추가


LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);

#define TEMP_ADDR  72   // 온도센서의 주소 값 정의
#define BUZ_PIN  11    //- 부저 연결 핀 선언

int HUM_PIN = 54;   // 아두이노와 연결된 습도센서 핀 선언
const int LED_R = 26;  //- LED 연결 핀 번호 선언
const int LED_G = 27;  //- LED 연결 핀 번호 선언
const int LED_W = 28;  //- LED 연결 핀 번호 선언
int LCD_PIN = 39;   //LCD 핀 선언
int LIGHT_PIN = 57;    //아두이노와 연결된 조도센서 핀 선언
const int trigPin = 84; //초음파에서 아두이노
const int echoPin = 83; //아두이노에서 초음파

Buzzer buzzer;      //- 센서 객체 선언
Humidity humidity;     // 습도센서 제어 객체 변수 선언
Temperature temper;    // Temperature 객체 선언
UltraSonic3 ultra;   // 초음파 센서 객체 선언

float temp = 0.0;  //temp변수 선언 

void setup() {
  humidity.begin(HUM_PIN); // 습도센서 연결 핀 54번을 입력 모드 설정 초기화
  temper.begin(TEMP_ADDR);            // 온도센서 주소 전달 통신 시작
  buzzer.begin(BUZ_PIN);   //- 부저 객체 초기화
  ultra.begin(trigPin,echoPin); // begin()함수로 초음파센서 초기화 및 초음파 출력
  pinMode(LED_R, OUTPUT);           //- LED 연결 핀 동작 모드 설정
  pinMode(LED_G, OUTPUT);           //- LED 연결 핀 동작 모드 설정
  pinMode(LIGHT_PIN, INPUT); //조도(빛) 센서 연결 핀을 입력모드 설정 
  Serial.begin(9600);  //개발 PC 모니터에 측정된 습도 값 출력을 위한 Serial 포트 초기화

}

void loop() {
  Serial.print("Humidity = ");   //시리얼 모니터에 Humidity =  출력
  Serial.print( humidity.read() ); //측정된 습도 값을 읽어 모니터에 출력 (리턴값: 숫자)
  Serial.println(" %");  // 뒤에 추가로 “%“ 출력 
  Serial.print("Temperature = ");  //시리얼 모니터에 Temperature =  출력
  Serial.println(temper.getTemperatureC( ));    // 섭씨온도 출력

  lcd.clear();  //LCD 클리어
  lcd.begin(16,2);  // LCD의 열과 행 수를 설정합니다.
  lcd.print("Humidity: "); //LCD에 Humidity 출력
  lcd.print(humidity.read() );  //humidity 값 읽어오기
  lcd.setCursor(0,1);   //커서 위치 설정(두번째칸, 첫번째줄)
  lcd.print("Temprature: ");  //LCD에 Temprature 출력
  lcd.print((temper.getTemperatureC( )));  //temperature 값 읽어오기
  
  temp = temper.getTemperatureC(); //temp값을 temper값과 동일하게 얻어온다
  
  if(temp >= 27 ){ //온도가 27도 보다 높으면
    digitalWrite(LED_R, HIGH); //LED(Red) On
    digitalWrite(LED_G, LOW); //LED(Green) Off
  }else{ //아닐 시
    digitalWrite(LED_G, HIGH);//LED(Green) ON
    digitalWrite(LED_R, LOW); //LED(Red) Off
  }
  delay(1000); //1초 지연
  
  String tmp ="";  // tmp 문자 초기화(“ “ = 빈값) 
  tmp += analogRead(LIGHT_PIN); // 측정된 조도(빛) 값 읽어 오기
  tmp += "Lux";  // 뒤에 Lux 단어 추가
  //Serial.println(tmp); // 측정된 조도값(즉, tmp값) 모니터에 출력
  delay(1000); // 1초 동안 지연 후 다시 조도(빛) 값 측정 후 출력
  
  int light = 0; //light 변수 선언
  light = analogRead(LIGHT_PIN); //아날로그 LIGHT_PIN으로부터 입력값을 light로 저장
  if(light < 300){ //조도센서 값이 300 미만일 경우
    digitalWrite(LED_W, HIGH); //LED(White) ON
  }else{//아닐 시
    digitalWrite(LED_W, LOW); //LED(White) Off
  }
  
  int distance; //거리변수 선언

  if(ultra.ReadDistanceCentimeter() <= 10){//거리 값이 10이하일 경우
    buzzer.On();//부저 on
  }else{//아닐 시
    buzzer.Off();//부저 off
  }
  delay(1000);//1초 지연
} 

