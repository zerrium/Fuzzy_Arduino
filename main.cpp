#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <unistd.h>
#include <algorithm>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <HardwareSerial.h>
#include <PZEM004Tv30.h>
#include <Arduino.h>
#include <BH1750.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "juA6wjomDY8aYWS_UYGoLqGLSZSNJpIs"; //Don't touch plz

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "iPhone";
char pass[] = "valdymami190531";

BH1750 lightMeter; //Light sensor declaration

const int DHTPIN = 23;     // Temp sensor pin
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

float hum;  //Stores humidity value
float temp; //Stores temperature value

HardwareSerial S(2);
PZEM004Tv30 pzem(&S); //Pzem declaration

BlynkTimer timer; //timer for looping function
WidgetRTC rtc; //rtc time from Blynk server

const int device1 = 18; //GPIO pin from ESP to relay module
const int device2 = 19;
int fan = LOW; //to save fan on/off state

const byte pwm1 = 27;
const int pwm2 = 26;

const int freq = 976; //Using UNO PWM freq
const int pwm1Channel = 0; //anything between 0-15
const int pwm2Channel = 1;
const int resolution = 8; //0-255

float R; //PWM formula for linear beightness output to LED

//deklarasi variable global
float suhu,kelembapan,kecepatan_kipas;
String hasil_suhu[6],hasil_kelembapan[6],rules[2],kondisi_kipas;
float nilai_suhu[6],nilai_kelembapan[6];
float nilai_implikasi_x[4],nilai_predikat[2];
float Def_nilai_suhu[2],Def_nilai_kelembapan[2],batas_suhu[2];
float N_suhu;
float N_kelembapan;
float a1,a2,aa1,aa2,aa3,M1,M2,M3,selisih,z;

String hasil_cahaya[6],hasil_waktu[6],rules2[2],kondisi_lampu;
float nilai_cahaya[6],nilai_waktu[6];
float nilai_implikasi_x2[4],nilai_predikat2[2];
float Def_nilai_cahaya[2],Def_nilai_waktu[2],batas_cahaya[3];
float N_cahaya;
float N_waktu;
float a3,a4,aa4,aa5,aa6,M4,M5,M6,selisih2,z2;
bool firstBoot=true; //flag to sync variables from Blynk cloud
const int cost=1467; //cost per kWh in IDR

BLYNK_CONNECTED() {
  if(firstBoot){ //Get last value from Blynk server and sync it to the board upon boot
    Blynk.virtualWrite(V0, HIGH); //lampu button, ON right on boot
    Blynk.syncVirtual(V2); //kipas button
    firstBoot=false;
  }
}

BLYNK_WRITE(V0){ //lampu button
  if(firstBoot){
    digitalWrite(device1, LOW); //ON relay (inverted)
  }
  //Serial.println("RELAYYY BUTTON TRIGGERED!!!");
  int x = param.asInt(); //get parameter from Blynk app
  int val = LOW;
  if(x==LOW){
    val=HIGH; //Relay, inverted input
  }
  //Serial.println(x);
  //Serial.println(val);
  digitalWrite(device1, val);
}

BLYNK_WRITE(V2){ //kipas button
  //Serial.println("RELAYYY BUTTON TRIGGERED!!!");
  fan = param.asInt(); //get parameter from Blynk app
  if(fan==LOW){
    ledcWrite(pwm1Channel, 255); //PWM
  }
}

//prosedur sensor 
int random_cahaya(){ //every 2 secs
  int T_cahaya;
  T_cahaya = 135.0;//lightMeter.readLightLevel();
  Blynk.virtualWrite(V9, T_cahaya); //Update value to Blynk app
  return roundf(T_cahaya * 100)/100;
}

float random_waktu(){
  float T_waktu;
  T_waktu = 11;//hour()+(minute()/60)+(second()/3600); //get time from RTC Blynk server
  return roundf(T_waktu * 100)/100;
}

float random_suhu(){ //every 2 secs
  float T_suhu;
  T_suhu = dht.readTemperature();
  Blynk.virtualWrite(V4, T_suhu); //Update value to Blynk app
  return roundf(T_suhu * 100)/100;
}

float random_kelembapan_udara(){ //every 2 secs
  float T_kelembapan;
  T_kelembapan = dht.readHumidity();
  Blynk.virtualWrite(V5, T_kelembapan); //Update value to Blynk app
  return roundf(T_kelembapan * 100)/100;
}

void readPzem(){
  float powe = pzem.power();
  //Serial.print("Power: ");
  //Serial.print(powe);
  //Serial.println("W");
  Blynk.virtualWrite(V6, roundf(powe*10)/10); //Update value to Blynk app

  float ener = pzem.energy();
  //Serial.print("Energy: ");
  //Serial.print(ener,3);
  //Serial.println("kWh");
  Blynk.virtualWrite(V7, roundf(ener*100)/100); //Update value to Blynk app

  Blynk.virtualWrite(V8, roundf(ener*cost*100)/100); //Update value to Blynk app

  //Serial.println("\n--------------");
}



