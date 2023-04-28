# PI4IOE5V9554

Arduino library for [PI4IOE5V9554](https://www.diodes.com/part/view/PI4IOE5V9554) (8-bit general-purpose I/O expander that provides remote I/O expansion for most microcontroller families via the I2C-bus interface).

This library is forked from ponoor's [PI4IOE5V6416](https://github.com/ponoor/PI4IOE5V6416) library.

To Do:
- Read interrupt status

## Usage

You can specify a port using either index (`0` - `7`) or enum (`PI4IOE5V95XX::Port::P02`, etc.).

### Input

```C++
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
```

### Output

```C++
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

```

## APIs

```C++
void attach(WireType& wire, uint8_t i2c_addr = BASE_I2C_ADDR);
uint8_t read();
Level::Level read(const Port::Port port);
bool write(const uint8_t value);
bool write(const Port::Port port, const Level::Level level);
bool write(const uint8_t port, const Level::Level level);
bool polarity(const uint8_t value);
bool polarity(const Port::Port port, const Polarity::Polarity pol);
bool direction(const uint8_t value);
bool direction(const Port::Port port, const Direction::Direction dir);
uint8_t i2c_error();
```

```C++
namespace Port {
    enum Port : uint8_t {
        P00,
        P01,
        P02,
        P03,
        P04,
        P05,
        P06,
        P07
    };
}
namespace Level {
    enum Level : uint8_t { L, H };
    enum LevelAll : uint8_t { L_ALL = 0x00, H_ALL = 0xFF };
}
namespace Polarity {
    enum Polarity : uint8_t { ORIGINAL, INVERTED };
    enum PolarityAll : uint8_t { ORIGINAL_ALL = 0x00, INVERTED_ALL = 0xFF };
}
namespace Direction {
    enum Direction : uint8_t { OUT, IN };
    enum DirectionAll : uint8_t { OUT_ALL = 0x00, IN_ALL = 0xFF };
}
```

## License

MIT
