#include <SoftwareSerial.h>                                   // Библиотека програмной реализации обмена по UART-протоколу
#include <LowPower.h>          // библиотека сна

SoftwareSerial SIM800(8, 9);                                  // RX, TX

int pins[3] = {5, 6, 7};                                      // Пины с подключенными светодиодами

String _response    = "";                                     // Переменная для хранения ответа модуля
long lastUpdate = millis();                                   // Время последнего обновления
long updatePeriod   = 1000;                                  // Проверять каждую минуту

String phones = "+**********";   // Белый список телефонов

void beginsim800(){
  Serial.begin(9600); 
   SIM800.begin(9600);                                         // Скорость обмена данными с модемом
  Serial.println("Start!");

  sendATCommand("AT", true);                                  // Отправили AT для настройки скорости обмена данными
  //sendATCommand("AT+CMGDA=\"DEL ALL\"", true);               // Удаляем все SMS, чтобы не забивать память
//power.setSleepMode(POWERDOWN_SLEEP);
  // Команды настройки модема при каждом запуске
  //_response = sendATCommand("AT+CLIP=1", true);             // Включаем АОН
  //_response = sendATCommand("AT+DDET=1", true);             // Включаем DTMF
  _response=sendATCommand("AT+CMGF=1;&W", true);                        // Включаем текстовый режима SMS (Text mode) и сразу сохраняем значение (AT&W)!
  delay(10000);
}

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(pins[i], OUTPUT);                                 // Настраиваем пины в OUTPUT
  }
   pinMode(A3, OUTPUT);
   Serial.begin(9600); 
  // power.setSleepMode(POWERDOWN_SLEEP);
digitalWrite(A3, HIGH);
   delay(5000);
   beginsim800();
    _response = sendATCommand("AT+CMGDA=\"DEL ALL\"", true);               // Удаляем все SMS, чтобы не забивать память
  delay(5000);
    digitalWrite(A3, LOW);
  delay(10000);
 
 
  lastUpdate = millis();                                      // Обнуляем таймер
}

String sendATCommand(String cmd, bool waiting) {
  String _resp = "";                                              // Переменная для хранения результата
  Serial.println(cmd);                                            // Дублируем команду в монитор порта
  SIM800.println(cmd);                                            // Отправляем команду модулю
  if (waiting) {                                                  // Если необходимо дождаться ответа...
    _resp = waitResponse();                                       // ... ждем, когда будет передан ответ
    // Если Echo Mode выключен (ATE0), то эти 3 строки можно закомментировать
    if (_resp.startsWith(cmd)) {                                  // Убираем из ответа дублирующуюся команду
      _resp = _resp.substring(_resp.indexOf("\r", cmd.length()) + 2);
    }
    Serial.println(_resp);                                        // Дублируем ответ в монитор порта
  }
  return _resp;                                                   // Возвращаем результат. Пусто, если проблема
}

String waitResponse() {                                           // Функция ожидания ответа и возврата полученного результата
  String _resp = "";                                              // Переменная для хранения результата
  long _timeout = millis() + 10000;                               // Переменная для отслеживания таймаута (10 секунд)
  while (!SIM800.available() && millis() < _timeout)  {};         // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
  if (SIM800.available()) {                                       // Если есть, что считывать...
    _resp = SIM800.readString();                                  // ... считываем и запоминаем
  }
  else {                                                          // Если пришел таймаут, то...
    Serial.println("Timeout...");                                 // ... оповещаем об этом и...
  }
  return _resp;                                                   // ... возвращаем результат. Пусто, если проблема
}

                                             // Флаг наличия сообщений к удалению
bool flf;
bool al = HIGH;
void loop() {
 


 // delay(20000);
  Serial.println("LOOP");
  digitalWrite(A3, HIGH);
   delay(5000);
  beginsim800();
  delay(5000);
  
   delay(30000);
  checksms();
 delay(1000);
 
if (al == HIGH){
if (flf == HIGH){

   sendSMS(phones,"hai");
  delay(5000);
    
  flf = LOW;
}}
   delay(5000);
    digitalWrite(A3, LOW);
    delay(10000);
Serial.println("sleep");
delay(1000);
 attachInterrupt(1, wake_w, CHANGE);

LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // спать
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // спать
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // спать
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // спать
 
  
}
void wake_w() {
   
  Serial.println("WAKE_W");
  
 flf = HIGH;
  
}