void cari_suhu(){
  N_suhu = random_suhu();  
  //cari nilai_suhu anggota 
  //suhu
  //dingin
  if(N_suhu <= 21){

    hasil_suhu[0] = "dingin";
    nilai_suhu[0] = 1;
  }
  else if(N_suhu > 21 && N_suhu <=23)
  {

    hasil_suhu[0] = "dingin";
    nilai_suhu[0] = (23- N_suhu) / (23-21);
  }
  else{

    hasil_suhu[0] = "dingin";
    nilai_suhu[0] = 0;
  }
  
  //sejuk
    //bawah
  if(N_suhu >= 22 && N_suhu <= 24){

      hasil_suhu[1] = "sejuk";
      nilai_suhu[1] = (N_suhu - 22) / (24-22);
    }
  else{

      hasil_suhu[1] = "sejuk";
      nilai_suhu[1] = 0;
    }
    
    //atas
  if(N_suhu > 24 && N_suhu <= 27){

      hasil_suhu[2] = "sejuk";
      nilai_suhu[2] = ((27 - N_suhu) / (27-24));
    }
  else{

      hasil_suhu[2] = "sejuk";
      nilai_suhu[2] = 0;
    }
  
  //hangat
    //bawah
  if(N_suhu >= 26 && N_suhu <= 28){

      hasil_suhu[3] = "hangat";
      nilai_suhu[3] = ((N_suhu - 26) / (28-26));
    }
  else{

      hasil_suhu[3] = "hangat";
      nilai_suhu[3] = 0;
    }
    
    //atas
  if(N_suhu > 28 && N_suhu <= 30){

      hasil_suhu[4] = "hangat";
      nilai_suhu[4] = (30 - N_suhu) / (30-28);
    }
  else{

      hasil_suhu[4] = "hangat";
      nilai_suhu[4] = 0;
    }
    
  //panas
  if(N_suhu <= 29){

    hasil_suhu[5] = "panas";
    nilai_suhu[5] = 0;
  }
  else if(N_suhu > 29  && N_suhu <= 31)
  {

    hasil_suhu[5] = "panas";
    nilai_suhu[5] = (N_suhu - 29) / (31-29);
  }
  else{

    hasil_suhu[5] = "panas";
    nilai_suhu[5] = 1;
  }
  
}

void cari_kelembapan(){
  //kelembapan
  //kering
  N_kelembapan = random_kelembapan_udara();
  if(N_kelembapan <= 40)
  {

    hasil_kelembapan[0] = "kering";
    nilai_kelembapan[0] = 1;
  }
  else if(40 < N_kelembapan && N_kelembapan <= 45.5 )
  {

    hasil_kelembapan[0] = "kering";
    nilai_kelembapan[0] = (45.5-N_kelembapan) / (45.5 - 40);
  }
  else
  {

    hasil_kelembapan[0] = "kering";
    nilai_kelembapan[0] = 0;
  }
  
  
  //kering bawah
  if(45 <= N_kelembapan && N_kelembapan <= 55)
  {

    hasil_kelembapan[1] = "normal";
    nilai_kelembapan[1] = (N_kelembapan - 45) / (55 - 45);
  }
  else 
  {

    hasil_kelembapan[1] = "normal";
    nilai_kelembapan[1] = 0;
  }
  
  //kering atas
  if(55 < N_kelembapan && N_kelembapan <= 65)
  {

    hasil_kelembapan[2] = "normal";
    nilai_kelembapan[2] = (65 - N_kelembapan) / (65 - 55);
  }
  else 
  {

    hasil_kelembapan[2] = "normal";
    nilai_kelembapan[2] = 0;
  }
  
  //basah
  if(N_kelembapan <= 60.5)
  {

    hasil_kelembapan[3] = "basah";
    nilai_kelembapan[3] = 0;
  }
  else if(60.5 < N_kelembapan && N_kelembapan <= 70 )
  {

    hasil_kelembapan[3] = "basah";
    nilai_kelembapan[3] = (N_kelembapan - 60.5) / (70 - 60.5);
  }
  else
  {
    hasil_kelembapan[3] = "basah";
    nilai_kelembapan[3] = 1;
  }
}
  
