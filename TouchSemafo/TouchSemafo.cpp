/*
  作成者：岩田快道
  作成日：7月3日

  ESP32に付属するピン2つのうちいずれかに触れるとそのピンのIDをMacの画面に表示する

  ISR(割り込み)関数にはIRAM_ATTR属性をつける必要がある
  ISR関数からアクセスする変数にはvolatileをつける必要がある
  割り込み処理は必要最低限の処理のみ

  割り込み処理には排他制御を組み込む

  mode:CHANGE->ピンの状態が変化した時
       FALLING->HIGHからLOWに変化した時
       RISING->LOWからHIGHに変化した時

*/

volatile int pinjudge = -1;//ISR関数からアクセスするための変数
volatile SemaphoreHandle_t xSemaphore;//ISR関数で排他制御するための変数

void IRAM_ATTR onButton1() {
  if(xSemaphoreTakeFromISR(xSemaphore, NULL) == pdTRUE){//セマフォを取得し、取得できているときに実行する
    pinjudge = 12;//ピンに触れた時にpinIDを識別するため、判定用変数の値を変更する
  }
  xSemaphoreGiveFromISR(xSemaphore, NULL);//セマフォの解放
}

void IRAM_ATTR onButton2(){
  if(xSemaphoreTakeFromISR(xSemaphore, NULL) == pdTRUE){
    pinjudge = 34;//ピンに触れた時にpinIDを識別するため、判定用変数の値を変更する
  }
  xSemaphoreGiveFromISR(xSemaphore, NULL);//セマフォの解放
}
void setup() {
  Serial.begin(115200);
  delay(50);  // Serial Init Wait

  xSemaphore = xSemaphoreCreateMutex();//競合が起きないようにミューテックスを作成するための関数で複数のタスクが同時にアクセスするのを防ぐ

  if(xSemaphore == NULL){
    while(1);//セマフォを取得できていない時の取得するまで実行する
  }

  pinMode(GPIO_NUM_12, INPUT);
  pinMode(GPIO_NUM_34, INPUT);

  attachInterrupt(GPIO_NUM_12, onButton1, FALLING);//ピンの状態が変化した時の割り込み処理
  attachInterrupt(GPIO_NUM_34, onButton2, FALLING);//ピンの状態が変化した時の割り込み処理
}
void loop() {
  
    if(pinjudge != -1){
      Serial.printf("pin ID = %d\n", pinjudge);//ピンのIDをシリアルモニタに出力

      if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE){//セマフォを取得し、それが取得できているときに条件文を回す。
                                                          //xSemaphoreはミューテックスで保存したハンドル、portMAX_DELAYは取得できるまで無限に待つことを意味する
        pinjudge = -1;//判定用変数をリセットする
        xSemaphoreGive(xSemaphore);//セマフォを解放する、解放されるとpdTRUEになる
      }
    }
}
