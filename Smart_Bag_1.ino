#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 9, 8, 7, 6);

#define DT A0
#define SCK A1
#define sw 2

long sample=0;
float val=0;
long count=0;
unsigned long readCount(void)
{
  unsigned long Count;
  unsigned char i;
  pinMode(DT, OUTPUT);
  digitalWrite(DT,HIGH);
  digitalWrite(SCK,LOW);
  Count=0;
  pinMode(DT, INPUT);
  while(digitalRead(DT));
  for (i=0;i<24;i++)
  {
    digitalWrite(SCK,HIGH);
    Count=Count<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT)) 
    Count++;
  }
  digitalWrite(SCK,HIGH);
  Count=Count^0x800000;
  digitalWrite(SCK,LOW);
  return(Count);
}
void setup()
{
  Serial.begin(9600);
  pinMode(SCK, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.print("    Weight ");
  lcd.setCursor(0,1);
  lcd.print(" Measurement ");
  delay(1000);
  lcd.clear();
  calibrate();
}

void loop()
{
  count= readCount();
  int w=(((count-sample)/val)-2*((count-sample)/val));
  Serial.print("weight:");
  Serial.print(67);
  Serial.println("g");
  lcd.setCursor(0,0);
  lcd.print("Weight            ");
  lcd.setCursor(0,1);
  lcd.print(w);
  lcd.print("g             ");

  if(digitalRead(sw)==0)
  {
    val=0;
    sample=0;
    w=0;
    count=0;
    calibrate();
  }
}
void calibrate()
{
    lcd.clear();
  lcd.print("Calibrating...");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  for(int i=0;i<100;i++)
  {
    count=readCount();
    sample+=count;
    Serial.println(count);
  }
}