void cari_cahaya(){
  N_cahaya = random_cahaya(); 
  //Serial.println("Nilai A Cahaya  : "+String(N_cahaya));
  //cari nilai_cahaya anggota 
  //suhu
  //Gelap
  if(N_cahaya <= 10){

    hasil_cahaya[0] = "Gelap";
    nilai_cahaya[0] = 1;
  }
  else if( 10 < N_cahaya  && N_cahaya <=25)
  {

    hasil_cahaya[0] = "Gelap";
    nilai_cahaya[0] = (25- N_cahaya) / (25-10);
  }
  else{

    hasil_cahaya[0] = "Gelap";
    nilai_cahaya[0] = 0;
  }
  
  //Sangat_redup
    //bawah
  if( 20 <= N_cahaya && N_cahaya <= 60){

      hasil_cahaya[1] = "Sangat_redup";
      nilai_cahaya[1] = (N_cahaya - 20) / (60-20);
    }
  else{

      hasil_cahaya[1] = "Sangat_redup";
      nilai_cahaya[1] = 0;
    }
    
    //atas
  if(60 < N_cahaya  && N_cahaya <= 100){

      hasil_cahaya[2] = "Sangat_redup";
      nilai_cahaya[2] = ((100 - N_cahaya) / (100-60));
    }
  else{

      hasil_cahaya[2] = "Sangat_redup";
      nilai_cahaya[2] = 0;
    }
  
  //Redup
    //bawah
  if(95 <= N_cahaya && N_cahaya <= 150){

      hasil_cahaya[3] = "Redup";
      nilai_cahaya[3] = ((N_cahaya - 95) / (150-95));
    }
  else{

      hasil_cahaya[3] = "Redup";
      nilai_cahaya[3] = 0;
    }
    
    //atas
  if(150 < N_cahaya  && N_cahaya <= 200){

      hasil_cahaya[4] = "Redup";
      nilai_cahaya[4] = (200 - N_cahaya) / (200 - 150);

    }
  else{

      hasil_cahaya[4] = "Redup";
      nilai_cahaya[4] = 0;
    }
    
  //Terang
  if(N_cahaya <= 200){

    hasil_cahaya[5] = "Terang";
    nilai_cahaya[5] = 0;
  }
  else if(195 < N_cahaya  && N_cahaya <= 250)
  {

    hasil_cahaya[5] = "Terang";
    nilai_cahaya[5] = (N_cahaya - 195) / (250-195);
  }
  else{

    hasil_cahaya[5] = "Terang";
    nilai_cahaya[5] = 1;
  }
  
  
}

void cari_waktu(){
  //kelembapan
  //Dini_hari
  N_waktu = random_waktu();
  if(N_waktu <= 5)
  {

    hasil_waktu[0] = "Dini_hari";
    nilai_waktu[0] = 1;
  }
  else if(5 < N_waktu && N_waktu <= 7 )
  {

    hasil_waktu[0] = "Dini_hari";
    nilai_waktu[0] = (7-N_waktu) / (7 - 5);
  }
  else
  {

    hasil_waktu[0] = "Dini_hari";
    nilai_waktu[0] = 0;
  }
  
  
  // bawah
  if(6 <= N_waktu && N_waktu <= 12)
  {

    hasil_waktu[1] = "Pagi_sore";
    nilai_waktu[1] = (N_waktu - 6) / (12 - 6);
     
  }
  else 
  {

    hasil_waktu[1] = "Pagi_sore";
    nilai_waktu[1] = 0;
  }
  
  // atas
  if(12 < N_waktu && N_waktu <= 19)
  {

    hasil_waktu[2] = "Pagi_sore";
    nilai_waktu[2] = (19 - N_waktu) / (19 - 12);
  }
  else 
  {

    hasil_waktu[2] = "Pagi_sore";
    nilai_waktu[2] = 0;
  }
  
  //M_istirahat
    // bawah
  if(18 <= N_waktu && N_waktu <= 20)
  {

    hasil_waktu[3] = "Malam";
    nilai_waktu[3] = (N_waktu - 18) / (20 - 18);
  }
  else 
  {

    hasil_waktu[3] = "Malam";
    nilai_waktu[3] = 0;
  }
  
  // atas
  if(20 < N_waktu && N_waktu <= 22)
  {

    hasil_waktu[4] = "Malam";
    nilai_waktu[4] = (22 - N_waktu) / (22 - 20);
  }
  else 
  {

    hasil_waktu[4] = "Malam";
    nilai_waktu[4] = 0;
  }

  //malam istirahat
  if(N_waktu < 21)
  {

    hasil_waktu[5] = "M_istirahat";
    nilai_waktu[5] = 0;
  }
  else if(21 < N_waktu && N_waktu <= 23 )
  {

    hasil_waktu[5] = "M_istirahat";
    nilai_waktu[5] = (N_waktu - 21) / (23 - 21);
  }
  else
  {
    hasil_waktu[5] = "M_istirahat";
    nilai_waktu[5] = 1;
  }
}

