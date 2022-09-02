/*
  Example testing sketch for various DHT humidity/temperature sensors
  DHT CODE Written by ladyada, public domain

  밀리함수 컨트롤 코드는 https://blog.naver.com/damtaja/220992246602
  by 해바우

  화면 반전은 u8g.setRot180() 함수를 사용하여 할 수 있습니다.
  가변저항 값에 따라서 ON 시간 , OFF 시간을 지정하여
  모터 가동 시간을 정할수 있습니다.

  The circuit :
    각 input에 연결되어있는 구성 요소의 목록

    각 output에 연결되어있는 구성 요소의 목록
    수중펌프 ap201
    모터 드라이버 - L9110 모듈 2채널  ,  채널당 최대 700mA

  pwm 제어
  Created 연월일
  By iot발걸음
  Modified 날짜
  By iot발걸음

  V01 스케치에서
  V02 스케치 테스트중 , 차이는 가변 저항을 2개이용한다.
  22-08-31 아두이노스토리 카페 회원분들 조언에 따라  시 . 분 계산 코드를 변경한다.
           폐기된 코드는 맨 아래에 모아둠
           https://wokwi.com/projects/341468870682346066
  V03 여러 카페에 올려서 조언 받기 심화 버전
    아두이노 연구회 카페에선  출력용 아니면  second 계산이 어떤하지 와 
    스타트 , 정지 버튼도 고려하면 좋지 않나 라는 댓글이 달렸습니다. 

  Lisence : 
  The MIT License

Copyright (c) <year> <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <U8glib.h>
#include <DHT.h>
#include <Servo.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);

#define DHTTYPE DHT22
const int DHTPIN = 12; // what pin we're connected to
DHT dht(DHTPIN, DHTTYPE);
float Humidity;
float Temperature;

Servo servo, servo2;
int servoAngle, servo2Angle;

int potentioPin = A0, potentioPin2 = A1;
int potentioValue, potentioValue2;
int potentioMappingValue, potentioMappingValue2;

const int pump_A1 = 5;
const int pump_A2 = 6;
const int pump_B1 = 10;
const int pump_B2 = 11;

unsigned long previousMillis, previousMillis2, previousMillis3;
unsigned long currentMillis, currentMillis2;

long intervalOn, intervalOff;
int matchingTime = 3 * 60 / 5; // 3은 최대시간 , 60은 분으로계산 , 5는 5분씩
long selectTime, selectTime2;  // -> int로 하면 왜 intervalOn 계산값이 엉망이 되는가?
float selectTimeMinute, selectTimeMinute2;
int selectTimeHour, selectTimeHour2;
int motorState = LOW;
int motorSpeed1, motorSpeed2;

void setup()
{
    Serial.begin(115200);
    dht.begin();
    //servoInSetup(); // 서보 모터 일경우 주석 제거 및 일반 모터 주석 처리 
    motorInSetup();  // 일반 모터일경우 주석 제거 및 서보모터 주석 처리     
}

void loop()
{
    currentMillis = millis();
    currentMillis2 = millis();
    potentiometerMapping();  //가변저항1 값 -> 36 ( 180분 / 5분) 에 맞춤
    convertHourMinute(); // 가변저항1값 -> 시 , 분 으로 변환
    potentiometerMapping2(); //가변저항2 값 -> 36 ( 180분 / 5분) 에 맞춤
    convertHourMinute2(); // 가변저항2값 -> 시 , 분 으로 변환
    intervalOnOff(); // ON 할 시간 , OFF 할 시간 계산
    temp_humidity(); //온도 , 습도 측정
    water_motor_control();  // 일반 모터용
    //water_motor_control2(); // 서보모터용 컨트롤  , 일반 모터사용시 주석처리 
    oled_Display(); // OLED 출력
    serialPrint(); //  디버깅용 시리얼 출력
}

void potentiometerMapping() // read a0 and map() to mappingTime
{
    potentioValue = analogRead(potentioPin);
    potentioMappingValue = map(potentioValue, 0, 1023, 1, matchingTime);
    selectTime = potentioMappingValue * 5;
}

void convertHourMinute()
{
    if( selectTime >= 180)
    {
      selectTimeHour = selectTime / 60;
      selectTimeMinute = (selectTime - 180);
    }
    else
    {
    selectTimeHour = selectTime / 60;    // 나누기를 하게 되면 정수부분이 시간임 그래서 int(정수형)씀
    selectTimeMinute = selectTime % 60;  // 나눗셈으로 분을 구할수있음   
    }
}

void potentiometerMapping2()
{
    potentioValue2 = analogRead(potentioPin2);
    potentioMappingValue2 = map(potentioValue2, 0, 1023, 1, matchingTime);
    selectTime2 = potentioMappingValue2 * 5;
}

void convertHourMinute2()
{
    selectTimeHour2 = selectTime2 / 60;
    selectTimeMinute2 = selectTime2 % 60;   
}

void intervalOnOff()
{
    //intervalOn = selectTime * 60 * 10;   // 테스트용
    //intervalOff = selectTime2 * 60 * 10; // 테스트용
    intervalOn = selectTime * 60 * 1000;  // 1000밀리세컨드
    intervalOff = selectTime2 * 60 * 1000;
}

void temp_humidity()
{
    if (currentMillis2 - previousMillis2 >= 2500)
    {
        Humidity = dht.readHumidity();
        Temperature = dht.readTemperature();
        if (isnan(Temperature) || isnan(Humidity))
        {
            Serial.println("Failed to read from DHT sensor!");
        }
        else
        {
            Serial.print("Humidity: ");
            Serial.print(Humidity);
            Serial.print(" %\t");
            Serial.print("Temperature: ");
            Serial.print(Temperature);
            Serial.println(" *C ");
        }
        previousMillis2 = currentMillis2;
    }
}

void water_motor_control() // 일반모터용
{
    currentMillis = millis();
    if ((motorState == HIGH && currentMillis - previousMillis >= intervalOn) ||
        (motorState == LOW && currentMillis - previousMillis >= intervalOff))
    { // 수정한 부분
        previousMillis = currentMillis;
        if (motorState == LOW)
        {
            motorState = HIGH;
            motorSpeed1 = 255;
            motorSpeed2 = 0;
        }
        else
        {
            motorState = LOW;
            motorSpeed1 = 0;
            motorSpeed2 = 0;
        }

         analogWrite(pump_A1, motorSpeed1);
         analogWrite(pump_A2, motorSpeed2);
         analogWrite(pump_B1, motorSpeed1);
         analogWrite(pump_B2, motorSpeed2);        
    }
}

void water_motor_control2() // 서보모터용
{
    currentMillis = millis();
    if ((motorState == HIGH && currentMillis - previousMillis >= intervalOn) ||

        (motorState == LOW && currentMillis - previousMillis >= intervalOff))
    { // 수정한 부분
        previousMillis = currentMillis;
        if (motorState == LOW)
        {
            motorState = HIGH;
            servoAngle = 0;
            servo2Angle = 0;
        }
        else
        {
            motorState = LOW;
            servoAngle = 180;
            servo2Angle = 180;
        }

        servo.write(servoAngle);
        servo2.write(servo2Angle);
    }
}

void oled_Display()
{
    u8g.firstPage();
    do
    {
        u8g.setRot180(); //화면 반전
        u8g.setFont(u8g_font_unifont);
        // u8g.drawStr(25, 10, "Water Pump");
        u8g.setPrintPos(5, 10);
        u8g.print(selectTimeHour);
        u8g.drawStr(15, 10, "H");
        u8g.setPrintPos(26, 10);
        u8g.print(selectTimeMinute, 0);
        u8g.drawStr(42, 10, "M");
        u8g.setPrintPos(78, 10);
        u8g.print(selectTimeHour2);
        u8g.drawStr(88, 10, "H");
        u8g.setPrintPos(98, 10);
        u8g.print(selectTimeMinute2, 0);
        u8g.drawStr(114, 10, "M");
        u8g.drawStr(10, 30, "ON");
        u8g.drawStr(90, 30, "OFF");
        u8g.setFont(u8g_font_courB14);
        u8g.setPrintPos(5, 56);
        u8g.print(Humidity, 0);
        u8g.print("%");
        u8g.setPrintPos(70, 56);
        u8g.print(Temperature, 0);
        u8g.print("C");
        u8g.setFont(u8g_font_unifont);
        if (motorState == HIGH)
        {
            u8g.drawFrame(5, 13, 25, 25);
        }
        else
        {
            u8g.drawFrame(85, 13, 35, 25);
        }
    } while (u8g.nextPage());
}

void serialPrint() // 디버깅용
{
    if (currentMillis - previousMillis3 >= 2500)
    {
        previousMillis3 = currentMillis;
        Serial.print("motorState = ");Serial.print(motorState);Serial.print("    ");
        Serial.print("servoAngle = ");Serial.print(servoAngle);Serial.print("    ");
        Serial.print("servo2Angle = ");Serial.print(servo2Angle);Serial.println("    ");
        Serial.print("potentioValue = ");Serial.print(potentioValue);Serial.print("    ");
        Serial.print("potentioMappingValue = ");Serial.print(potentioMappingValue);Serial.println("    ");
        Serial.print("potentioValue2 = ");Serial.print(potentioValue2);Serial.print("    ");
        Serial.print("potentioMappingValue2 = ");Serial.print(potentioMappingValue2);Serial.println("    ");

        Serial.print("selectTime = ");Serial.print(selectTime);Serial.print("    ");
        Serial.print("selectTimeHour = ");Serial.print(selectTimeHour);Serial.print("    ");
        Serial.print("selectTimeMinute = ");Serial.print(selectTimeMinute);Serial.println("    ");
        Serial.print("selectTime2 = ");Serial.print(selectTime2);Serial.print("    ");
        Serial.print("selectTimeHour2 = ");Serial.print(selectTimeHour2);Serial.print("    ");
        Serial.print("selectTimeMinute2 = ");Serial.print(selectTimeMinute2);Serial.println("    ");
        Serial.print("intervalOn = ");Serial.print(intervalOn);Serial.print("    ");
        Serial.print("intervalOff = ");Serial.print(intervalOff);Serial.println("    ");
    }
}

void servoInSetup()
{
    
    servo.attach(10);  //서보모터용
    servo2.attach(11); //서보모터용
    servo.write(0);    //서보모터 초기 각도 0
    delay(1000);       //서보모터 움직일수 있게
    servo2.write(0);   //서보모터 초기 각도 0
    delay(1000);       //서보모터 움직일수 있게    
}

void motorInSetup()
{
    pinMode(pump_A1, OUTPUT); // 일반모터용
    pinMode(pump_A2, OUTPUT); // 일반모터용
    pinMode(pump_B1, OUTPUT); // 일반모터용
    pinMode(pump_B2, OUTPUT); // 일반모터용
}

/*      폐기된 코드 모음    


    //    시 , 분 을 구하는 간단한 코드를 몰랐을때 썼던 다중 if
    if (selectTime >= 0 && selectTime < 60)
    {
        selectTimeHour = selectTime / 60;
        selectTimeMinute = selectTime;
    }
    else if (selectTime >= 60 && selectTime < 120)
    {
        selectTimeHour = selectTime / 60;

        selectTimeMinute = (selectTime - 60);
    }
    else if (selectTime >= 120 && selectTime < 180)
    {
        selectTimeHour = selectTime / 60;
        selectTimeMinute = (selectTime - 120);
    }
    else if (selectTime = 180)
    {
        selectTimeHour = selectTime / 60;
        selectTimeMinute = (selectTime - 180);
    }
    else
    {
        selectTimeHour = 0;
    }
*/