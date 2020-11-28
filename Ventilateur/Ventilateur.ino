// @ : vpotin
// but : contrôle d'un ventilateur par la température et affichage de données


#include <LiquidCrystal.h>

       // -- Déclaration des entrées/sorties et variables utilisées -- //

int TemperaturePin = 0;  
int VentilateurPin = 3;  
int TachymetrePin = 2; 

float temperature;
const float consigneTemp = 22.0;
const int gain = 10;


LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Déclaration de l'afficheur LCD


        // -- Traitement des données collectées -- //

float ConversionTemp(unsigned capteur) {  
  double tempK = log(10000.0 * ((1024.0 / capteur - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );
  float tempC = tempK - 273.15;
  return tempC;
  } 

unsigned LireTPM() {  
  unsigned char signal2=digitalRead(TachymetrePin);
  while (digitalRead(TachymetrePin)==signal2) {
    ;
  }
  unsigned long datefront_1=micros();
  while (digitalRead(TachymetrePin)!=signal2) {
    ;
  }
  while (digitalRead(TachymetrePin)==signal2) {
    ;
  }
  unsigned long datefront_3=micros();
  return 30000000/(datefront3-datefront1);
}

void CommandeVentilo(unsigned val) {
  OCR2B = val;
}

int borner(int v, int Min, int Max) { 
  if (v>Max) return Max;
  if (v<Min) return Min;
  return v;
}


       // -- Séquence de Fonctionnement -- //


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(VentilateurPin, OUTPUT);
  TCCR2A = 0x27; 
  TCCR2B = 0x0A;
  OCR2A = 80;
  OCR2B = 1;
  pinMode(TachymetrePin, INPUT);
  temperature = ConversionTemp(analogRead(TemperaturePin));
  

}

void loop() {
  unsigned capteur = analogRead(TemperaturePin);
  temperature = ConversionTemp(capteur);
  int commande = borner((temperature-consigneTemp)*gain,1,79);
  CommandeVentilo(commande);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temperature");
  lcd.setCursor(6, 0);
  lcd.print(temperature);
  delay(1000);
  lcd.clear();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Vitesse");
  lcd.setCursor(6, 0);
  lcd.print(LireTPM());
  delay(1000);
  lcd.clear();

}