void predikat(){
    Serial.println("Suhu : "+String(N_suhu));
    Serial.println("Kelembapan : "+String(N_kelembapan));
    const int N = sizeof(nilai_suhu) / sizeof(float);
    const int NK = sizeof(nilai_kelembapan) / sizeof(float);
    int tz = 0;
    for(int i=0; i<N; i++){
    
      if(nilai_suhu[i] != 0){
        Def_nilai_suhu[tz]= nilai_suhu[i];

        if(nilai_suhu[i]>nilai_suhu[i+1] && nilai_suhu[i+1] !=0){
          rules[0] = hasil_suhu[i];
        }
        else {
          rules[0] = hasil_suhu[i];
        }
        
        //Serial.println("hasil_suhu : "+String(hasil_suhu[i]));
        //Serial.println("nilai_suhu : "+String(nilai_suhu[i]));
        tz++;
      }
    }
    
    int tx = 0;
    for(int i=0; i<NK; i++){
    
      if(nilai_kelembapan[i] != 0){
        Def_nilai_kelembapan[tx]= nilai_kelembapan[i];
        rules[1] = hasil_kelembapan[i];
        //Serial.println("hasil_kelembapan : "+String(hasil_kelembapan[i]));
        //Serial.println("nilai_kelembapan : "+String(nilai_kelembapan[i]));
        tx++;
      }
          
      
    }
    
    const int T_defuzi_suhu = sizeof(Def_nilai_suhu) / sizeof(float);
    const int T_defuzi_kelembapan = sizeof(Def_nilai_kelembapan) / sizeof(float);
    int T_implikasi = 0 ;
    for(int x = 0 ; x < T_defuzi_suhu ; x++ )
    {
      for(int y = 0 ; y < T_defuzi_kelembapan ; y++)
      {
//        nilai_implikasi_x[T_implikasi] = min(Def_nilai_suhu[y],Def_nilai_kelembapan[x]);
        nilai_implikasi_x[T_implikasi] = min(Def_nilai_suhu[y],Def_nilai_kelembapan[x]);
        T_implikasi++;
      }
    }
    
    //Serial.println("T_defuzi_suhu : "+String(T_defuzi_suhu));
    //Serial.println("T_defuzi_kelembapan : "+String(T_defuzi_kelembapan));
    
    const int T_hasil_implikasi = sizeof(nilai_implikasi_x) / sizeof(float);
    
    for(int i = 0 ; i < T_hasil_implikasi ; i++)
    {
      //Serial.println("Hasil Implikasi "+String(i+1)+" : "+String(nilai_implikasi_x[i]));
    }
    
      for(int i = 0 ; i < T_hasil_implikasi/2 ; i++)
    {
      nilai_predikat[i] = max(nilai_implikasi_x[i],nilai_implikasi_x[i+2]);
      //Serial.println("Hasil predikat "+String(i+1)+" : "+String(nilai_predikat[i]));
    }
    //Serial.println();
}

void predikat2(){
    Serial.println("cahaya : "+String(N_cahaya));
    Serial.println("waktu : "+String(N_waktu));
    const int N = sizeof(nilai_cahaya) / sizeof(float);
    const int NK = sizeof(nilai_waktu) / sizeof(float);
    int tz = 0;
    for(int i=0; i<N; i++){
        //Serial.println("hasil_cahaya : "+String(hasil_cahaya[i]));
        //Serial.println("nilai_cahaya : "+String(nilai_cahaya[i]));    
      if(nilai_cahaya[i] != 0){

        Def_nilai_cahaya[tz]= nilai_cahaya[i];
        
        if(nilai_cahaya[i]>nilai_cahaya[i+1] && nilai_cahaya[i+1] !=0){
          rules2[0] = hasil_cahaya[i];
        }
        else {
          rules2[0] = hasil_cahaya[i];
        }
        
        //Serial.println("hasil_cahaya : "+String(hasil_cahaya[i]));
        //Serial.println("nilai_cahaya : "+String(nilai_cahaya[i]));
        tz++;
      }
    }
    
    int tx = 0;
    for(int i=0; i<NK; i++){
     
      if(nilai_waktu[i] != 0){
        Def_nilai_waktu[tx]= nilai_waktu[i];
         if(nilai_waktu[i]>nilai_waktu[i+1] && nilai_waktu[i+1] !=0){
            rules2[1] = hasil_waktu[i];
         }
        else {
            rules2[1] = hasil_waktu[i];
          }
        //Serial.println("hasil_waktu : "+String(hasil_waktu[i]));
        //Serial.println("nilai_waktu : "+String(nilai_waktu[i]));
        tx++;
      }
          
      
    }
    
    const int T_defuzi_suhu = sizeof(Def_nilai_cahaya) / sizeof(float);
    const int T_defuzi_kelembapan = sizeof(Def_nilai_waktu) / sizeof(float);
    int T_implikasi = 0 ;
    for(int x = 0 ; x < T_defuzi_suhu ; x++ )
    {
      for(int y = 0 ; y < T_defuzi_kelembapan ; y++)
      {
        nilai_implikasi_x2[T_implikasi] = min(Def_nilai_cahaya[y],Def_nilai_waktu[x]);
        T_implikasi++;
      }
    }
    
    const int T_hasil_implikasi = sizeof(nilai_implikasi_x2) / sizeof(float);
    
    for(int i = 0 ; i < T_hasil_implikasi ; i++)
    {
      //Serial.println("Hasil Implikasi "+String(i+1)+" : "+String(nilai_implikasi_x2[i]));
    }
    
      for(int i = 0 ; i < T_hasil_implikasi/2 ; i++)
    {
      nilai_predikat2[i] = max(nilai_implikasi_x2[i],nilai_implikasi_x2[i+2]);
      //Serial.println("Hasil predikat "+String(i+1)+" : "+String(nilai_predikat2[i]));
    }
    //Serial.println();
}

