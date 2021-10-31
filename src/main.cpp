#include <Arduino.h>

#define ScooterSerial Serial
#define RX_DISABLE UCSR0B &= ~_BV(RXEN0);
#define RX_ENABLE  UCSR0B |=  _BV(RXEN0);
int speed = 30;

void setup() {
  RX_DISABLE;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  ScooterSerial.begin(115200);
}

uint16_t calculateChecksum(uint8_t *data)
{
  uint8_t len = data[0] + 2;
  uint16_t sum = 0;
  for (int i = 0; i < len; i++)
    sum += data[i];

  sum ^= 0xFFFF;
  return sum;
}

void setSpeed(const int speed) { //Setze Maximalgeschwindigkeit in km/h
  uint8_t data[] = {
    0x55, 0xAA, 0x04, 0x22, 0x01, 0xF2,
    0, 0, //rpm
    0, 0, //checksum
  };

  *(uint16_t *)&data[6] = (speed * 252) / 10;
  *(uint16_t *)&data[8] = calculateChecksum(data + 2);

  for (int i=0; i<= 4; i++) {
    if (i != 0) {
      delay(150);
    }
    ScooterSerial.write(data, sizeof(data) / sizeof(uint8_t));
  }

}

void loop() {
  setSpeed(speed);
  delay(150);
}