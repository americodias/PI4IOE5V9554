#include <PI4IOE5V9554.h>

PI4IOE5V9554 ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(PI4IOE5V95XX::Polarity::ORIGINAL_ALL);
    ioex.direction(PI4IOE5V95XX::Direction::OUT_ALL);
    ioex.write(PI4IOE5V95XX::Level::L_ALL);
}

void loop() {
    for (uint8_t i = 0; i < 8; ++i) {
        Serial.print("set port high: ");
        Serial.println(i);

        ioex.write(i, PI4IOE5V95XX::Level::H);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }

    for (uint8_t i = 0; i < 8; ++i) {
        Serial.print("set port low: ");
        Serial.println(i);

        ioex.write(i, PI4IOE5V95XX::Level::L);
        Serial.println(ioex.read(), BIN);
        delay(500);
    }
}