void defuzzy(){
  //1
  if ( rules[0] == "dingin" && rules[1] == "basah"){
    batas_suhu[0] = 25;
    batas_suhu[1] = 0;
        batas_suhu[2] = 10;

  }
  else if ( rules[0] == "dingin" && rules[1] == "kering"){
    batas_suhu[0] = 25;
    batas_suhu[1] = 0;
    batas_suhu[2] = 10;
    
  }
  else if ( rules[0] == "dingin" && rules[1] == "normal"){
    batas_suhu[0] = 25;
    batas_suhu[1] = 0;
    batas_suhu[2] = 10;
    
  }
  
  //2
  else if ( rules[0] == "sejuk" && rules[1] == "basah"){
    batas_suhu[0] = 65;
    batas_suhu[1] = 55;
    batas_suhu[2] = 45;
    
  }
  else if ( rules[0] == "sejuk" && rules[1] == "kering"){
    batas_suhu[0] = 25;
    batas_suhu[1] = 0;
    batas_suhu[2] = 10;
    
  }
  else if ( rules[0] == "sejuk" && rules[1] == "normal"){
    batas_suhu[0] = 45;
    batas_suhu[1] = 35;
    batas_suhu[2] = 25;
    
  }
  
  
  //3
  else if ( rules[0] == "hangat" && rules[1] == "basah"){
    batas_suhu[0] = 65;
    batas_suhu[1] = 55;
    batas_suhu[2] = 45;
    
  }
  else if ( rules[0] == "hangat" && rules[1] == "kering"){
    batas_suhu[0] = 45;
    batas_suhu[1] = 35;
    batas_suhu[2] = 25;
    
  }
  else if ( rules[0] == "hangat" && rules[1] == "normal"){
    batas_suhu[0] = 65;
    batas_suhu[1] = 55;
    batas_suhu[2] = 45;
    
  }
  
  //4
  else if ( rules[0] == "panas" && rules[1] == "basah"){
    batas_suhu[0] = 75;
    batas_suhu[1] = 0;
    batas_suhu[2] = 65;
    
  }
  else if ( rules[0] == "panas" && rules[1] == "kering"){
    batas_suhu[0] = 75;
    batas_suhu[1] = 0;
    batas_suhu[2] = 65;
  }
  else if ( rules[0] == "panas" && rules[1] == "normal"){
    batas_suhu[0] = 75;
    batas_suhu[1] = 0;
    batas_suhu[2] = 65;
    
  }
  

  
  selisih = batas_suhu[0] - batas_suhu[1];
  float max_predikat=max(nilai_predikat[0],nilai_predikat[1]);
  
  if(rules[0] == "panas" || rules[0] == "dingin"){
    a1 = (max_predikat*(batas_suhu[1]-batas_suhu[2]))+batas_suhu[2];
    a2 = batas_suhu[0] - (max_predikat*(batas_suhu[0]-batas_suhu[2]));
  
    
    aa2 = (a2-a1) * max_predikat;
    M2 = ((max_predikat/2) * ( pow(a2,2) )) - ((max_predikat/2) * ( pow(a1,2) )); 
    
    if(rules[0] == "panas"){
    aa1 = ((a2 - batas_suhu[2])/2) * max_predikat;
    M1 = (( (0.1/3) * (pow(a1,3))) - ((6.5/2) * (pow(a1,2))) ) - (((0.1/3) * (pow(batas_suhu[2],3)) )- ( (6.5/2) * (pow(batas_suhu[2],2)) ));
    aa3 = 0;
    M3 = 0;
    
    }
    
    else{
    aa1 = 0;
    M1 = 0;
    
    aa3 = ((batas_suhu[0] - a2)/2) * max_predikat;
    M3 = (((2.5/2) * ( pow(batas_suhu[0],2) ))- (0.033333 * (pow(batas_suhu[0],3)))) - (((2.5/2) * ( pow(a2,2) ))- (0.033333 * (pow(a2,3))));
    }
    
    
  }
  
  else if(rules[0] == "hangat" || rules[0] == "sejuk"){
    a1 = (max_predikat*(batas_suhu[1]-batas_suhu[2]))+batas_suhu[2];
    a2 = batas_suhu[0] - (max_predikat*(batas_suhu[0]-batas_suhu[1]));
  
    
    aa2 = (a2-a1) * max_predikat;
    M2 = ((max_predikat/2) * ( pow(a2,2) )) - ((max_predikat/2) * ( pow(a1,2) )); 
    
  
    aa1 = ((a1 - batas_suhu[2])/2) * max_predikat;
    M1 = (( (0.1/3) * (pow(a1,3))) - ((4.5/2) * (pow(a1,2))) ) - (((0.1/3) * (pow(batas_suhu[2],3)) )- ( (4.5/2) * (pow(batas_suhu[2],2)) )); 
    
    aa3 = ((batas_suhu[0] - a2)/2) * max_predikat;
    M3 = (((6.5/2) * ( pow(batas_suhu[0],2) ))- (0.033333 * (pow(batas_suhu[0],3)))) - (((6.5/2) * ( pow(a2,2) ))- (0.033333 * (pow(a2,3))));
    
    
    
  }
  z = (M1 + M2 +M3) / (aa1 + aa2 + aa3);
  
  //Serial.println("a1 : "+String(a1));
  //Serial.println("a2 : "+String(a2));
  
  //Serial.println("aa1 : "+String(aa1));
  //Serial.println("M1 : "+String(M1));
  
  //Serial.println("aa2 : "+String(aa2));
  //Serial.println("M2 : "+String(M2));
  
  //Serial.println("aa3 : "+String(aa3));
  //Serial.println("M3 : "+String(M3));
  
  Serial.println("Z kipas : "+String(z));
}

