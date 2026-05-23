#pragma once
#include "esphome.h"

#define PIN_CLK_ET  6
#define PIN_DAT_ET  7

volatile uint8_t et_digits[4] = {0, 0, 0, 0};
volatile bool et_dp[4] = {false, false, false, false};
volatile bool et_new_data = false;

volatile bool isr_active = false;
volatile uint8_t isr_bits = 0;
volatile uint8_t isr_byte = 0;
volatile uint8_t isr_cmd = 0;
volatile bool isr_lclk = true;
volatile bool isr_ldat = true;

int decode_seg(uint8_t b) {
    b &= 0x7F;
    switch (b) {
        case 0x3F: return 0; case 0x06: return 1; case 0x5B: return 2;
        case 0x4F: return 3; case 0x66: return 4; case 0x6D: return 5;
        case 0x7D: return 6; case 0x07: return 7; case 0x7F: return 8;
        case 0x6F: return 9; default: return -1;
    }
}

void IRAM_ATTR et_isr() {
    bool clk = digitalRead(PIN_CLK_ET);
    bool dat = digitalRead(PIN_DAT_ET);
    if (isr_lclk && clk && isr_ldat && !dat) { isr_active = true; isr_bits = 0; isr_byte = 0; goto end_isr; }
    if (isr_lclk && clk && !isr_ldat && dat) { isr_active = false; goto end_isr; }
    if (isr_active && !isr_lclk && clk) {
        isr_byte = (isr_byte << 1) | (dat ? 1 : 0);
        isr_bits++;
        if (isr_bits == 8) {
            if (isr_cmd == 0) { isr_cmd = isr_byte; }
            else {
                int pos = -1;
                if (isr_cmd == 0x68) pos = 0;
                else if (isr_cmd == 0x6A) pos = 1;
                else if (isr_cmd == 0x6C) pos = 2;
                else if (isr_cmd == 0x6E) pos = 3;
                if (pos >= 0) {
                    et_digits[pos] = isr_byte & 0x7F;
                    et_dp[pos] = (isr_byte & 0x80) != 0;
                    if (pos == 3) et_new_data = true;
                }
                isr_cmd = 0;
            }
        } else if (isr_bits == 9) { isr_bits = 0; isr_byte = 0; }
    }
    end_isr:
    isr_lclk = clk;
    isr_ldat = dat;
}

class DeskController : public PollingComponent {
public:
    esphome::sensor::Sensor *height_sensor;

    DeskController() : PollingComponent(50) {
        height_sensor = new esphome::sensor::Sensor();
    }

    void setup() override {
        pinMode(PIN_CLK_ET, INPUT_PULLUP);
        pinMode(PIN_DAT_ET, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(PIN_CLK_ET), et_isr, CHANGE);
        attachInterrupt(digitalPinToInterrupt(PIN_DAT_ET), et_isr, CHANGE);
    }

    void update() override {
        if (et_new_data) {
            noInterrupts();
            uint8_t temp_digits[4];
            bool temp_dp[4];
            for (int i = 0; i < 4; i++) {
                temp_digits[i] = et_digits[i];
                temp_dp[i] = et_dp[i];
            }
            et_new_data = false;
            interrupts();

            int d[4];
            for (int i = 0; i < 4; i++) d[i] = decode_seg(temp_digits[i]);

            float h = -1;
            if (d[1] >= 0 && d[2] >= 0 && d[3] >= 0) {
                if (temp_dp[2]) h = d[1] * 10.0 + d[2] + d[3] * 0.1;
                else            h = d[1] * 100.0 + d[2] * 10.0 + d[3];
            }
            
            if (h >= 50.0 && h <= 140.0) {
                height_sensor->publish_state(h);
            }
        }
    }
};

inline DeskController* get_desk_controller() {
    static DeskController* instance = nullptr;
    if (!instance) { 
        instance = new DeskController(); 
        App.register_component(instance); 
    }
    return instance;
}