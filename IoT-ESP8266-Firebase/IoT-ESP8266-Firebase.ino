//Inclusão de bibliotecas
#include <ModbusMaster232.h> //ModbusMaster RTU pins   D7(13), D8(15)   RX, TX  RO,DI
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//Declaracoes do modbus
#define TX_ENABLE_PIN D6                //Definição de pino enable
ModbusMaster232 node(1, TX_ENABLE_PIN); //Buscando no slave id=1
#define BAUD_RATE_MODBUS 9600           //BaudRate

// Definição do host e da chave do Firebase
#define FIREBASE_HOST "firebase-host"
#define FIREBASE_AUTH "firebase-key"

//Definição rede a se conectar
#define WIFI_SSID "sua-rede"
#define WIFI_PASSWORD "sua-senha"

void setup() {
  Serial.begin(9600);

  //Setup inicial do modbus
  node.begin(BAUD_RATE_MODBUS); 

  //Conecta ao wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("conectado: ");
  Serial.println(WiFi.localIP());

  //Conecta ao Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

//definição de algumas variáveis 
int n = 0;

float ar1_fase1 = -1.0;
float ar1_fase2 = -1.0;
float ar2_fase1 = -1.0;
float ar2_fase2 = -1.0;


void loop() {
  //Registro [0]
  //uint16_t data;

  node.readHoldingRegisters(1, 1);
  //Serial.println("REG[1]");                        
  //Serial.println(node.getResponseBuffer(0));        
  //delay(500);
  ar1_fase1 = node.getResponseBuffer(0);
  // Passando valor
  Firebase.setFloat("ar1_fase1", ar1_fase1);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }else{
    Serial.print("setting ar1_fase1: ");
    Serial.println(ar1_fase1);
  }
  node.clearResponseBuffer();
  delay(1000);
  

  //enviando um valor aleatório ao firebase somente para testes
  ar1_fase2 = n + 2.2;
  Firebase.setFloat("ar1_fase2", ar1_fase2);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }else{
    Serial.print("setting ar1_fase2: ");
    Serial.println(ar1_fase2);
  }
  delay(1000);


  //enviando um valor aleatório ao firebase somente para testes
  ar2_fase1 = n + 3.3;
  Firebase.setFloat("ar2_fase1", ar2_fase1);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }else{
    Serial.print("setting ar2_fase1: ");
    Serial.println(ar2_fase1);
  }
  delay(1000);

  //enviando um valor aleatório ao firebase somente para testes
  ar2_fase2 = n + 3.4;
  Firebase.setFloat("ar2_fase2", ar2_fase2);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }else{
    Serial.print("setting ar2_fase2: ");
    Serial.println(ar2_fase2);
  }
  delay(1000);


  //somente para enviar alguns valores aleatórios ao firebase
  if(n<=20){
    n++;
  }else{
    n = 0;  
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