void parseSMS(String msg) {                                   // Парсим SMS
  String msgheader  = "";
  String msgbody    = "";
  String msgphone   = "";

  msg = msg.substring(msg.indexOf("+CMGR: "));
  msgheader = msg.substring(0, msg.indexOf("\r"));            // Выдергиваем телефон

  msgbody = msg.substring(msgheader.length() + 2);
  msgbody = msgbody.substring(0, msgbody.lastIndexOf("OK"));  // Выдергиваем текст SMS
  msgbody.trim();

  int firstIndex = msgheader.indexOf("\",\"") + 3;
  int secondIndex = msgheader.indexOf("\",\"", firstIndex);
  msgphone = msgheader.substring(firstIndex, secondIndex);

  Serial.println("Phone: " + msgphone);                       // Выводим номер телефона
  Serial.println("Message: " + msgbody);                      // Выводим текст SMS

  if (msgphone.length() > 6 && phones.indexOf(msgphone) > -1) { // Если телефон в белом списке, то...
    setLedState(msgbody, msgphone);                           // ...выполняем команду
  }
  else {
    Serial.println("Unknown phonenumber");
    }
}

void setLedState (String result, String phone) {
  
if(result == "off"){
  al = LOW;
}
if(result == "on"){
  al = HIGH;
}

}
bool hasmsg = false; 
void checksms(){

// if (lastUpdate + updatePeriod < millis() ) {                    // Пора проверить наличие новых сообщений
 //do {
      _response = sendATCommand("AT+CMGL=\"REC UNREAD\",1", true);// Отправляем запрос чтения непрочитанных сообщений
      if (_response.indexOf("+CMGL: ") > -1) {                    // Если есть хоть одно, получаем его индекс
        int msgIndex = _response.substring(_response.indexOf("+CMGL: ") + 7, _response.indexOf("\"REC UNREAD\"", _response.indexOf("+CMGL: ")) - 1).toInt();
       char i = 0;                                               // Объявляем счетчик попыток
        do {
          i++;                                                    // Увеличиваем счетчик
          _response = sendATCommand("AT+CMGR=" + (String)msgIndex + ",1", true);  // Пробуем получить текст SMS по индексу
          _response.trim();                                       // Убираем пробелы в начале/конце
          if (_response.endsWith("OK")) {                         // Если ответ заканчивается на "ОК"
            if (!hasmsg) hasmsg = true;                           // Ставим флаг наличия сообщений для удаления
           sendATCommand("AT+CMGR=" + (String)msgIndex, true);   // Делаем сообщение прочитанным
            sendATCommand("\n", true);                            // Перестраховка - вывод новой строки
            parseSMS(_response);                                  // Отправляем текст сообщения на обработку
           break;                                                // Выход из do{}
          }
          else {                                                  // Если сообщение не заканчивается на OK
            Serial.println ("Error answer");                      // Какая-то ошибка
            sendATCommand("\n", true);                            // Отправляем новую строку и повторяем попытку
          }
        } while (i < 10);
     //   break;
      }
      else {
        lastUpdate = millis();                                    // Обнуляем таймер
        if (hasmsg) {
          sendATCommand("AT+CMGDA=\"DEL READ\"", true);           // Удаляем все прочитанные сообщения
          hasmsg = false;
        }
    //    break;
      }
   // }
   }

void sendSMS(String phone, String message)
{
 sendATCommand("AT+CMGS=\"" + phone + "\"", true);             // Переходим в режим ввода текстового сообщения
sendATCommand(message + "\r\n" + (String)((char)26), true);   // После текста отправляем перенос строки и Ctrl+Z
}
