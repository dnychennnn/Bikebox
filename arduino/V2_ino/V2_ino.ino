#include <LedControl.h>
#include <SoftwareSerial.h>   // 引用程式庫

/*
This is for the moto navigation.
At first, we use the blabla pin as input(bluetooth),then have the pins: blablabla as output.
Smart phone send some text to arduino by bluetooth
e.g, :
6
Q

numbers mean the time counting down, 6 means we need 6 pins fot led
Q


*/

#include <Arduino.h>

//IO    
#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9
#define NBR_MTX 2 

LedControl lc=LedControl(12,11,10, NBR_MTX);
String digits= "0123456";
boolean dirCheckForA = false;
boolean dirCheckForB2I = false;
boolean dirCheckForJ2P = false;
boolean countingTimeEvent =true;
int countingTime =6;
char chr = 'Z';
int i = 0;
unsigned char Display_Buffer[2];

const unsigned char  Word1[1][32] = 
{
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xDF,0xCF,0xC7,0xC3,0x01,0x01,0xC3,0xC7,0xCF,0xDF,0xFF,0xFF,0xFF,
};  //　　向前 

const unsigned char Word2[1][32] = 
{
  0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,
0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,
}; //　向右

const unsigned char Word3[1][32] = 
{
  0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,
  0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,
 
}; //　向左

const unsigned char Word4[1][32] = 
{
 0xFF,0xFF,0xFF,0xFF,0xFB,0xF1,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7B,0x33,0x03,0x83,0x83,0x03,0x03,0xFF,0xFF,
 
}; //　向右前

const unsigned char Word5[1][32] = 
{
 0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF1,0xFB,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x03,0x03,0x83,0x83,0x03,0x33,0x7B,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};  //　向左前

const unsigned char Word6[1][32] = 
{
    0xFF,0xFF,0xFF,0xFF,0xFB,0xFF,0xFD,0xFF,0xFE,0xFF,0xFF,0xFD,0xF7,0xFE,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xDF,0x7F,0xEF,0xBF,0xFF,0xDF,0xFF,
}; //　　急右轉

const unsigned char Word7[1][32] = 
{
    0xFF,0xFF,0xFE,0xF7,0xFD,0xFF,0xFF,0xFE,0xFF,0xFD,0xFF,0xFB,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xDF,0xFF,0xBF,0xEF,0x7F,0xDF,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
}; //　　急左轉

const unsigned char Word8[1][32] = 
{
    0xFF,0xFF,0xFB,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFB,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xBF,0xEF,0xF7,0xFB,0xFB,0xF7,0xEF,0xBF,0xFF,0xFF,0xFF,0xFF,
}; //　　　U

const unsigned char Word9[1][32] = 
{
    0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,
  0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,
}; //　　　START

const unsigned char exclamation[1][32] = 
{
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x8C,0x8C,0x8C,0x8C,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
}; //　　　　驚嘆號





void setup(){
  Serial.begin(9600); // 開啟 Serial port, 通訊速率為 9600 bps
  //for (int pin1 = 2 ; pin1 <=13;pin1++){ // 定義digital 2~13 為輸出
  //pinMode(pin1, OUTPUT);
pinMode(LEDARRAY_D, OUTPUT); 
pinMode(LEDARRAY_C, OUTPUT);
pinMode(LEDARRAY_B, OUTPUT);
pinMode(LEDARRAY_A, OUTPUT);
pinMode(LEDARRAY_G, OUTPUT);
pinMode(LEDARRAY_DI, OUTPUT);
pinMode(LEDARRAY_CLK, OUTPUT);
pinMode(LEDARRAY_LAT, OUTPUT);

//Display(Word9);

 for (int i=0; i< NBR_MTX; i++){
    lc.shutdown(i,false);
  /* Set the brightness to a medium values */
    lc.setIntensity(i,8);
  /* and clear the display */
    lc.clearDisplay(i); }
  //digitalWrite(pin1,HIGH); //確認每個LED燈
 // delay(50);
  //digitalWrite(pin1,LOW);
//  delay(50);
 // }
  //Serial.println("OK"); // 回傳"OK"

}

void serialEvent() {
  while (Serial.available()){
    chr = Serial.read();
    //Serial.println(chr);
    char inputcheck = Serial.read();
    Serial.println(inputcheck);
    if (inputcheck == '7'){
      countingTime = 7;
    }
    if (inputcheck == '6'){
    countingTimeEvent =true;
    countingTime = 6;
    }
    else if (inputcheck == '5'){
      countingTimeEvent =true;
    countingTime = 5;
    }
    else if (inputcheck == '4'){
      countingTimeEvent =true;
    countingTime = 4;
    }
    else if (inputcheck == '3'){
      countingTimeEvent =true;
    countingTime = 3;
    }
    else if (inputcheck == '2'){
      countingTimeEvent =true;
    countingTime = 2;
    }
    else if (inputcheck == '1'){
      countingTimeEvent =true;
    countingTime = 1;
    }
    else if (inputcheck == '0'){
      countingTimeEvent =true;
    countingTime = 0;
    }
  //    Serial.println(inputcheck); // 回傳"inputcheck"
  }

  

}

