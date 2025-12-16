 𝒀𝑶𝑼𝑺𝑺𝑬𝑭, [12/16/2025 5:20 PM]
#include <LiquidCrystal.h>

// تعريف أطراف الشاشة LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// حساس الموجات فوق الصوتية (اللي بيقيس المسافة)
const int trigPin = 12;
const int echoPin = 13;

// أطراف الموتور الأول (التاني مش شغّال دلوقتي)
int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4; // مش مستخدم
int motor2pin2 = 5; // مش مستخدم

// أطراف التحكم في السرعة
const int motor1ENA = 9;
const int motor2ENB = 10; // مش مستخدم

// ليدات الإضاءة للتنبيه
const int blueLED = 6;
const int greenLED = 7;
const int yellowLED = 8;
const int redLED = 11;

// متغيرات لحساب المسافة
long duration;
int distance;
int prevDistance = 0;

void setup() {
  lcd.begin(16, 2); // نبدأ تشغيل الشاشة
  lcd.print("#The Distance :"); // نكتب رسالة أول ما تشتغل

  // نجهز أطراف الحساس
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // نجهز أطراف الموتور
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  // نجهز أطراف السرعة
  pinMode(motor1ENA, OUTPUT);
  pinMode(motor2ENB, OUTPUT);

  // نجهز الليدات
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  Serial.begin(9600); // نشغل السيريال مونيتور علشان نطبع بيانات
}

void loop() {
  delay(10); // تأخير بسيط علشان القراءة تبقى مظبوطة
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // نحسب الوقت اللي الإشارة رجعت فيه ونحسب منه المسافة
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // نعرض المسافة على الشاشة
  lcd.setCursor(0, 1);
  lcd.print("                "); // نمسح السطر
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm ");

  // نخلي الموتور التاني واقف على طول
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  analogWrite(motor2ENB, 0);

  // لو قربنا قوي من حاجة، والعربية لسه بتقرب كمان
  if (distance < 15 && distance < prevDistance) {
    Serial.println("Reverse: Too close!");
    lcd.setCursor(8, 1); lcd.print("Reverse ");

    // نرجّع العربية لورا بهدوء
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(motor1ENA, 50);

    // نفضل نرجّع لورا لحد ما نبعد شوية
    while (distance < 15) {
      flickerLED(redLED); // نخلي الليد الأحمر يلمع

      delay(10); // نقرأ المسافة بسرعة
      digitalWrite(trigPin, LOW); delayMicroseconds(2);
      digitalWrite(trigPin, HIGH); delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;

      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(distance);
      lcd.print(" cm ");
      lcd.setCursor(8, 1); lcd.print("Reverse ");
      Serial.print("Reversing... Distance: ");
      Serial.println(distance);
      delay(30);
    }

    // نوقف الموتور بعد ما نبعد
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 0);
  }

  // لو المسافة قريبة جدًا (أقل من 20 سم)
  else if (distance < 20) {
    Serial.println("Stop: Very close");
    lcd.setCursor(8, 1); lcd.print("Stop    ");
    setLEDs(false, false, false, true); // نشغل الليد الأحمر

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 0); // نوقف الموتور خالص
  }

  // لو المسافة معقولة نتحرك ببطء
  else if (distance < 30) {
    Serial.println("Forward: Slow");
    lcd.setCursor(8, 1); lcd.print("Slow    ");
    flickerLED(yellowLED); // الليد الأصفر يلمع

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 50); // سرعة بطيئة
  }

  // مسافة أكتر شوية نمشي بسرعة متوسطة
  else if (distance < 40) {
    Serial.println("Forward: Medium");
    lcd.setCursor(8, 1); lcd.print("Medium  ");
    setLEDs(false, true, false, false); // نشغل الليد الأخضر

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 100);
  }

𝒀𝑶𝑼𝑺𝑺𝑬𝑭, [12/16/2025 5:20 PM]
// لو الطريق فاضي نمشي بسرعة كويسة
  else {
    Serial.println("Forward: Full Speed");
    lcd.setCursor(8, 1); lcd.print("Fast    ");
    setLEDs(true, false, false, false); // نشغل الليد الأزرق

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 150); // سرعة عالية
  }

  // نخزن آخر قراءة علشان نعرف إذا قربنا أو بعدنا
  prevDistance = distance;
  delay(20); // تأخير بسيط بين كل قراءة والتانية
}

// دالة نتحكم فيها في الليدات كلها مرة واحدة
void setLEDs(bool blue, bool green, bool yellow, bool red) {
  digitalWrite(blueLED, blue);
  digitalWrite(greenLED, green);
  digitalWrite(yellowLED, yellow);
  digitalWrite(redLED, red);
}

// دالة تخلي الليد يلمع شوية (ينوّر ويطفي)
void flickerLED(int ledPin) {
  // نطفي كل الليدات الأول
  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  // ننوّر الليد ده ونطفيه بسرعة
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}
