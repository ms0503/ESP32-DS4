#include <DataPacket.hh>
#include <algorithm>

DataPacket::DataPacket() {
    _raw.fill(0);
    _raw.at(BYTE_HEADER) = HEADER;
    _raw.at(BYTE_CHECKSUM) = get_checksum(_raw);
}

DataPacket::~DataPacket() = default;

void DataPacket::set_bit(const uint16_t type, const bool bit) {
    if(0xff < type) {
        bit ? _raw.at(BYTE_BTN_HIGH) |= type >> 8 : _raw.at(BYTE_BTN_HIGH) &= ~(type >> 8);
    } else {
        bit ? _raw.at(BYTE_BTN_LOW) |= type : _raw.at(BYTE_BTN_LOW) &= ~type;
    }
}

void DataPacket::set_sticks(const int8_t rx, const int8_t ry, const int8_t lx, const int8_t ly) {
    _raw.at(BYTE_STICK_RX) = rx;
    _raw.at(BYTE_STICK_RY) = ry;
    _raw.at(BYTE_STICK_LX) = lx;
    _raw.at(BYTE_STICK_LY) = ly;
}

void DataPacket::set_triggers(const uint8_t r2, const uint8_t l2) {
    _raw.at(BYTE_R2) = r2;
    _raw.at(BYTE_L2) = l2;
}

std::uint8_t DataPacket::get_checksum(const std::array<std::uint8_t, 10> raw) {
    std::uint8_t checksum = 0;
    std::for_each(raw.begin(), raw.end() - 1, [&](uint8_t e) {
        checksum += e;
    });
    return checksum;
}

std::array<std::uint8_t, DataPacket::LENGTH> &DataPacket::get_raw_data() {
    return _raw;
}

void DataPacket::compute_checksum() {
    _raw.at(BYTE_CHECKSUM) = get_checksum(_raw);
}
