#include <PI4IOE5V9554.h>

PI4IOE5V9554 ioex;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire.begin();
    ioex.attach(Wire);
    ioex.polarity(PI4IOE5V95XX::Polarity::ORIGINAL_ALL);
    ioex.direction(PI4IOE5V95XX::Direction::IN_ALL);
}

void loop() {
    Serial.println(ioex.read(), BIN);
    delay(1000);
}
