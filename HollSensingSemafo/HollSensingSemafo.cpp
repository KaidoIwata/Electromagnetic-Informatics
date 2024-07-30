/*
   -作成日-
   7月27日

   -氏名-
   岩田快道

   -概要-
   磁石に反応する内部の内部のホールセンサーを利用して、
   磁石との距離を丸めて計算し、
   距離によってLEDを光らせるプログラム

*/
#include <WiFi.h>
#include <WebServer.h>


//アクセスポイント情報
const char* ssid = "ssid";        //ssidを入力
const char* passwd = "password";      //ネットワークパスワード入力

//WebServerクラス
WebServer server(80);                     //通信を受けるポート番号（通常は80に設定）

int i = 0;//カウンター変数
int sum = 0;//丸めた値
volatile int hallValue = 0;

//Core1で動く関数
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //マルチコアタスク(タスク名、"タスク名", スタックメモリサイズ、起動パラメータ、タスク優先順位、タスクハンドル、CoreID)
  xTaskCreateUniversal(task1, "task1", 8192, NULL, 1, NULL, 0);//タスクの作成

  WiFi.begin(ssid, passwd);               //アクセスポイント接続のためのIDとパスワードの設定

  while (WiFi.status() != WL_CONNECTED) { //接続状態の確認
    delay(300);                           //接続していなければ0.3秒待つ
    Serial.print(".");                    //接続しなかったらシリアルモニタに「.」と表示
  }

  //通信が可能となったら各種情報を表示する
  Serial.println("");                     //シリアルモニタ改行
  Serial.println("WiFi Connected");       //接続したらシリアルモニタに「WiFi Connected」と表示
  Serial.print("IP Address : ");          //シリアルモニタに表示
  Serial.println(WiFi.localIP());         //割り当てられたIPアドレスをシリアルモニタに表示

  //serverにアクセスしたときの処理関数
  server.on("/", handleLedOnOff);         //TOPページのアドレスにアクセスしたときの処理関数
  server.onNotFound(handleNotFound);      //存在しないアドレスにアクセスしたときの処理関数
  
  //WebServerを起動、server(80)で作成したサーバー
  server.begin();                         //WebServer起動
}

//Core1で動く関数, WDTを無効にして動いている
void loop() {
  server.handleClient();
  // put your main code here, to run repeatedly:

  if(i > 2){//数値を丸めて処理
    sum = sum / (i + 1);
    i = 0;//iを初期化

    delay(1000);
  }

  hallValue = hallRead();//ホールセンサの値を読み取る
  sum += hallValue;
  i ++;
}

void task1(void *pvParameters){//LED点灯制御用関数
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);

  while(1){
    if (server.hasArg("click")){
      if (server.arg("click").equals("start")){

        Serial.println(sum);//ホールセンサーの値を表示する

        if(hallValue >= 50){
          digitalWrite(25,HIGH);
          digitalWrite(26, LOW);
          digitalWrite(27, LOW);
        } else if(hallValue < 50 && hallValue > 10){
          digitalWrite(25,LOW);
          digitalWrite(26, HIGH);
          digitalWrite(27, LOW);
        } else {
          digitalWrite(25,LOW);
          digitalWrite(26, LOW);
          digitalWrite(27, HIGH);
        }
      } else if(server.arg("click").equals("stop")){
        digitalWrite(25,LOW);
        digitalWrite(26,LOW);
        digitalWrite(27,LOW);
      } 
    }
    delay(1);//Watch Dog Timerをリセットし、ループを継続させる
  }
}

void handleLedOnOff() {
    String html;
  //HTML記述
  html = "<!DOCTYPE html>";
  html += "<html lang='ja'>";
  html += "<meta charset=\"utf-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<head>";
  html += "<title>電磁波情報学 自由課題2</title>";
  html += "</head>";
  html += "<body>";
  html += "<h1>電磁波情報学 自由課題2</h1>";
  html += "<p>";
  html += "Click <a href=\"/?click=start\">計測開始</a>";
  html += " Click <a href=\"/?click=stop\">計測停止</a>";
  html += "</p>";
  html += "</body>";
  html += "</html>";
  html += "";
  server.send(200, "text/html", html);
}

//存在しないアドレスへアクセスしたときの処理関数
void handleNotFound(void) {
  server.send(404, "text/plain", "Not Found");
}

