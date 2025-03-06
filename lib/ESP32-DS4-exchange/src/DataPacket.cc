#include <DataPacket.hh>
#include <algorithm>

DataPacket::DataPacket() {
    this->_raw.fill(0);
    this->_raw.at(BYTE_HEADER) = HEADER;
    this->_raw.at(BYTE_CHECKSUM) = get_checksum(_raw);
}

DataPacket::~DataPacket() = default;

void DataPacket::set_bit(const std::uint16_t type, const bool bit) {
    if (0xff < type) {
        bit ? this->_raw.at(BYTE_BTN_HIGH) |= type >> 8
            : this->_raw.at(BYTE_BTN_HIGH) &= ~(type >> 8);
    } else {
        bit ? this->_raw.at(BYTE_BTN_LOW) |= type : this->_raw.at(BYTE_BTN_LOW) &= ~type;
    }
}

void DataPacket::set_sticks(
    const std::int8_t rx,
    const std::int8_t ry,
    const std::int8_t lx,
    const std::int8_t ly
) {
    this->_raw.at(BYTE_STICK_RX) = rx;
    this->_raw.at(BYTE_STICK_RY) = ry;
    this->_raw.at(BYTE_STICK_LX) = lx;
    this->_raw.at(BYTE_STICK_LY) = ly;
}

void DataPacket::set_triggers(const std::uint8_t r2, const std::uint8_t l2) {
    this->_raw.at(BYTE_R2) = r2;
    this->_raw.at(BYTE_L2) = l2;
}

std::uint8_t DataPacket::get_checksum(const std::array<std::uint8_t, 10>& raw) {
    std::uint8_t checksum = 0;
    std::for_each(raw.begin(), raw.end() - 1, [&](const std::uint8_t e) { checksum += e; });
    return checksum;
}

std::array<std::uint8_t, DataPacket::LENGTH>& DataPacket::get_raw_data() {
    return this->_raw;
}

void DataPacket::compute_checksum() {
    this->_raw.at(BYTE_CHECKSUM) = get_checksum(this->_raw);
}

std::string DataPacket::to_string() const {
    char str[41];
    snprintf(
        str,
        41,
        "[%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x]",
        this->_raw.at(0),
        this->_raw.at(1),
        this->_raw.at(2),
        this->_raw.at(3),
        this->_raw.at(4),
        this->_raw.at(5),
        this->_raw.at(6),
        this->_raw.at(7),
        this->_raw.at(8),
        this->_raw.at(9)
    );
    return str;
}
