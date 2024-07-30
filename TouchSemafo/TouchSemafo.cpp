/*

  作成日：7月24日
  作成者：iwatakaido

*/

#include <WiFi.h>
#include <WebServer.h>
#include <Ticker.h>

//LED ON,OFFに使用するピン
#define LED_PIN 26

Ticker tickerSetHigh;//指定したタイマーで関数を呼び出すためのインスタンス
Ticker tickerSetLow;

//アクセスポイント情報
const char* ssid = "ssid";        //ssidを入力
const char* passwd = "password";      //ネットワークパスワード入力

//WebServerクラス
WebServer server(80);                     //通信を受けるポート番号（通常は80に設定）

int interval = 1000;//点滅時間の制御用変数

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
int setPin(int state){
  digitalWrite(LED_PIN, HIGH);
  delay(state);//点灯時間
  digitalWrite(LED_PIN, LOW);
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
  html += "<style>";
  html += "<title>電磁波情報学 必須課題2</title>";
  html += "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }";
  html += ".button { display: inline-block; padding: 10px 20px; font-size: 20px; cursor: pointer; text-align: center; text-decoration: none; outline: none; color: #fff; background-color: #4CAF50; border: none; border-radius: 15px; box-shadow: 0 9px #999; }";
  html += ".button:hover { background-color: #3e8e41 }";
  html += ".button:active { background-color: #3e8e41; box-shadow: 0 5px #666; transform: translateY(4px); }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<h1>電磁波情報学 必須課題2</h1>";
  html += "<p>";
  html += "Click <a href=\"/?click=on\">ON</a> / <a href=\"/?click=off\">OFF</a>";   //パラメータの送信
  html += "</p>";
  html += "<p>";
  html += "Click <a href=\"/?click=plus\">+</a>";
  html += "</p>";
  html += "<p>";
  html += "Click <a href=\"/?click=minus\">-</a>";
  html += "</p>";
  html += "</body>";
  html += "</html>";
  html += "";
  
  if (server.hasArg("click"))                      //clickが送信されたかどうかの確認
  {
    //LED ON
    if (server.arg("click").equals("on"))          //サーバーから受けとったリクエストclickの値がonかどうかの判定
    {
      digitalWrite(LED_PIN, HIGH);
    }
    //LED OFF
    else if (server.arg("click").equals("off"))    //clickの値がoffかどうかの判定
    {
      digitalWrite(LED_PIN, LOW);
    } else if(server.arg("click").equals("plus"))  //clickの値がplusかどうかの判定
    {
      interval = changeup(interval);
      setPin(interval);
    } else if(server.arg("click").equals("minus")) //clickの値がminusかどうかの判定
    {
      interval = changedown(interval);
      setPin(interval);
    } 
  }
  server.send(200, "text/html", html);
}

//存在しないアドレスへアクセスしたときの処理関数
void handleNotFound(void) {
  server.send(404, "text/plain", "Not Found");
}
