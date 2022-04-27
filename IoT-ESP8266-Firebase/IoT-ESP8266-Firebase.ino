//Inclusão de bibliotecas
//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include <ModbusMaster232.h> //ModbusMaster RTU pins   D7(13), D8(15)   RX, TX  RO,DI

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

//Declaracoes do modbus
#define TX_ENABLE_PIN D6                //Definição de pino enable
//ModbusMaster232 sensor(1, TX_ENABLE_PIN); //Buscando no slave id=1
#define BAUD_RATE_MODBUS 9600           //BaudRate

// Definição da URL do banco e da chave do projeto no Firebase
#define DATABASE_URL "teste-dbfb9-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDyONlDPSYitp_WKjODZdSWR8Wmmuii3eA"

// Insere email e senha autorizados
#define USER_EMAIL "mateusreis.ufmt@gmail.com"
#define USER_PASSWORD "testeesp"

//Definição rede a se conectar
//#define WIFI_SSID "POCO X3 Pro"
//#define WIFI_PASSWORD "dev8765@"

#define WIFI_SSID "VIVOFIBRA-16A1"
#define WIFI_PASSWORD "aayuTarsba"


// Define Objetos do Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variável para armazenar o USER UID
String uid;

// Database main path (Caminho para salvar os dados que será atualizado com USER UID)
String databasePath;

// Database child nodes
String sala1_ar1_fase1_Path = "/sala1_ar1_fase1";
String sala1_ar1_fase2_Path = "/sala1_ar1_fase2";

// Parent Node (to be updated in every loop)
String parentPath;
FirebaseJson json;

// Variavél para salvar numeros aleatórios
long randNumber;

// Define um Servidor NTP para pegar o timestamp
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variável para salvar o timestamp
int timestamp;

// Variáveis de tempo (Envia nova leitura a cada 1 segundo)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 1000;


// Inicializa o Wifi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Função para pegar o tempo do servidor NTP
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

void setup() {
  Serial.begin(115200);
  
  initWiFi();
  
  timeClient.begin();
  
  //Setup inicial do modbus
  //node.begin(BAUD_RATE_MODBUS);

  // autenticação no Firebase Real Time Database
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  //Conecta ao Firebase
  Firebase.begin(&config, &auth);

  // Pega o User UID
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  // Printa o User UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Atualiza o caminho para salvar as leituras
  databasePath = "/UsersData/" + uid + "/readings";
  
}

#define LEITURAS 10
float soma;
int vector[LEITURAS];

//definição de algumas variáveis 
//float ar1_fase1 = -1.0;
//float ar1_fase2 = -1.0;
int pos;
float media = 0;



void loop() {

  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);
    parentPath = databasePath + "/" + String(timestamp);

    // leitura do canal A
    //node.readHoldingRegisters(8, 1);
    //delay(500);
    //ar1_fase1 = node.getResponseBuffer(0);
    
    randNumber = random(15);

    vector[pos] = randNumber;
    //ar1_fase1 = randNumber/10.0F;
    
    //json.set(sala1_ar1_fase1_Path.c_str(), String(randNumber/10.0F));


    // leitura do canal B
    //node.readHoldingRegisters(16, 1);
    //delay(500);
    //ar1_fase2 = node.getResponseBuffer(0);
    
    //randNumber = random(15);
    //ar1_fase2 = randNumber/5.0F;
    pos += 1;
    if (pos > (LEITURAS - 1)){
        soma = 0;
        pos = 0;
        media = 0;
        for (int i = 0; i < LEITURAS; i++){
          soma = soma +  vector[i];
          vector[i] = 0;
        }
     
        Serial.println("Soma");
        Serial.println(soma);
        
        Serial.println("Média");
        media = soma / LEITURAS;
        json.set(sala1_ar1_fase2_Path.c_str(), String(media));
        Serial.println(media);
        Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    }
    
   

    //json.set("/timestamp", String(timestamp));
    
    
  }
  

  //***********ALGUNS EXEMPLOS PARA TRABALHAR COM DADOS USANDO FIREBASE **********

  /*
  // Atualiza valor
  Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // Obtem valor
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // Remove valor
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);

  */
}
