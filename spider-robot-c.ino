#include <Servo.h>
#include <PS2X_lib.h>

// Servo definitions
Servo pivotFrontLeft, liftFrontLeft;
Servo pivotFrontRight, liftFrontRight;
Servo pivotBackLeft, liftBackLeft;
Servo pivotBackRight, liftBackRight;
// Servo 8
Servo pivotFrontLeft2, liftFrontLeft2;
Servo pivotFront

// Calibration offsets
int high = 130;  // Lift height
int slow_lift = 50;
int slow_pivot = 150;

int walk_left = 30;
int walk_right = 30;
int walk_back_right = 50;
int walk_back_left = 50;
int walk_speed = 150;         // Default walking speed (delay)
bool x_pressed = false;       // ตัวแปรใช้ติดตามสถานะการกดปุ่ม X
bool square_pressed = false;  // ตัวแปรใช้ติดตามสถานะการกดปุ่มสี่เหลี่ยม
// ตัวแปรสำหรับตรวจสอบสถานะการเคลื่อนที่
static bool isWalking = false;  // เริ่มต้นว่าไม่เคลื่อนที่
// กำหนด PS2 controller
PS2X ps2x;
int error = 0;
byte type = 0;
byte vibrate = 0;


// ตัวแปรสำหรับตรวจจับสถานะปุ่ม
static bool prevUpState = false;
static bool prevDownState = false;
static bool prevLeftState = false;
static bool prevRightState = false;
static bool prevGreenState = false;
static bool prevTleftState = false;
static bool prevRedState = false;


void center_servos() {
  pivotFrontLeft.write(90);
  liftFrontLeft.write(110);
  pivotFrontRight.write(90);
  liftFrontRight.write(110);
  pivotBackLeft.write(90);
  liftBackLeft.write(110);
  pivotBackRight.write(90);
  liftBackRight.write(110);
}

void WALK_SEQUENCE() {
  // Left front
  liftFrontLeft.write(high);
  delay(50);
  pivotFrontLeft.write(60);
  delay(walk_speed);
  liftFrontLeft.write(110);
  delay(50);
  pivotFrontLeft.write(90);

  // Right back
  liftBackRight.write(high);
  delay(50);
  pivotBackRight.write(140);
  delay(walk_speed);
  liftBackRight.write(110);
  delay(50);
  pivotBackRight.write(90);

  // Right front
  liftFrontRight.write(high);
  delay(50);
  pivotFrontRight.write(120);
  delay(walk_speed);
  liftFrontRight.write(110);
  delay(50);
  pivotFrontRight.write(90);

  // Left back
  liftBackLeft.write(high);
  delay(50);
  pivotBackLeft.write(40);
  delay(walk_speed);
  liftBackLeft.write(110);
  delay(50);
  pivotBackLeft.write(90);
}

void WALKBACK_SEQUENCE() {
  // Right back
  liftBackRight.write(high);
  delay(50);
  pivotBackRight.write(60);
  delay(walk_speed);
  liftBackRight.write(110);
  delay(50);
  pivotBackRight.write(90);

  // Left front
  liftFrontLeft.write(high);
  delay(50);
  pivotFrontLeft.write(140);
  delay(walk_speed);
  liftFrontLeft.write(110);
  delay(50);
  pivotFrontLeft.write(90);

  // Left back
  liftBackLeft.write(high);
  delay(50);
  pivotBackLeft.write(120);
  delay(walk_speed);
  liftBackLeft.write(110);
  delay(50);
  pivotBackLeft.write(90);

  // Right front
  liftFrontRight.write(high);
  delay(50);
  pivotFrontRight.write(40);
  delay(walk_speed);
  liftFrontRight.write(110);
  delay(50);
  pivotFrontRight.write(90);
}

void TURN_LEFT_SEQUENCE() {
  // Left back moves backward
  liftFrontLeft.write(high);
  delay(50);
  pivotFrontLeft.write(120);
  delay(walk_speed);
  liftFrontLeft.write(110);
  delay(50);
  pivotFrontLeft.write(90);

  // Right front moves forward
  liftFrontRight.write(high);
  delay(50);
  pivotFrontRight.write(120);
  delay(walk_speed);
  liftFrontRight.write(110);
  delay(50);
  pivotFrontRight.write(90);

  liftBackRight.write(high);
  delay(50);
  pivotBackRight.write(120);
  delay(walk_speed);
  liftBackRight.write(110);
  delay(50);
  pivotBackRight.write(90);


  liftBackLeft.write(high);
  delay(50);
  pivotBackLeft.write(120);
  delay(walk_speed);
  liftBackLeft.write(110);
  delay(50);
  pivotBackLeft.write(90);
}

