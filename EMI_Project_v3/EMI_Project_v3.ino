
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

byte dot[] = {
  B00011,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


#define ONE_WIRE_BUS 4

const int rs = 12, en = 11, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

short m_input1 = 2;
short m_input2 = 3;
short m_enable = 5;
short B_select = A3;
short B_left = A4;
short B_right = A5;

//***********************
//short T_sence = A0;
//float temp_in;
//*******************************

float tem;
short temp_cut_off = 35;


void setup(void) {

  Serial.begin(9600);

  pinMode(m_input1, OUTPUT);
  pinMode(m_input2, OUTPUT);
  pinMode(m_enable, OUTPUT);
  
//****************************
//pinMode(T_sence, INPUT);
//***************************

  pinMode(B_select, INPUT_PULLUP);
  pinMode(B_left, INPUT_PULLUP);
  pinMode(B_right, INPUT_PULLUP);

  sensors.begin();
  lcd.begin(16, 2);
  lcd.createChar(1, dot);

  digitalWrite(m_input1, HIGH);
  digitalWrite(m_input2, LOW);
}

void loop(void) {

  sensors.requestTemperatures();
  tem = (sensors.getTempCByIndex(0));

//********************************************
 // temp_in = analogRead(T_sence);
 // temp_in = temp_in * 0.004882813;
 // tem = (temp_in * 100);
//*******************************************

  //Read value of select button
  if (digitalRead(B_select) == LOW) {
    M_Menu();
  }

  if (tem >= temp_cut_off) {
    digitalWrite(m_enable, HIGH);
  }

  else {
    digitalWrite(m_enable, 0);
  }

  Serial.println("Temperature in Celsius: ");
  Serial.println(tem);

  //Function for print value of sensor in LCD
  Temp_LCD_Print();
  delay(500);
}



//FUnction start
void Temp_LCD_Print(void) {
  sensors.getTempCByIndex(0);
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  lcd.print(tem);
  lcd.setCursor(11, 0);
  lcd.write(1);
  lcd.setCursor(12, 0);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("Fan On Temp:");
  lcd.setCursor(0, 1);
  lcd.print("Fan On Temp:");
  lcd.setCursor(12, 1);
  lcd.print(temp_cut_off );
  lcd.setCursor(14, 1);
  lcd.write(1);
  lcd.setCursor(15, 1);
  lcd.print("C");
}

void M_Menu(void) {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fan Control Menu");
  delay(1000);
  lcd.clear();
  
   Fan_On ();
  
  lcd.clear();
}
//Fan on selection function

int Fan_On(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fan On Temp");
  lcd.setCursor(3, 1);
  lcd.print("<        >");
  lcd.setCursor(7, 1);
  lcd.print(temp_cut_off);

  while (1) {
    if (digitalRead(B_left) == 0) {
      temp_cut_off = temp_cut_off - 1;

      if (temp_cut_off <= 0) { temp_cut_off = 0; }
    }

    else if (digitalRead(B_right) == 0) {
      temp_cut_off = temp_cut_off + 1;

      if (temp_cut_off >= 100) { temp_cut_off = 100; }
    }

    lcd.setCursor(7, 1);
    lcd.print(temp_cut_off);
    delay(200);  //this delay is necessary for functionality

    if (digitalRead(B_select) == 0) {
      break;
    }
  }
  delay(50);
  lcd.clear();
  return 1;
}