void loop(){
    
 Display(Word9);
 if(chr != 'Z'){
 
   if(chr == '1') //　左轉
   { 
  digitalWrite(2, LOW); //給pin腳低電壓 (LED不通電就暗)
  delay(1000); //延遲1秒鐘(1000毫秒)
  digitalWrite(2, HIGH); //給pin腳高電壓 (LED通電就亮)
    //Display(Word3);	
   }
   else if(chr == 'K')  //　右轉
    {
//      digitalWrite(4, HIGH); //給pin腳高電壓 (LED通電就亮)
//    delay(1000); //延遲1秒鐘(1000毫秒)
//    digitalWrite(4, LOW); //給pin腳低電壓 (LED不通電就暗)
//    delay(1000); //延遲1秒鐘(1000毫秒)
     Display(Word2);	
    }
   else if(chr == 'L')  // 　微左
     Display(Word5);	
   else if(chr == 'M')  // 　微右
    Display(Word4);	
   else if(chr == 'N'){  //　左急轉
     Display(Word7);	
   }
   else if(chr == 'O'){   //　右急轉
   Display(Word6);
   }
    else if(chr == 'P' || chr == 'F'){  //U型迴轉
   Display(Word8);
   }
    else if(chr == 'S'){  //U型迴轉
   Display(Word9);
   }
    else if(chr == 'X'){  //　警示
     Display(exclamation);
   }
    
 }
 
 /*  //mark ven change 20140112
 if(chr == 'B'||chr == 'C'||chr == 'D'||chr == 'E'||chr == 'F'||chr == 'G'||chr == 'H'||chr == 'I' ){ // 當B~I時 全方向燈號亮
   for (int dir = 2; dir <= 5; dir++) { 
        digitalWrite(dir, HIGH);
       }
   }
*/
  //mark ven change 20140112
if (chr == 'C'|| chr == 'D' || chr == 'E')
  {
   Display(Word2);	
  }
  
if (chr == 'G'|| chr == 'H' || chr == 'I')
  {
    Display(Word3);
  }
  //mark ven change 20140112


  /* if(chr == 'Q'&&countingTime == 7)
   {
       Display(Word5);	
     //digitalWrite(10, HIGH);
   } */
     char ch;
    if(countingTimeEvent){
//       for (int ct=10;ct<=13;ct++){
  //          digitalWrite(ct, LOW);
    //      }
      // Serial.print(countingTime);
      // 
      if(countingTime==6){
        
      }
       ch= digits[countingTime];
         
    //   for(int i  = 0 ;i < 2500;i++){
      //  for (int ct=0;ct<=(4-countingTime);ct++)
        //    digitalWrite(ct+10, HIGH);
         //lc.displayChar(0, lc.getCharArrayPosition(ch));
         
      // }
     //if(countingTime == 0)
     //for(int i  = 0 ;i < 2500;i++){
       // for (int ct=10;ct<=13;ct++){
         //   digitalWrite(ct, LOW);
          //}
     //}      
      //countingTimeEvent = false;
       //            lc.clearAll();

    } 

}

  void Display(const unsigned char dat[][32])					
{
	unsigned char i;

	for( i = 0 ; i < 16 ; i++ )
	{
		digitalWrite(LEDARRAY_G, HIGH);		//更新数据时候关闭显示。等更新完数据，打开138显示行。防止重影。
		
		Display_Buffer[0] = dat[0][i];		
		Display_Buffer[1] = dat[0][i+16];

		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		digitalWrite(LEDARRAY_LAT, HIGH);					//锁存数据   
		delayMicroseconds(1);
	
		digitalWrite(LEDARRAY_LAT, LOW);
		delayMicroseconds(1);

		Scan_Line(i);							//选择第i行

		digitalWrite(LEDARRAY_G, LOW);
		
		delayMicroseconds(100);;			//延时一段时间，让LED亮起来。    				
	}	
}

//****************************************************
//扫描行
//****************************************************
void Scan_Line( unsigned char m)
{	
	switch(m)
	{
		case 0:			
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 					
			break;
		case 1:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 2:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 3:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 4:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 5:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 6:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 7:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 8:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 9:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;	
		case 10:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 11:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 12:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 13:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 14:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 15:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		default : break;	
	}
}

//****************************************************
//发送数据       
//****************************************************
void Send( unsigned char dat)
{
	unsigned char i;
	digitalWrite(LEDARRAY_CLK, LOW);
	delayMicroseconds(1);;	
	digitalWrite(LEDARRAY_LAT, LOW);
	delayMicroseconds(1);;

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 )
		{
			digitalWrite(LEDARRAY_DI, HIGH);	
		}
		else
		{
			digitalWrite(LEDARRAY_DI, LOW);
		}

		delayMicroseconds(1);
		digitalWrite(LEDARRAY_CLK, HIGH);				//上升沿发送数据   
			delayMicroseconds(1);
		digitalWrite(LEDARRAY_CLK, LOW);
			delayMicroseconds(1);		
		dat >>= 1; 
			
	}			
}

