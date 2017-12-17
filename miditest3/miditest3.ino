#define NOTE_ON 144        //ノートオン(0x90)
#define CONTROL_CHANGE 176 //コントロールチェンジ(0xB0)
#define ALL_SOUND_OFF 120  //オールサウンドオフ
 
#define CDS_NUMBER 4       //CdSセルの使用数
int note_no[] = {60,62,64,65};
int note_no1[] = {60,62,64,65};
int note_no2[] = {67,69,71,72};
 //CdSごとのノートナンバー

int CdS_val[] = {0,0,0,0};        //CdSの値格納用変数の初期化
int CdS_state[] = {0,0,0,0};      //CdSの状態(ON,OFF)格納用変数の初期化
int CdS_state_past[] = {0,0,0,0}; //CdSの過去状態格納用変数の初期化
int Pin2 = 12; 
int Pin3 = 13;
void setup() {
  //MIDI開始
  
  Serial.begin(31250);
  pinMode(Pin2, INPUT);  
  pinMode(Pin3, INPUT);  
  sendMidi(CONTROL_CHANGE, ALL_SOUND_OFF, 0); //コントロールチェンジ,オールサウンドオフ
  ma();
}
 
 
void loop() {  
  if(digitalRead(Pin2)==HIGH){
    for (int i = 0; i < CDS_NUMBER; i++) {
        note_no[i] = note_no1[i];
    }
    ma(); 
  }
  if(digitalRead(Pin3)==HIGH){
    for (int i = 0; i < CDS_NUMBER; i++) {
        note_no[i] = note_no2[i];
    }
    ma();
  }
}
void ma(){
  //CdSの値及び状態の読み取り
  for(int i=0; i < CDS_NUMBER; i++){
    CdS_val[i] = analogRead(i);
    if(CdS_val[i] < 980){            //CdSの閾値(0~1023)
      CdS_state[i] = 1;               //CdSの閾値未満：ON
    }else{
      CdS_state[i] = 0;               //CdSの閾値以上：OFF
    }
  }
  
  for(int i=0; i < CDS_NUMBER; i++){
    if(CdS_state_past[i] != CdS_state[i]){ //CdSの現在状態が過去状態と違ったら
      CdS_state_past[i] = CdS_state[i];    //CdSの過去状態を現在状態で更新
      sendMidi(NOTE_ON, note_no[i], CdS_state[i] * 127); //MIDI信号送信
    }
  }
}

void sendMidi(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

 

