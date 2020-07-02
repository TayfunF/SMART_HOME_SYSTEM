#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <virtuabotixRTC.h>    
LiquidCrystal lcd(13,12,11,10,19,18); 
const byte rows=4;  //KEYPAD SATIR
const byte cols=4;  //KEYPAD SUTUN
char keys[rows][cols]={
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'c','0','=','+'}
  };
boolean durum=0;
int basamakSayisi=0;
int sayac;
int oda1;
int oda2;
int oda3;
int salon;
int mutfak;
boolean terminal=false;
boolean kilitdurum=false;
char gelendeger=0;
boolean pencere=0;
String girilen_sifre="";
String sifre="1234";

byte satir[rows]={2,3,4,5};
byte sutun[cols]={6,7,8,9};
virtuabotixRTC myRTC(6, 7, 8);
Keypad keypad = Keypad(makeKeymap(keys),satir,sutun,rows,cols);

void setup() {
 pinMode(A5,OUTPUT);
 pinMode(23,INPUT);
 pinMode(22,OUTPUT);
 pinMode(17,INPUT);
 pinMode(14,INPUT);
 pinMode(15,OUTPUT);
 lcd.begin(16,2);
 Serial.begin(9600);
 lcd.print("ENTER THE KEY");
  Serial.println("CONTROLLING");
 Serial.println("STARTING");
 myRTC.setDS1302Time(00, 00, 14, 00, 15, 06, 2020);
}


void yazdir(){
 oda1=analogRead(1);
 oda2=analogRead(2);
 oda3=analogRead(3);
 salon=analogRead(7);
 mutfak=analogRead(6);
  float mv=(oda1/1024.0)*5000;
  float sicaklik_oda1=mv/10;
  float mvyat=(oda2/1024.0)*5000;
  float sicaklik_oda2=mvyat/10;
  float mvcocuk=(oda3/1024.0)*5000;
  float sicaklik_oda3=mvcocuk/10;
  float mvsalon=(salon/1024.0)*5000;
  float sicaklik_salon=mvsalon/10;
  float mvmutfak=(mutfak/1024.0)*5000;
  float sicaklik_mutfak=mvmutfak/10;
  Serial.print("ODA 1 = ");
  Serial.print(sicaklik_oda1);
  Serial.print("*C");
  Serial.println();
  Serial.print("ODA 2 = ");
  Serial.print(sicaklik_oda2);
  Serial.print("*C");
  Serial.println();
    Serial.print("ODA3 = ");
  Serial.print(sicaklik_oda3);
  Serial.print("*C");
  Serial.println();
    Serial.print("SALON = ");
  Serial.print(sicaklik_salon);
  Serial.print("*C");
  Serial.println();
    Serial.print("MUTFAK = ");
  Serial.print(sicaklik_mutfak);
  Serial.print("*C");
  Serial.println();
}

void password_1(){
   lcd.setCursor(0,1);
      lcd.print("TRUE PASSWORD");
      lcd.setCursor(0,0);
      lcd.print("GATE IS OPEN");
      kilitdurum=true;
      delay(50);
      lcd.setCursor(0,1);
      lcd.print("WELCOME");
      digitalWrite(15,LOW); 
      delay(100);
      for(int i=0; i<16 ;i++){
        lcd.setCursor(i,1); 
        lcd.print("");
      }
      girilen_sifre="";
      basamakSayisi=0;
}

void password_0(){
        lcd.setCursor(0,1);
      lcd.print("WRONG PASSWORD");
      delay(1000);
      sayac++;
      if(sayac==3){
        for(int j=0; j<1000 ; j++){
digitalWrite(22,HIGH);
delay(100);
digitalWrite(22,LOW); 
        }
      }
      for(int i=0; i<22 ;i++){
        lcd.setCursor(i,1); 
        lcd.print(" ");
      }
      girilen_sifre="";
      basamakSayisi=0;
}

void pencere_alarm_ac(){
digitalWrite(15,HIGH);
delay(50);
digitalWrite(15,LOW); 
}
void pencere_alarm_kapat(){
digitalWrite(15,LOW);
delay(50);
}

void elektrikac(){
digitalWrite(17,HIGH);
Serial.println("Elektrik,su");
Serial.println("Dogalgaz,acik");
}
void  elektrikkapat(){
  digitalWrite(17,LOW);
 Serial.println("Elektrik,su");
Serial.println("Dogalgaz,kapali");
}


void loop() {
  char key=keypad.getKey();
    if(key=='-'){
      lcd.setCursor(0,0);
      lcd.print("GATE IS OPEN");
      lcd.setCursor(0,1);
      lcd.print("WINDOWS LOCKED");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ENTER THE KEY");
      terminal  =true;
      kilitdurum=false;

      myRTC.updateTime();
      Serial.println("PEOPLE OUT");
      Serial.print(myRTC.dayofmonth);
      Serial.print("/");
      Serial.print(myRTC.month);
      Serial.print("/");
      Serial.print(myRTC.year);
      Serial.print(" ");
      Serial.print(myRTC.hours);
      Serial.print(":");
      Serial.print(myRTC.minutes);
      Serial.print(":");
      Serial.println(myRTC.seconds); 
      delay(1000);
    }
    
    if(key != NO_KEY){
       lcd.setCursor(basamakSayisi,1);
      lcd.print(key);
      girilen_sifre=girilen_sifre+key;
      basamakSayisi++;
  }
  if(key=='='){
    if((girilen_sifre.substring(0,basamakSayisi-1))==sifre){
      password_1();  
    }
    else  if((girilen_sifre.substring(0,basamakSayisi-1))!=sifre){
      password_0();
    }
  }
if(kilitdurum==false){
  durum=digitalRead(14);
delay(50);
if(durum==1){
pencere_alarm_ac();
    }
else  if(durum==0){
pencere_alarm_kapat();

    }
}

if(terminal==true){
  gelendeger=Serial.read();
delay(50);
if(gelendeger=='q'){
 elektrikac();
}
else if(gelendeger=='w'){
  elektrikkapat();
}
}
if(kilitdurum==false){
   gelendeger=Serial.read();
   delay(50);
   if(gelendeger=='t'){
    yazdir();
}
}   
}