void defuzzy2(){
  //1
  if ( rules2[0] == "Gelap" && rules2[1] == "M_istirahat"){
    batas_cahaya[0] = 75;
    batas_cahaya[1] = 30;
    batas_cahaya[2] = 10;
    
  }
  else if ( rules2[0] == "Gelap" && rules2[1] == "Dini_hari"){
    batas_cahaya[0] = 75;
    batas_cahaya[1] = 30;
    batas_cahaya[2] = 10;
    
  }
  else if ( rules2[0] == "Gelap" && rules2[1] == "Pagi_sore"){
      batas_cahaya[0] = 130;
      batas_cahaya[1] = 110;
      batas_cahaya[2] = 75;
    
   }

  else if ( rules2[0] == "Gelap" && rules2[1] == "Malam"){
    batas_cahaya[0] = 145;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 130;
    
  }
  
  //2
  else if ( rules2[0] == "Sangat_redup" && rules2[1] == "M_istirahat"){
    batas_cahaya[0] = 10;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 0;
    
  }
  else if ( rules2[0] == "Sangat_redup" && rules2[1] == "Dini_hari"){
    batas_cahaya[0] = 10;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 0;
    
  }
  else if ( rules2[0] == "Sangat_redup" && rules2[1] == "Pagi_sore"){
    batas_cahaya[0] = 75;
    batas_cahaya[1] = 30;
    batas_cahaya[2] = 10;
    
  }

  else if ( rules2[0] == "Sangat_redup" && rules2[1] == "Malam"){
    batas_cahaya[0] = 145;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 130;
    
  }
  
  
  //3
  else if ( rules2[0] == "Redup" && rules2[1] == "M_istirahat"){
    batas_cahaya[0] = 10;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 0;
    
  }
  else if ( rules2[0] == "Redup" && rules2[1] == "Dini_hari"){
    batas_cahaya[0] = 10;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 0;
    
  }
  else if ( rules2[0] == "Redup" && rules2[1] == "Pagi_sore"){
    batas_cahaya[0] = 75;
    batas_cahaya[1] = 30;
    batas_cahaya[2] = 10;
    
  }

  else if ( rules2[0] == "Redup" && rules2[1] == "Malam"){
    batas_cahaya[0] = 145;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 130;
    
  }
  
  //4
  else if ( rules2[0] == "Terang" && rules2[1] == "M_istirahat"){
    batas_cahaya[0] = 10;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 0;
    
  }
  else if ( rules2[0] == "Terang" && rules2[1] == "Dini_hari"){
    batas_cahaya[0] = 10;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 0;
  }
  else if ( rules2[0] == "Terang" && rules2[1] == "Pagi_sore"){
    batas_cahaya[0] = 10;
    batas_cahaya[1] = 0;
    batas_cahaya[2] = 0;
    
  }

  else if ( rules2[0] == "Terang" && rules2[1] == "Malam"){
    batas_cahaya[0] = 130;
    batas_cahaya[1] = 110;
    batas_cahaya[2] = 75;
    
  }
  

  
  selisih2 = batas_cahaya[0] - batas_cahaya[2];
  
  float max_predikat2=max(nilai_predikat2[0],nilai_predikat2[1]);
   //Serial.println("Max Predikat2 : "+String(max_predikat2));
  //off
    if(rules2[1] == "M_istirahat" && rules2[0] == "Sangat_redup"|| rules2[1] == "Dini_hari"  && rules2[0] == "Sangat_redup" || rules2[1] == "M_istirahat"&& rules2[0] == "Redup" || rules2[1] == "Dini_hari"  && rules2[0] == "Redup" || rules2[1] == "M_istirahat" && rules2[0] == "Terang"|| rules2[1] == "Dini_hari" && rules2[0] == "Terang"|| rules2[1] == "Pagi_sore" && rules2[0] == "Terang"){
    a3 = (max_predikat2*(batas_cahaya[1]-batas_cahaya[2]))+batas_cahaya[2];
    a4 = batas_cahaya[0] - (max_predikat2*(batas_cahaya[0]-batas_cahaya[2]));
  
    
    aa5 = (a4-a3) * max_predikat2;
    M5 = ((max_predikat2/2) * ( pow(a4,2) )) - ((max_predikat2/2) * ( pow(a3,2) )); 
    
    aa4 = 0;
    M4 = 0;
    
    aa6 = ((batas_cahaya[0] - a4)/2) * max_predikat2;
    M6 = (pow((1.0/2) * batas_cahaya[0],2) - pow(0.1/3 * batas_cahaya[0],3)) - ((( pow((1.0/2) * a4,2) ))- ( pow(0.1/3 * a4,3)));

    
    
  }
  //redup
  else if(rules2[1] == "M_istirahat"  && rules2[0] == "Gelap" || rules2[1] == "Dini_hari"  && rules2[0] == "Gelap" || rules2[1] == "Pagi_sore"  && rules2[0] == "Sangat_redup" || rules2[1] == "Pagi_sore"  && rules2[0] == "Redup"){
    a3 = (max_predikat2*(batas_cahaya[1]-batas_cahaya[2]))+batas_cahaya[2];
    a4 = batas_cahaya[0] - (max_predikat2*(batas_cahaya[0]-batas_cahaya[1]));
  
    
    aa5 = (a4-a3) * max_predikat2;
    M5 = ((max_predikat2/2) * ( pow(a4,2) )) - ((max_predikat2/2) * ( pow(a3,2) )); 
    
  
    aa4 = ((a3 - batas_cahaya[2])/2) * max_predikat2;
    M4 = (( ((1/20)/3) * (pow(a3,3))) - (((10/20)/2) * (pow(a3,2))) ) - ((((1/20)/3) * (pow(batas_cahaya[2],3)) )- ( ((10/20)/2) * (pow(batas_cahaya[2],2)) )); 
    
    aa6 = ((batas_cahaya[0] - a4)/2) * max_predikat2;
    M6 = ((((75/45)/2) * ( pow(batas_cahaya[0],2) ))- (((1/45)/3) * (pow(batas_cahaya[0],3)))) - ((((75/45)/2) * ( pow(a4,2) ))- (((1/45)/3) * (pow(a4,3))));
    
    
    
  }
//terang
else if(rules2[1] == "Malam"  && rules2[0] == "Terang" || rules2[1] == "Pagi_sore"  && rules2[0] == "Gelap"){
    a3 = (max_predikat2*(batas_cahaya[1]-batas_cahaya[2]))+batas_cahaya[2];
    a4 = batas_cahaya[0] - (max_predikat2*(batas_cahaya[0]-batas_cahaya[1]));
  
    
    aa5 = (a4-a3) * max_predikat2;
    M5 = ((max_predikat2/2) * ( pow(a4,2) )) - ((max_predikat2/2) * ( pow(a3,2) )); 
    
  
    aa4 = ((a3 - batas_cahaya[2])/2) * max_predikat2;
    M4 = (( (1.0/35/3) * pow(a3,3)) - ((75.0/35/2) * (pow(a3,2))) ) - (((1.0/35/3) * (pow(batas_cahaya[2],3)) )- ( ((75.0/35)/2) * (pow(batas_cahaya[2],2)) )); 
    
    aa6 = ((batas_cahaya[0] - a4)/2) * max_predikat2;
    M6 = ((((130.0/20)/2) * ( pow(batas_cahaya[0],2) ))- (((1.0/20)/3) * (pow(batas_cahaya[0],3)))) - ((((130.0/20)/2) * ( pow(a4,2) ))- (((1.0/20)/3) * (pow(a4,3))));
    
    
    
  }
  
  //sangat terang
    else if(rules2[1] == "Malam" && rules2[0] == "Sangat_redup" || rules2[1] == "Malam"&& rules2[0] == "Redup" || rules2[1] == "Malam" && rules2[0] == "Gelap"){
    a3 = (max_predikat2*(batas_cahaya[1]-batas_cahaya[2]))+batas_cahaya[2];
    a4 = batas_cahaya[0] - (max_predikat2*(batas_cahaya[0]-batas_cahaya[2]));
  
    
    aa5 = (a4-a3) * max_predikat2;
    M5 = ((max_predikat2/2) * ( pow(a4,2) )) - ((max_predikat2/2) * ( pow(a3,2) )); 
    
    aa4 = (a4-batas_cahaya[2]) / 2 * max_predikat2;
    M4 = (((1.0/15)/3)*(pow(a3,3))-(((130.0/15)/2)*(pow(a3,2))))-(((1.0/15)/3)*(pow(batas_cahaya[2],3))-(((130.0/15)/2) * pow(batas_cahaya[2],2)));//((1.0/15.0/3.0* pow(a3,3))- 130.0/15.0/2.0 * pow(a3,2)) - 1.0/15.0/3.0 * pow(batas_cahaya[2],3) - 130.0/15.0/2 * pow(batas_cahaya[2],2);
    
    aa6 = 0;
    M6 = 0;

    
    
  }
  
  z2 = (M4 + M5 +M6) / (aa4 + aa5 + aa6);
  
  //Serial.println("a3 : "+String(a3));
  //Serial.println("a4 : "+String(a4));
  
  //Serial.println("aa4 : "+String(aa4));
  //Serial.println("M4 : "+String(M4));
  
  //Serial.println("aa5 : "+String(aa5));
  //Serial.println("M5 : "+String(M5));
  
  //Serial.println("aa6 : "+String(aa6));
  //Serial.println("M6 : "+String(M6));
  
  Serial.println("Z Lampu : "+String(z2));
}