void TURN_RIGHT_SEQUENCE() {
  // Right back moves backward
  liftFrontRight.write(high);
  delay(50);
  pivotFrontRight.write(60);
  delay(walk_speed);
  liftFrontRight.write(110);
  delay(50);
  pivotFrontRight.write(90);

  // Left front moves forward
  liftFrontLeft.write(high);
  delay(50);
  pivotFrontLeft.write(60);
  delay(walk_speed);
  liftFrontLeft.write(110);
  delay(50);
  pivotFrontLeft.write(90);

  liftBackLeft.write(high);
  delay(50);
  pivotBackLeft.write(60);
  delay(walk_speed);
  liftBackLeft.write(110);
  delay(50);
  pivotBackLeft.write(90);

  liftBackRight.write(high);
  delay(50);
  pivotBackRight.write(60);
  delay(walk_speed);
  liftBackRight.write(110);
  delay(50);
  pivotBackRight.write(90);
}

void UP_DOWN() {
  // ยก lift ของทุกเซอร์โวขึ้น
  liftFrontLeft.write(high);
  liftFrontRight.write(high);
  liftBackLeft.write(high);
  liftBackRight.write(high);

  delay(500);  // รอให้ servo ยกขึ้น

  // ลด lift ของทุกเซอร์โวลง
  liftFrontLeft.write(110);
  liftFrontRight.write(110);
  liftBackLeft.write(110);
  liftBackRight.write(110);

  delay(500);  // รอให้ servo ลดลง
}

void WALK_RIGHT_SEQUENCE() {
  // ขาหน้าขวา
  liftFrontRight.write(high);
  delay(50);
  pivotFrontRight.write(40);
  delay(walk_speed);
  liftFrontRight.write(110);
  delay(50);
  pivotFrontRight.write(90);

  // ขาหลังซ้าย
  liftBackLeft.write(high);
  delay(50);
  pivotBackLeft.write(140);  // เลี้ยวไปทางขวา
  delay(walk_speed);
  liftBackLeft.write(110);
  delay(50);
  pivotBackLeft.write(90);

  // ขาหลังขวา
  liftBackRight.write(high);
  delay(50);
  pivotBackRight.write(140);  // ขาหลังขวาจะเดินไปข้างหน้า
  delay(walk_speed);
  liftBackRight.write(110);
  delay(50);
  pivotBackRight.write(90);

  // ขาหน้าซ้าย
  liftFrontLeft.write(high);
  delay(50);
  pivotFrontLeft.write(40);  // ขาหน้าซ้ายจะเดินไปข้างหน้า
  delay(walk_speed);
  liftFrontLeft.write(110);
  delay(50);
  pivotFrontLeft.write(90);
}

void WALK_LEFT_SEQUENCE() {

  // ขาหลังซ้าย
  liftBackLeft.write(high);
  delay(50);
  pivotBackLeft.write(40);
  delay(walk_speed);
  liftBackLeft.write(110);
  delay(50);
  pivotBackLeft.write(90);

  // ขาหน้าขวา
  liftFrontRight.write(high);
  delay(50);
  pivotFrontRight.write(140);
  delay(walk_speed);
  liftFrontRight.write(110);
  delay(50);
  pivotFrontRight.write(90);

  // ขาหน้าซ้าย
  liftFrontLeft.write(high);
  delay(50);
  pivotFrontLeft.write(140);  // ขาหน้าซ้ายเดินไปทางซ้าย
  delay(walk_speed);
  liftFrontLeft.write(110);
  delay(50);
  pivotFrontLeft.write(90);

  // ขาหลังขวา
  liftBackRight.write(high);
  delay(50);
  pivotBackRight.write(40);  // ขาหลังขวาเดินไปทางซ้าย
  delay(walk_speed);
  liftBackRight.write(110);
  delay(50);
  pivotBackRight.write(90);
}

