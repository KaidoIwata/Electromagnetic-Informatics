/*

  作成日：7月24日
  作成者：iwatakaido

*/

#include <WiFi.h>
#include <WebServer.h>
#include <Ticker.h>

//LED ON,OFFに使用するピン
#define LED_PIN 26

Ticker tickerBlink;//指定したタイマーで関数を呼び出すためのインスタンス

//アクセスポイント情報
const char* ssid = "ssid";        //ssidを入力
const char* passwd = "password";      //ネットワークパスワード入力

//WebServerクラス
WebServer server(80);                     //通信を受けるポート番号（通常は80に設定）

int interval = 1000;//点滅時間の制御用変数

bool blinkflag = false;

int changeup(int interval){  //点滅の感覚を早くする
  if(interval >= 100)
  {
    interval = interval - 100;//100ms早くする
  }

  return interval;
}

int changedown(int interval){ //点滅の感覚を長くする
  if(interval <= 2000)
  {
    interval = interval + 100;//100ms遅くする
  }

  return interval;
}

//早くor遅く光らせる関数
void BlinkLED(){
  if(blinkflag){
      digitalWrite(LED_PIN, LOW);
      delay(interval);
      digitalWrite(LED_PIN, HIGH);
      delay(interval);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  //シリアルポート設定
  Serial.begin(115200);                   //シリアル通信のデータ転送レートを115200bpsで指定
  pinMode(LED_PIN, OUTPUT);                   //LED_PINで指定したピンをOutput指定
  digitalWrite(LED_PIN, LOW);                 //LED_PINで指定したピンをLowにする
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

void loop() {
  server.handleClient();//常にサーバーから受け取ったリクエストをチェックし、またそのリクエストを転送するためのWebServerのインスタンス
}

//TOPページにアクセスしたきの処理関数

//on_offのアドレスにアクセスしたときの処理関数
void handleLedOnOff() {
  String html;
  //HTML記述
  html = "<!DOCTYPE html>";
  html += "<html lang='ja'>";
  html += "<meta charset=\"utf-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<head>";
  html += "<title>電磁波情報学 必須課題2</title>";
  html += "</head>";
  html += "<body>";
  html += "<h1>電磁波情報学 必須課題2</h1>";
  html += "<p>";
  html += "<button><a href=\"/?click=swich\">ON/OFF</a></button>";   //パラメータの送信
  html += "</p>";
  html += "<p>";
  html += "<button><a href=\"/?click=plus\">+</a></button>";
  html += "</p>";
  html += "<p>";
  html += "<button><a href=\"/?click=minus\">-</a></button>";
  html += "</p>";
  html += "</body>";
  html += "</html>";
  html += "";
  
  if (server.hasArg("click"))                      //clickが送信されたかどうかの確認
  {
          //LED ON/OFF
      if (server.arg("click").equals("swich"))          //サーバーから受けとったリクエストclickの値がonかどうかの判定
      {
        blinkflag = !blinkflag;//フラグを反転させる
      } else if(server.arg("click").equals("plus"))  //clickの値がplusかどうかの判定
      {
        interval = changeup(interval);
      } else if(server.arg("click").equals("minus")) //clickの値がminusかどうかの判定
      {
        interval = changedown(interval);
      }
      tickerBlink.attach_ms(interval, BlinkLED); // 点滅時間の更新
  }
  server.send(200, "text/html", html);
}

//存在しないアドレスへアクセスしたときの処理関数
void handleNotFound(void) {
  server.send(404, "text/plain", "Not Found");
}
