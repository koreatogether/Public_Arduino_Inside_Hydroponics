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

    코드 등록 블로그 : https://togetherfactory.tistory.com/4
    

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