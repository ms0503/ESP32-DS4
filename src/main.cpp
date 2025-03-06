#include <Arduino.h>
#include <DataPacket.hh>
#include <PS4Controller.h>
#include <Ps3Controller.h>
#include <esp_mac.h>

constexpr unsigned long UART_BAUD_RATE = 115200; // 115.2kbps
constexpr uint32_t UART_CONFIG = SERIAL_8N1; // Data: 8, Parity: None, Stop: 1
constexpr int8_t UART_PIN_RX = 32;
constexpr int8_t UART_PIN_TX = 33;
constexpr uint8_t DS3_PIN_1 = 19;
constexpr uint8_t DS3_PIN_2 = 21;

auto isDS3 = false;

void setup() {
    Serial.begin(115200);
    pinMode(DS3_PIN_1, OUTPUT);
    pinMode(DS3_PIN_2, INPUT_PULLUP);
    digitalWrite(DS3_PIN_1, LOW);
    isDS3 = digitalRead(DS3_PIN_2) == LOW;
    Serial1.begin(UART_BAUD_RATE, UART_CONFIG, UART_PIN_RX, UART_PIN_TX);
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_BT);
    char mac_str[18];
    snprintf(
        mac_str,
        18,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],
        mac[1],
        mac[2],
        mac[3],
        mac[4],
        mac[5]
    );
    if (isDS3) {
        Serial.println("Start with DualShock3 mode.");
        Ps3.begin(mac_str);
    } else {
        Serial.println("Start with DualShock4 mode.");
        PS4.begin(mac_str);
    }
}

void loop() {
    if ((isDS3 && Ps3.isConnected()) || (!isDS3 && PS4.isConnected())) {
        if (isDS3) {
            Ps3.setPlayer(1);
        } else {
            PS4.setLed(0x00, 0x00, 0xff);
            PS4.sendToController();
        }
        DataPacket data;
        data.set_bit(DataPacket::TOUCHPAD, isDS3 ? false : PS4.Touchpad());
        data.set_bit(DataPacket::PS_LOGO, isDS3 ? Ps3.data.button.ps : PS4.PSButton());
        data.set_bit(DataPacket::SHARE, isDS3 ? Ps3.data.button.select : PS4.Share());
        data.set_bit(DataPacket::OPTIONS, isDS3 ? Ps3.data.button.start : PS4.Options());
        data.set_bit(DataPacket::L3, isDS3 ? Ps3.data.button.l3 : PS4.L3());
        data.set_bit(DataPacket::R3, isDS3 ? Ps3.data.button.r3 : PS4.R3());
        data.set_bit(DataPacket::L1, isDS3 ? Ps3.data.button.l1 : PS4.L1());
        data.set_bit(DataPacket::R1, isDS3 ? Ps3.data.button.r1 : PS4.R1());
        data.set_bit(DataPacket::DOWN, isDS3 ? Ps3.data.button.down : PS4.Down());
        data.set_bit(DataPacket::LEFT, isDS3 ? Ps3.data.button.left : PS4.Left());
        data.set_bit(DataPacket::UP, isDS3 ? Ps3.data.button.up : PS4.Up());
        data.set_bit(DataPacket::RIGHT, isDS3 ? Ps3.data.button.right : PS4.Right());
        data.set_bit(DataPacket::CROSS, isDS3 ? Ps3.data.button.cross : PS4.Cross());
        data.set_bit(DataPacket::SQUARE, isDS3 ? Ps3.data.button.square : PS4.Square());
        data.set_bit(DataPacket::TRIANGLE, isDS3 ? Ps3.data.button.triangle : PS4.Triangle());
        data.set_bit(DataPacket::CIRCLE, isDS3 ? Ps3.data.button.circle : PS4.Circle());
        data.set_sticks(
            isDS3 ? Ps3.data.analog.stick.rx : PS4.RStickX(),
            isDS3 ? Ps3.data.analog.stick.ry : PS4.RStickY(),
            isDS3 ? Ps3.data.analog.stick.lx : PS4.LStickX(),
            isDS3 ? Ps3.data.analog.stick.ly : PS4.LStickY()
        );
        data.set_triggers(
            isDS3 ? Ps3.data.analog.button.r2 : PS4.R2Value(),
            isDS3 ? Ps3.data.analog.button.l2 : PS4.L2Value()
        );
        data.compute_checksum();
        Serial1.write(data.get_raw_data().data(), DataPacket::LENGTH);
        Serial.printf(
            "Send data: [%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, "
            "%02x, %02x]",
            data.get_raw_data().at(0),
            data.get_raw_data().at(1),
            data.get_raw_data().at(2),
            data.get_raw_data().at(3),
            data.get_raw_data().at(4),
            data.get_raw_data().at(5),
            data.get_raw_data().at(6),
            data.get_raw_data().at(7),
            data.get_raw_data().at(8),
            data.get_raw_data().at(9)
        );
    }
    delay(100);
}

// vim: ft=arduino