void SIT_UP() {
  // for (int i = 0; i < 3; i++) {  // ทำงาน 3 รอบ
    // Move legs in a pattern to turn clockwise
    liftFrontRight.write(high);
    liftBackLeft.write(high);
    delay(150);
    pivotFrontRight.write(140);  // Move front-right leg backward
    pivotBackLeft.write(140);    // Move back-left leg forward
    delay(walk_speed);
    liftFrontRight.write(110);
    liftBackLeft.write(110);
    delay(150);
    pivotFrontRight.write(90);
    pivotBackLeft.write(90);
    delay(150);

    liftFrontLeft.write(high);
    liftBackRight.write(high);
    delay(150);
    pivotFrontLeft.write(140);  // Move front-left leg forward
    pivotBackRight.write(140);  // Move back-right leg backward
    delay(walk_speed);
    liftFrontLeft.write(110);
    liftBackRight.write(110);
    delay(150);
    pivotFrontLeft.write(90);
    pivotBackRight.write(90);
    delay(150);
  // }
}

void HELLO_RIGHT() {
  liftBackLeft.write(130);
  liftBackRight.write(130);
  liftFrontLeft.write(30);
  liftFrontRight.write(30);
  delay(300);
  liftFrontRight.write(110);
  pivotFrontRight.write(120);
  delay(100);
  pivotFrontRight.write(90);
  delay(100);
  // โยกมือไปกลับ 3 รอบ
  for (int i = 0; i < 3; i++) {
    pivotFrontRight.write(120);  // โยกมือไป
    delay(100);                  // หน่วงเวลา 500 มิลลิวินาที
    pivotFrontRight.write(90);   // โยกมือกลับ
    delay(100);                  // หน่วงเวลา 500 มิลลิวินาที
  }
  delay(100);
}


void HELLO_LEFT() {
  liftBackLeft.write(130);
  liftBackRight.write(130);
  liftFrontLeft.write(30);
  liftFrontRight.write(30);
  delay(300);
  liftFrontLeft.write(110);
  pivotFrontLeft.write(60);
  delay(100);
  pivotFrontLeft.write(90);
  delay(100);
  // โยกมือไปกลับ 3 รอบ
  for (int i = 0; i < 3; i++) {
    pivotFrontLeft.write(60);  // โยกมือไป
    delay(100);                // หน่วงเวลา 500 มิลลิวินาที
    pivotFrontLeft.write(90);  // โยกมือกลับ
    delay(100);                // หน่วงเวลา 500 มิลลิวินาที
  }
  delay(100);
}

void setup() {
  Serial.begin(57600);
  error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
  if (error == 0) Serial.println("Found Controller, configured successfully");
  else Serial.println("Error configuring controller");

  type = ps2x.readType();
  if (type == 1) Serial.println("DualShock Controller Found");

  // Attach servos
  pivotFrontLeft.attach(2);
  liftFrontLeft.attach(3);
  pivotFrontRight.attach(4);
  liftFrontRight.attach(5);
  pivotBackLeft.attach(6);
  liftBackLeft.attach(7);
  pivotBackRight.attach(8);
  liftBackRight.attach(9);

  // Center all servos
  center_servos();
}