void setup(){
  Serial.begin(115200);
  //Serial.println("Check 1"); //just for debugging
  Blynk.begin(auth,ssid,pass);
  //WiFi.begin(ssid, pass);  // Non-blocking if no WiFi available
  //Serial.println("Check 1.1");
  //Blynk.config(auth);
  //Serial.println("Check 1.2");
  //Blynk.connect();
  //Serial.println("Check 2");
  Wire.begin(); //I2C Light sensor, pin 22 and 21
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire); //I2C Address and light sensor mode
  dht.begin();
  rtc.begin();
  //Serial.println("Check 3");
  pinMode(device1, OUTPUT);
  digitalWrite(device1, HIGH); //HIGH is off, LOW is on (revert input to relay module)
  pinMode(device2, OUTPUT);
  digitalWrite(device2, HIGH);
  //Serial.println("Check 4");
  setSyncInterval(3600); // Sync interval in seconds (1 hr)
  //Serial.println("Check 5");
  timer.setInterval(2000L, loopp); //run loopp function every 2 seconds
  timer.setInterval(1000L, readPzem); //read pzem data every sec
  R = (3 * log10(2))/(log10(255)); //PWM formula for LED
  pinMode(pwm1, OUTPUT);
  ledcSetup(pwm1Channel, freq, resolution); //PWM setup
  ledcAttachPin(pwm1, pwm1Channel); //PWM initialization
  pinMode(pwm2, OUTPUT);
  ledcSetup(pwm2Channel, 500000, resolution);
  ledcAttachPin(pwm2, pwm2Channel);
  Serial.println("OK");
}

