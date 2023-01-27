#include <Wire.h> // I²C iletişim için gerekli..
#include <VL53L0X.h> //Sensör kütüphanesi
#include <LCD_I2C.h>
#include <SPI.h>
#include <WiFi.h>
#include <ThingSpeak.h>
VL53L0X sensor; //sensor adında bir sensör objesi oluşturuldu...

LCD_I2C lcd(0x27, 16, 2);  //Kullandığımız LCD I²C adres ve karakter satır sayısı tanımlama
float cm;
float mm;
WiFiClient client;
long myChannelNumber = 2018188;                    //değişkeni uzun bir veri türü olarak tanımladık
const char myWriteAPIKey[] = "8HEUH7OHJI30ICQV";   //değişkenin değerinin atandıktan sonra değiştirilemeyeceği anlamına gelir

void setup()
{
  Serial.begin(9600);
  WiFi.begin("ilyasancar", "ilyasm12323");
  Wire.begin(); //
  Wire.begin();
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);

 
 while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }

   lcd.begin();  //LCD başlasın
  lcd.clear();  // ve ekran temizlensin
  lcd.backlight(); //lcd arka plan yansın

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).

  sensor.startContinuous();//Sensör, continious modda başlatıldı...
}

/********************************************************************
  LOOP_LOOPLOOPLOOPLOOPLOOPLOOPLOOPLOOPLOOPLOOP_
 ********************************************************************/
void loop()
{
  Serial.print(sensor.readRangeContinuousMillimeters()); //"sensor.readRangeContinuousMillimeters()" değerin atandığı fonksiyon...
  if (sensor.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }

  Serial.println();
  cm = sensor.readRangeContinuousMillimeters() / (float) 10;
  sensor.readRangeContinuousMillimeters();
  //LCD kodları
  
  lcd.setCursor(0, 0);
  lcd.print("NM Yazilim ");
  lcd.setCursor(0, 1);
  lcd.print("Tekstil");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Olculen Mesafe:  ");
  lcd.setCursor(0, 1);
  lcd.print(sensor.readRangeContinuousMillimeters()); 
  lcd.setCursor(4, 1);
  lcd.print("mm");
  lcd.setCursor(8, 1);
  lcd.print(cm,1);
  lcd.setCursor(13, 1);
  lcd.print("cm");
  delay(1000); // Saniyede bir okuma ve lcd ye yazma
  lcd.clear(); // LCD clear edilmezse değerler üst üse binebilir...
  ThingSpeak.setField(1, sensor.readRangeContinuousMillimeters());
  ThingSpeak.setField(2, cm);
   //ThingSpeak.setField(3, Akim_mA);
   //ThingSpeak.setField(4, Lvoltage);
   //ThingSpeak.setField(5, Guc_mW);
   ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//ThingSpeak.writeField (myChannelNumber, 1, temperature, myWriteAPIKey);
//ThingSpeak.writeField (myChannelNumber, 2, event.pressure, myWriteAPIKey);  
//ThingSpeak.writeField (myChannelNumber, 3, Akim_mA, myWriteAPIKey);
//ThingSpeak.writeField (myChannelNumber, 4, Lvoltage, myWriteAPIKey);  
//ThingSpeak.writeField (myChannelNumber, 5, Guc_mW, myWriteAPIKey);  
delay(10);
}