void loop() {
  if (error == 1) return;

  ps2x.read_gamepad(false, vibrate);  // อ่านค่าปุ่ม

  // ตรวจสอบการกดปุ่มขึ้น
  bool upState = ps2x.Button(PSB_PAD_UP);
  if (upState && !isWalking) {  // หากปุ่มถูกกดและยังไม่เคลื่อนที่
    Serial.println("Up pressed!");
    WALK_SEQUENCE();   // เรียกฟังก์ชันเดินไปข้างหน้า
    isWalking = true;  // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!upState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  // ตรวจสอบการกดปุ่มลง
  bool downState = ps2x.Button(PSB_PAD_DOWN);
  if (downState && !isWalking) {  // หากปุ่มถูกกดและยังไม่เคลื่อนที่
    Serial.println("Down pressed!");
    WALKBACK_SEQUENCE();  // เรียกฟังก์ชันเดินถอยหลัง
    isWalking = true;     // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!downState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  // ตรวจสอบการกดปุ่มเลี้ยวซ้าย
  bool tleftState = ps2x.Button(PSB_L2);
  if (tleftState && !isWalking) {  // หากปุ่มถูกกดและยังไม่เคลื่อนที่
    Serial.println("Left pressed!");
    TURN_LEFT_SEQUENCE();  // เรียกฟังก์ชันเลี้ยวซ้าย
    isWalking = true;      // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!tleftState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  // ตรวจสอบการกดปุ่มเลี้ยวขวา
  bool trightState = ps2x.Button(PSB_R2);
  if (trightState && !isWalking) {  // หากปุ่มถูกกดและยังไม่เคลื่อนที่
    Serial.println("Right pressed!");
    TURN_RIGHT_SEQUENCE();  // เรียกฟังก์ชันเลี้ยวขวา
    isWalking = true;       // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!trightState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  // ตรวจสอบการกดปุ่มเขียว (Triangle)
  bool greenState = ps2x.Button(PSB_GREEN);
  if (greenState && !isWalking) {  // หากปุ่มถูกกดและยังไม่เคลื่อนที่
                                   // if (greenState) {  // หากปุ่มถูกกด
    Serial.println("Triangle pressed");
    UP_DOWN();         // เรียกฟังก์ชันยกและลด
    isWalking = true;  // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!greenState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  // ตรวจสอบการกดปุ่มแดง (Circle)
  bool redState = ps2x.Button(PSB_RED);  // เพิ่มการตรวจสอบปุ่มแดง
  if (redState && !isWalking) {          // หากปุ่มถูกกดและยังไม่เคลื่อนที่
                                         // if (redState) {
    Serial.println("Red Circle pressed");
    SIT_UP();          // เรียกฟังก์ชัน SIT_UP
    isWalking = true;  // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!redState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  bool rs1State = ps2x.Button(PSB_R1);  // เพิ่มการตรวจสอบปุ่มแดง
  if (rs1State && !isWalking) {         // หากปุ่มถูกกดและยังไม่เคลื่อนที่
                                        // if (redState) {
    Serial.println("L1 pressed");
    HELLO_RIGHT();     // เรียกฟังก์ชัน SIT_UP
    isWalking = true;  // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!rs1State && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  bool l1State = ps2x.Button(PSB_L1);  // เพิ่มการตรวจสอบปุ่มแดง
  if (l1State && !isWalking) {         // หากปุ่มถูกกดและยังไม่เคลื่อนที่
                                       // if (redState) {
    Serial.println("L1 pressed");
    HELLO_LEFT();      // เรียกฟังก์ชัน SIT_UP
    isWalking = true;  // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!l1State && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  // ตรวจสอบการกดปุ่มขวา
  bool rightState = ps2x.Button(PSB_PAD_RIGHT);
  if (rightState && !isWalking) {  // หากปุ่มถูกกดและยังไม่เคลื่อนที่
    Serial.println("Right pressed!");
    WALK_RIGHT_SEQUENCE();  // เรียกฟังก์ชันเดินไปทางขวา
    isWalking = true;       // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!rightState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }

  // ตรวจสอบการกดปุ่มซ้าย
  bool leftState = ps2x.Button(PSB_PAD_LEFT);
  if (leftState && !isWalking) {  // หากปุ่มถูกกดและยังไม่เคลื่อนที่
    Serial.println("Left pressed!");
    WALK_LEFT_SEQUENCE();  // เรียกฟังก์ชันเดินไปทางซ้าย
    isWalking = true;      // ตั้งสถานะว่าเริ่มเคลื่อนที่
  } else if (!leftState && isWalking) {
    // หากปุ่มไม่ถูกกดและยังเคลื่อนที่อยู่
    isWalking = false;  // หยุดการเคลื่อนที่
    center_servos();
  }
  // Adjust walking speed with button X
  // ตรวจจับการกดปุ่ม X เพื่อเพิ่มความเร็ว
  if (ps2x.ButtonPressed(PSB_BLUE)) {        // ปุ่ม X
    if (!x_pressed) {                        // หากยังไม่ได้กดปุ่ม X
      walk_speed -= 50;                      // ลดค่า walk_speed
      if (walk_speed < 50) walk_speed = 50;  // จำกัดค่าต่ำสุด
      x_pressed = true;                      // เปลี่ยนสถานะว่ากด X แล้ว
      Serial.print("Increased speed, new walk speed: ");
      Serial.println(walk_speed);
    }
  }

  // ตรวจจับการกดปุ่มสี่เหลี่ยมเพื่อรีเซ็ตสถานะ
  if (ps2x.ButtonPressed(PSB_SQUARE)) {  // ปุ่มสี่เหลี่ยม
    walk_speed = 150;                    // คืนค่า walk_speed เป็นค่าปกติ
    x_pressed = false;                   // รีเซ็ตสถานะการกดปุ่ม X
    Serial.println("Reset speed to default, walk speed: 150");
  }

  // รีเซ็ตสถานะเมื่อไม่ได้กดปุ่ม X
  if (!ps2x.Button(PSB_BLUE)) {  // หากไม่ได้กด X ค้าง
    x_pressed = false;           // อนุญาตให้กด X ใหม่ได้
  }

  delay(50);  // หน่วงเวลาเพื่อลดการโหลด
}
