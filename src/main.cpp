#include <Arduino.h>
#include <DataPacket.hh>
#include <PS4Controller.h>

constexpr unsigned long BAUD_RATE = 115200;    // 115.2kbps
constexpr uint32_t SERIAL_CONFIG = SERIAL_8N1; // Data:8 Parity:None Stop:1
constexpr int8_t SERIAL_PIN_RX = 32;
constexpr int8_t SERIAL_PIN_TX = 33;

void setup() {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_BT);
    char mac_str[18];
    snprintf(mac_str, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    PS4.begin(mac_str);
    Serial1.begin(BAUD_RATE, SERIAL_CONFIG, SERIAL_PIN_RX, SERIAL_PIN_TX);
}

void loop() {
    if(PS4.isConnected()) {
        PS4.setLed(0x00, 0x00, 0xff);
        DataPacket data;
        data.set_bit(DataPacket::TOUCHPAD, PS4.Touchpad());
        data.set_bit(DataPacket::PS_LOGO, PS4.PSButton());
        data.set_bit(DataPacket::SHARE, PS4.Share());
        data.set_bit(DataPacket::OPTIONS, PS4.Options());
        data.set_bit(DataPacket::L3, PS4.L3());
        data.set_bit(DataPacket::R3, PS4.R3());
        data.set_bit(DataPacket::L1, PS4.L1());
        data.set_bit(DataPacket::R1, PS4.R1());
        data.set_bit(DataPacket::DOWN, PS4.Down());
        data.set_bit(DataPacket::LEFT, PS4.Left());
        data.set_bit(DataPacket::UP, PS4.Up());
        data.set_bit(DataPacket::RIGHT, PS4.Right());
        data.set_bit(DataPacket::CROSS, PS4.Cross());
        data.set_bit(DataPacket::SQUARE, PS4.Square());
        data.set_bit(DataPacket::TRIANGLE, PS4.Triangle());
        data.set_bit(DataPacket::CIRCLE, PS4.Circle());
        data.set_sticks(PS4.RStickX(), PS4.RStickY(), PS4.LStickX(), PS4.LStickY());
        data.set_triggers(PS4.R2Value(), PS4.L2Value());
        data.compute_checksum();
        Serial1.write(data.get_raw_data().data(), DataPacket::LENGTH);
    }
    delay(100);
}
