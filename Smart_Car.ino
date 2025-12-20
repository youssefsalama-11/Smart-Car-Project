#include <LiquidCrystal.h>

// تعريف أطراف شاشة LCD: RS, E, D4, D5, D6, D7
// LCD pins definition: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// حساس الموجات فوق الصوتية (لقياس المسافة)
// Ultrasonic sensor pins (distance measurement)
const int trigPin = 12;
const int echoPin = 13;

// أطراف الموتور الأول (الموتور الثاني غير مستخدم حالياً)
// Motor 1 pins (Motor 2 is currently unused)
int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4; // غير مستخدم / Not used
int motor2pin2 = 5; // غير مستخدم / Not used

// أطراف التحكم في السرعة (PWM)
// Speed control pins (PWM)
const int motor1ENA = 9;
const int motor2ENB = 10; // غير مستخدم / Not used

// ليدات التنبيه
// Status indicator LEDs
const int blueLED   = 6;
const int greenLED  = 7;
const int yellowLED = 8;
const int redLED    = 11;

// متغيرات حساب المسافة
// Distance calculation variables
long duration;
int distance;
int prevDistance = 0;

void setup() {
  lcd.begin(16, 2);              // تشغيل الشاشة / Initialize LCD
  lcd.print("#The Distance :");  // رسالة البداية / Startup message

  // إعداد أطراف حساس الموجات فوق الصوتية
  // Ultrasonic sensor pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // إعداد أطراف الموتورات
  // Motor pins setup
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  // إعداد أطراف التحكم في السرعة
  // Speed control pins setup
  pinMode(motor1ENA, OUTPUT);
  pinMode(motor2ENB, OUTPUT);

  // إعداد الليدات
  // LED pins setup
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  Serial.begin(9600); // تشغيل السيريال للمراقبة / Start Serial Monitor
}

void loop() {
  delay(10); // تأخير بسيط لاستقرار القراءة / Small delay for stable reading

  // إرسال نبضة للحساس
  // Send trigger pulse
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // حساب المسافة
  // Calculate distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // عرض المسافة على الشاشة
  // Display distance on LCD
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm ");

  // تثبيت الموتور الثاني متوقف دائماً
  // Keep motor 2 always stopped
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  analogWrite(motor2ENB, 0);

  // اقتراب شديد + المسافة بتقل
  // Too close and still approaching
  if (distance < 15 && distance < prevDistance) {
    Serial.println("Reverse: Too close!");
    lcd.setCursor(8, 1);
    lcd.print("Reverse ");

    // الرجوع للخلف ببطء
    // Reverse slowly
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(motor1ENA, 50);

    // الاستمرار في الرجوع حتى تزداد المسافة
    // Keep reversing until distance increases
    while (distance < 15) {
      flickerLED(redLED); // وميض الليد الأحمر / Red LED flicker

      delay(10);
      digitalWrite(trigPin, LOW);  
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;

      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(distance);
      lcd.print(" cm ");
      lcd.setCursor(8, 1);
      lcd.print("Reverse ");

      Serial.print("Reversing... Distance: ");
      Serial.println(distance);
      delay(30);
    }

    // إيقاف الموتور بعد الابتعاد
    // Stop motor after safe distance
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 0);
  }

  // مسافة خطيرة جداً
  // Extremely close distance
  else if (distance < 20) {
    Serial.println("Stop: Very close");
    lcd.setCursor(8, 1);
    lcd.print("Stop    ");
    setLEDs(false, false, false, true); // LED أحمر / Red LED

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 0);
  }

  // حركة بطيئة
  // Slow movement
  else if (distance < 30) {
    Serial.println("Forward: Slow");
    lcd.setCursor(8, 1);
    lcd.print("Slow    ");
    flickerLED(yellowLED); // LED أصفر / Yellow LED

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 50);
  }

  // سرعة متوسطة
  // Medium speed
  else if (distance < 40) {
    Serial.println("Forward: Medium");
    lcd.setCursor(8, 1);
    lcd.print("Medium  ");
    setLEDs(false, true, false, false); // LED أخضر / Green LED

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 100);
  }

  // طريق مفتوح – سرعة عالية
  // Clear path – high speed
  else {
    Serial.println("Forward: Full Speed");
    lcd.setCursor(8, 1);
    lcd.print("Fast    ");
    setLEDs(true, false, false, false); // LED أزرق / Blue LED

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(motor1ENA, 150);
  }

  // حفظ آخر مسافة
  // Store last distance reading
  prevDistance = distance;
  delay(20);
}

// التحكم في كل الليدات مرة واحدة
// Control all LEDs at once
void setLEDs(bool blue, bool green, bool yellow, bool red) {
  digitalWrite(blueLED, blue);
  digitalWrite(greenLED, green);
  digitalWrite(yellowLED, yellow);
  digitalWrite(redLED, red);
}

// وميض الليد
// LED flicker function
void flickerLED(int ledPin) {
  // إطفاء جميع الليدات
  // Turn off all LEDs
  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  // تشغيل وإطفاء الليد المطلوب
  // Blink selected LED
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}
