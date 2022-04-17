#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Definição do host e da chave do Firebase
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

//Definição rede a se conectar
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

void setup() {
  Serial.begin(9600);

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

int n = 0;

void loop() {


  float corrente1 = n + 1;
  // Passando valor
  Firebase.setFloat("corrente1", corrente1);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  
  float corrente2 = n + 2.2;
  Firebase.setFloat("corrente2", corrente2);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);


  float corrente3 = n + 3.3;
  Firebase.setFloat("corrente3", corrente3);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);


  float corrente4 = n + 3.4;
  Firebase.setFloat("corrente4", corrente4);
  // Captura erro
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);


  if(n<=20){
    n++;
  }else{
    n = 0;  
  }
  

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
