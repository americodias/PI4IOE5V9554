#include <Arduino.h>
#include <Wire.h>

namespace PI4IOE5V95XX {


namespace Reg {
    enum : uint8_t {
        INPUT_PORT_0 = 0x00,
        OUTPUT_PORT_0,
        POLARITY_INVERSION_PORT_0,
        CONFIGURATION_PORT_0,
    };
}

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
}  // namespace Port

namespace Level {
    enum Level : uint8_t { L, H };
    enum LevelAll : uint8_t { L_ALL = 0x00, H_ALL = 0xFF };
}  // namespace Level

namespace Polarity {
    enum Polarity : uint8_t { ORIGINAL, INVERTED };
    enum PolarityAll : uint8_t { ORIGINAL_ALL = 0x00, INVERTED_ALL = 0xFF };
}  // namespace Polarity

namespace Direction {
    enum Direction : uint8_t { OUT, IN };
    enum DirectionAll : uint8_t { OUT_ALL = 0x00, IN_ALL = 0xFF };
}  // namespace Direction

template <typename WireType = TwoWire>
class PI4IOE5V95XX {
    union Ports {
        uint8_t w;
        uint8_t b[1];
    };

    static constexpr uint8_t BASE_I2C_ADDR = 0x20;

    WireType* wire {nullptr};
    uint8_t addr {BASE_I2C_ADDR};
    Ports input {0x00};
    Ports output {0xFF};
    Ports pol {0x00};
    Ports dir {0xFF};
    uint8_t status {0x00};

public:
    void attach(WireType& wire, uint8_t i2c_addr = BASE_I2C_ADDR) {
        this->wire = &wire;
        this->addr = i2c_addr;
    }

    uint8_t read() {
        read_bytes(this->addr, Reg::INPUT_PORT_0, this->input.b, 1);
        return this->input.w;
    }
    Level::Level read(const Port::Port port) {
        uint8_t v = read();
        return (v & (1 << port)) ? Level::H : Level::L;
    }

    bool write(const uint8_t value) {
        this->output.w = value;
        return write_impl();
    }

    bool write(const Port::Port port, const Level::Level level) {
        if (level == Level::H) {
            this->output.w |= (1 << port);
        } else {
            this->output.w &= ~(1 << port);
        }
        return write_impl();
    }

    bool write(const uint8_t port, const Level::Level level) {
        if (level == Level::H) {
            this->output.w |= (1 << port);
        } else {
            this->output.w &= ~(1 << port);
        }
        return write_impl();
    }

    bool polarity(const uint8_t value) {
        this->pol.w = value;
        return polarity_impl();
    }
    bool polarity(const Port::Port port, const Polarity::Polarity pol) {
        if (pol == Polarity::INVERTED) {
            this->pol.w |= (1 << port);
        } else {
            this->pol.w &= ~(1 << port);
        }
        return polarity_impl();
    }

    bool direction(const uint8_t value) {
        this->dir.w = value;
        return direction_impl();
    }

    bool direction(const Port::Port port, const Direction::Direction dir) {
        if (dir == Direction::IN) {
            this->dir.w |= (1 << port);
        } else {
            this->dir.w &= ~(1 << port);
        }
        return direction_impl();
    }

    uint8_t i2c_error() const {
        return status;
    }

private:
    bool write_impl() {
        return write_bytes(this->addr, Reg::OUTPUT_PORT_0, this->output.b, 1);
    }

    bool polarity_impl() {
        return write_bytes(this->addr, Reg::POLARITY_INVERSION_PORT_0, this->pol.b, 1);
    }

    bool direction_impl() {
        return write_bytes(this->addr, Reg::CONFIGURATION_PORT_0, this->dir.b, 2);
    }

    int8_t read_bytes(const uint8_t dev, const uint8_t reg, uint8_t* data, const uint8_t size) {
        wire->beginTransmission(dev);
        wire->write(reg);
        wire->endTransmission(true);
        wire->requestFrom(dev, size);
        int8_t count = 0;
        while (wire->available())
        {
            data[count++] = wire->read();
            if(count >= size) break;
        }
        wire->endTransmission(true);
        return count;
    }

    bool write_bytes(const uint8_t dev, const uint8_t reg, const uint8_t* data, const uint8_t size) {
        wire->beginTransmission(dev);
        wire->write(reg);
        for (uint8_t i = 0; i < size; i++) wire->write(data[i]);
        status = wire->endTransmission(true);
        return (status == 0);
    }
};

}  // namespace PI4IOE5V95XX

// using PI4IOE5V6408 = PI4IOE5V95XX::PI4IOE5V95XX<>;
using PI4IOE5V9554 = PI4IOE5V95XX::PI4IOE5V95XX<>;
