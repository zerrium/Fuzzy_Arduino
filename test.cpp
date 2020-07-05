#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <unistd.h>
#include <algorithm>

using namespace std;

//deklarasi variable global
float suhu,kelembapan,kecepatan_kipas;
string hasil_suhu[6],hasil_kelembapan[6],rules[2],kondisi_kipas;
float nilai_suhu[6],nilai_kelembapan[6];
float nilai_implikasi_x[4],nilai_predikat[2];
float Def_nilai_suhu[2],Def_nilai_kelembapan[2],batas_suhu[2];
float N_suhu;
float N_kelembapan;
float a1,a2,aa1,aa2,aa3,M1,M2,M3,selisih,z;

string hasil_cahaya[6],hasil_waktu[6],rules2[2],kondisi_lampu;
float nilai_cahaya[6],nilai_waktu[6];
float nilai_implikasi_x2[4],nilai_predikat2[2];
float Def_nilai_cahaya[2],Def_nilai_waktu[2],batas_cahaya[3];
float N_cahaya;
float N_waktu;
float a3,a4,aa4,aa5,aa6,M4,M5,M6,selisih2,z2;

//prosedur sensor 
int random_cahaya(){ //every 2 secs
  int T_cahaya;
  T_cahaya = 550;
  return roundf(T_cahaya * 100)/100;
}

float random_waktu(){
  float T_waktu;
  T_waktu = 14.05; //get time from RTC Blynk server
  return roundf(T_waktu * 100)/100;
}

float random_suhu(){ //every 2 secs
  float T_suhu;
  T_suhu = 30;
  return roundf(T_suhu * 100)/100;
}

float random_kelembapan_udara(){ //every 2 secs
  float T_kelembapan;
  T_kelembapan = 70;
  return roundf(T_kelembapan * 100)/100;
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
  //cout<<("Nilai A Cahaya  : "+string(N_cahaya));
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
    cout<<"Suhu : "<<N_suhu<<endl;
    cout<<"Kelembapan : "<<N_kelembapan<<endl;
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
        
        //cout<<("hasil_suhu : "+string(hasil_suhu[i]));
        //cout<<("nilai_suhu : "+string(nilai_suhu[i]));
        tz++;
      }
    }
    
    int tx = 0;
    for(int i=0; i<NK; i++){
    
      if(nilai_kelembapan[i] != 0){
        Def_nilai_kelembapan[tx]= nilai_kelembapan[i];
        rules[1] = hasil_kelembapan[i];
        //cout<<("hasil_kelembapan : "+string(hasil_kelembapan[i]));
        //cout<<("nilai_kelembapan : "+string(nilai_kelembapan[i]));
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
    
    //cout<<("T_defuzi_suhu : "+string(T_defuzi_suhu));
    //cout<<("T_defuzi_kelembapan : "+string(T_defuzi_kelembapan));
    
    const int T_hasil_implikasi = sizeof(nilai_implikasi_x) / sizeof(float);
    
    for(int i = 0 ; i < T_hasil_implikasi ; i++)
    {
      //cout<<("Hasil Implikasi "+string(i+1)+" : "+string(nilai_implikasi_x[i]));
    }
    
      for(int i = 0 ; i < T_hasil_implikasi/2 ; i++)
    {
      nilai_predikat[i] = max(nilai_implikasi_x[i],nilai_implikasi_x[i+2]);
      //cout<<("Hasil predikat "+string(i+1)+" : "+string(nilai_predikat[i]));
    }
    //cout<<();
}

void predikat2(){
    cout<<"cahaya : "<<N_cahaya<<endl;
    cout<<"waktu : "<<N_waktu<<endl;
    const int N = sizeof(nilai_cahaya) / sizeof(float);
    const int NK = sizeof(nilai_waktu) / sizeof(float);
    int tz = 0;
    for(int i=0; i<N; i++){
        //cout<<("hasil_cahaya : "+string(hasil_cahaya[i]));
        //cout<<("nilai_cahaya : "+string(nilai_cahaya[i]));    
      if(nilai_cahaya[i] != 0){

        Def_nilai_cahaya[tz]= nilai_cahaya[i];
        
        if(nilai_cahaya[i]>nilai_cahaya[i+1] && nilai_cahaya[i+1] !=0){
          rules2[0] = hasil_cahaya[i];
        }
        else {
          rules2[0] = hasil_cahaya[i];
        }
        
        //cout<<("hasil_cahaya : "+string(hasil_cahaya[i]));
        //cout<<("nilai_cahaya : "+string(nilai_cahaya[i]));
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
        //cout<<("hasil_waktu : "+string(hasil_waktu[i]));
        //cout<<("nilai_waktu : "+string(nilai_waktu[i]));
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
      //cout<<("Hasil Implikasi "+string(i+1)+" : "+string(nilai_implikasi_x2[i]));
    }
    
      for(int i = 0 ; i < T_hasil_implikasi/2 ; i++)
    {
      nilai_predikat2[i] = max(nilai_implikasi_x2[i],nilai_implikasi_x2[i+2]);
      //cout<<("Hasil predikat "+string(i+1)+" : "+string(nilai_predikat2[i]));
    }
    //cout<<();
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
  
  //cout<<("a1 : "+string(a1));
  //cout<<("a2 : "+string(a2));
  
  //cout<<("aa1 : "+string(aa1));
  //cout<<("M1 : "+string(M1));
  
  //cout<<("aa2 : "+string(aa2));
  //cout<<("M2 : "+string(M2));
  
  //cout<<("aa3 : "+string(aa3));
  //cout<<("M3 : "+string(M3));
  
  cout<<"Z kipas : "<<z<<endl;
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
   //cout<<("Max Predikat2 : "+string(max_predikat2));
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
  
  //cout<<("a3 : "+string(a3));
  //cout<<("a4 : "+string(a4));
  
  //cout<<("aa4 : "+string(aa4));
  //cout<<("M4 : "+string(M4));
  
  //cout<<("aa5 : "+string(aa5));
  //cout<<("M5 : "+string(M5));
  
  //cout<<("aa6 : "+string(aa6));
  //cout<<("M6 : "+string(M6));
  
  cout<<"Z Lampu : "<<z2<<endl;
}

void main()
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
  }
  else if(z>=25 && z<=45){
    kondisi_kipas = "pelan";
  }
  else if(z>=45 && z<=65){
    kondisi_kipas = "sedang";
  }
  else if(z>=65){
    kondisi_kipas = "cepat";
  }
  cout<<("kondisi_kipas : "+kondisi_kipas)<<endl;
  cout<<("-------------------------------------------\n")<<endl;

  if(z2 <= 10){
    kondisi_lampu = "mati";
  }
  else if(z2>5 && z2<=75){
    kondisi_lampu = "redup";
  }
  else if(z2>=70 && z2<=135){
    kondisi_lampu = "terang";
  }
  else if(z2>=135){
    kondisi_lampu = "sangat terang";
  }
  
  cout<<"z : "<<z<<endl;
  cout<<("kondisi_lampu : "+string(kondisi_lampu))<<endl;
  cout<<endl;
}