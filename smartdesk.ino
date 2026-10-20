//====================================================
// SMART DESK TEKNISI KOMPUTER BERBASIS ARDUINO
// HC-SR04  = Deteksi Pengguna
// MQ-2     = Deteksi Asap/Gas
// Relay 1  = Lampu
// Relay 2  = Kipas
//====================================================
//---------------- PIN ----------------
const int trigPin = 9;
const int echoPin = 10;
const int relayLampu = 7;
const int relayKipas = 8;
const int mq2Pin = A0;
//--------------- VARIABEL -------------
long duration;
int distance;
int mq2Value;

//----------- BATAS DETEKSI ------------
const int batasJarak = 70;
const int batasAsap = 250;   // Sesuaikan hasil kalibrasi
//----------------------------------------------------
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayLampu, OUTPUT);
  pinMode(relayKipas, OUTPUT);

  digitalWrite(relayLampu, LOW);
  digitalWrite(relayKipas, LOW);

  Serial.println("SMART DESK AKTIF");
  Serial.println("Kalibrasi MQ-2...");
  delay(15000);      // Tunggu sensor MQ-2 stabil
  Serial.println("Kalibrasi selesai");
}
//====================================================
void loop() {
  //---------------- HC-SR04 ----------------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  //---------------- Lampu ------------------
  bool statusLampu = false;

  if (distance > 0 && distance <= batasJarak) {
    digitalWrite(relayLampu, HIGH);
    statusLampu = true;
  } else {
    digitalWrite(relayLampu, LOW);
  }

  //---------------- MQ-2 -------------------
  long total = 0;

  for (int i = 0; i < 10; i++) {
    total += analogRead(mq2Pin);
    delay(5);
  }

  mq2Value = total / 10;

  //---------------- Kipas ------------------
  bool statusKipas = false;

  if (mq2Value >= batasAsap) {
    digitalWrite(relayKipas, HIGH);
    statusKipas = true;
  } else {
    digitalWrite(relayKipas, LOW);
  }

  //---------------- Serial Monitor ---------
  Serial.print("Jarak : ");
  Serial.print(distance);
  Serial.print(" cm");

  Serial.print(" | Lampu : ");
  Serial.print(statusLampu ? "ON" : "OFF");

  Serial.print(" | MQ2 : ");
  Serial.print(mq2Value);

  Serial.print(" | Kipas : ");
  Serial.print(statusKipas ? "ON" : "OFF");

  Serial.print(" | Status : ");

  if (statusKipas)
    Serial.println("ASAP/GAS TERDETEKSI");
  else
    Serial.println("AMAN");

  delay(500);
}
