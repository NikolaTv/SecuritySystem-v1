#include <SoftwareSerial.h>                                   // Библиотека програмной реализации обмена по UART-протоколу
#include <LowPower.h>          // библиотека сна

SoftwareSerial SIM800(8, 9);                                  // RX, TX

int pins[3] = {5, 6, 7};                                      // Пины с подключенными светодиодами

String _response    = "";                                     // Переменная для хранения ответа модуля
long lastUpdate = millis();                                   // Время последнего обновления
long updatePeriod   = 1000;                                  // Проверять каждую минуту

String phones = "+**********";   // Белый список телефонов

void beginsim800() {
  Serial.begin(9600);
  SIM800.begin(9600);                                         // Скорость обмена данными с модемом
  Serial.println("Start!");

  sendATCommand("AT", true);                                  // Отправили AT для настройки скорости обмена данными
  //sendATCommand("AT+CMGDA=\"DEL ALL\"", true);               // Удаляем все SMS, чтобы не забивать память
  //power.setSleepMode(POWERDOWN_SLEEP);
  // Команды настройки модема при каждом запуске
  //_response = sendATCommand("AT+CLIP=1", true);             // Включаем АОН
  //_response = sendATCommand("AT+DDET=1", true);             // Включаем DTMF
  _response = sendATCommand("AT+CMGF=1;&W", true);                      // Включаем текстовый режима SMS (Text mode) и сразу сохраняем значение (AT&W)!
  delay(10000);
}

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(pins[i], OUTPUT);                                 // Настраиваем пины в OUTPUT
  }
  pinMode(A3, OUTPUT);
  Serial.begin(9600);
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

bool hasmsg = false;                                              // Флаг наличия сообщений к удалению
bool trevoga;
void loop() {



  // delay(20000);
  Serial.println("LOOP");
  
  //delay(10000);

  if (trevoga == HIGH) {
    digitalWrite(A3, HIGH);
  delay(5000);

  beginsim800();
    sendSMS(phones, "trevoga");
    trevoga = LOW;
     delay(5000);
  digitalWrite(A3, LOW);
  }

 
  Serial.println("sleep");
  delay(1000);

  attachInterrupt(1, wake_w, CHANGE);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); // спать
delay(1000);

}
void wake_w() {

  Serial.println("WAKE_W");
  trevoga = HIGH;
  delay(1000);
}


void sendSMS(String phone, String message)
{
  sendATCommand("AT+CMGS=\"" + phone + "\"", true);             // Переходим в режим ввода текстового сообщения
  sendATCommand(message + "\r\n" + (String)((char)26), true);   // После текста отправляем перенос строки и Ctrl+Z
}
