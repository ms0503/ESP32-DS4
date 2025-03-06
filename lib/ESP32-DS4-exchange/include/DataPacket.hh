#pragma once

#include <array>
#include <cstdint>

/*
 * Controller data packet.
 *
 * | byte |   0    |    1     |    2    |    3     |    4     |    5     |    6     | 7  | 8  |    9     |
 * | name | HEADER | BTN_HIGH | BTN_LOW | STICK_RX | STICK_RY | STICK_LX | STICK_LY | R2 | L2 | CHECKSUM |
 */
class DataPacket {
public:
    static constexpr std::uint8_t HEADER = 0x80;
    static constexpr std::uint16_t CIRCLE = 0b0000000000000001;
    static constexpr std::uint16_t TRIANGLE = 0b0000000000000010;
    static constexpr std::uint16_t SQUARE = 0b0000000000000100;
    static constexpr std::uint16_t CROSS = 0b0000000000001000;
    static constexpr std::uint16_t RIGHT = 0b0000000000010000;
    static constexpr std::uint16_t UP = 0b0000000000100000;
    static constexpr std::uint16_t LEFT = 0b0000000001000000;
    static constexpr std::uint16_t DOWN = 0b0000000010000000;
    static constexpr std::uint16_t R1 = 0b0000000100000000;
    static constexpr std::uint16_t L1 = 0b0000001000000000;
    static constexpr std::uint16_t R3 = 0b0000010000000000;
    static constexpr std::uint16_t L3 = 0b0000100000000000;
    static constexpr std::uint16_t OPTIONS = 0b0001000000000000;
    static constexpr std::uint16_t SHARE = 0b0010000000000000;
    static constexpr std::uint16_t PS_LOGO = 0b0100000000000000;
    static constexpr std::uint16_t TOUCHPAD = 0b1000000000000000;

    static constexpr std::size_t BYTE_HEADER = 0;
    static constexpr std::size_t BYTE_BTN_HIGH = 1;
    static constexpr std::size_t BYTE_BTN_LOW = 2;
    static constexpr std::size_t BYTE_STICK_RX = 3;
    static constexpr std::size_t BYTE_STICK_RY = 4;
    static constexpr std::size_t BYTE_STICK_LX = 5;
    static constexpr std::size_t BYTE_STICK_LY = 6;
    static constexpr std::size_t BYTE_R2 = 7;
    static constexpr std::size_t BYTE_L2 = 8;
    static constexpr std::size_t BYTE_CHECKSUM = 9;

    static constexpr std::size_t LENGTH = 10;

    explicit DataPacket();

    ~DataPacket();

    void set_bit(std::uint16_t type, bool bit);

    void set_sticks(std::int8_t rx, std::int8_t ry, std::int8_t lx, std::int8_t ly);

    void set_triggers(std::uint8_t r2, std::uint8_t l2);

    std::array<std::uint8_t, LENGTH>& get_raw_data();

    void compute_checksum();

    std::string to_string() const;

private:
    std::array<std::uint8_t, LENGTH> _raw {};

    static std::uint8_t get_checksum(const std::array<std::uint8_t, 10>& raw);
};