void loopp()
{   
  cari_suhu();
  cari_kelembapan();
  predikat();
  defuzzy();
  cari_cahaya();
  cari_waktu();
  predikat2();
  defuzzy2();

  if(z <= 25){
    kondisi_kipas = "mati";
    ledcWrite(pwm1Channel, 255); //Inverted input to PWM fan
  }
  else if(z>=25 && z<=45){
    kondisi_kipas = "pelan";
    if(fan==HIGH){
      ledcWrite(pwm1Channel, 170); 
    }else{
      ledcWrite(pwm1Channel, 255);
    }
  }
  else if(z>=45 && z<=65){
    kondisi_kipas = "sedang";
    if(fan==HIGH){
      ledcWrite(pwm1Channel, 85); 
    }else{
      ledcWrite(pwm1Channel, 255);
    }
  }
  else if(z>=65){
    kondisi_kipas = "cepat";
    if(fan==HIGH){
      ledcWrite(pwm1Channel, 0);  
    }else{
      ledcWrite(pwm1Channel, 255);
    }
  }
  if(fan==HIGH){
    Blynk.virtualWrite(V3, kondisi_kipas); //Update value to Blynk app
  }else{
    Blynk.virtualWrite(V3, "mati"); 
  }
  Serial.println("kondisi_kipas : "+kondisi_kipas);
  Serial.println("-------------------------------------------\n");

  if(z2 <= 10){
    kondisi_lampu = "mati";
    ledcWrite(pwm2Channel, 0);
  }
  else if(z2>5 && z2<=75){
    kondisi_lampu = "redup";
    ledcWrite(pwm2Channel, pow(2, (1 / R)) - 1); //PWM formula for LED
  }
  else if(z2>=70 && z2<=135){
    kondisi_lampu = "terang";
    ledcWrite(pwm2Channel, pow(2, (2 / R)) - 1);
  }
  else if(z2>=135){
    kondisi_lampu = "sangat terang";
    ledcWrite(pwm2Channel, 255);
  }
  
  Serial.println("z : "+String(z));
  if(digitalRead(device1)==LOW){
    Blynk.virtualWrite(V1, kondisi_lampu); //Update value to Blynk app
  }else{
    Blynk.virtualWrite(V1, "mati");
  }
  Serial.println("kondisi_lampu : "+String(kondisi_lampu));
  Serial.println();
}

void loop(){
  Blynk.run();
  timer.run();
}

//Last change on 5th July 2020 at 22:49