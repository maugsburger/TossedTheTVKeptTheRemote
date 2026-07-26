#pragma once
#include <stdint.h>

void setupUsbHid();
bool asciiToHid(uint8_t ascii, uint8_t* modifier, uint8_t* keycode);
void sendKeyboardReport(uint8_t keycode, uint8_t modifier = 0);
void sendKeyboardKey(uint8_t ascii);
void sendConsumerKey(uint16_t key);
