#include "hid.h"
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

// ------------------------------
// USB HID (Adafruit TinyUSB)

Adafruit_USBD_HID usb_hid;

uint8_t const hid_report_desc[] = {
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1)),
  TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(2))
};

void setupUsbHid() {
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(hid_report_desc, sizeof(hid_report_desc));
  usb_hid.begin();
}

bool asciiToHid(uint8_t ascii, uint8_t* modifier, uint8_t* keycode) {
  *modifier = 0;
  switch (ascii) {
    case 'a' ... 'z':
      *keycode = (uint8_t)(ascii - 'a' + 0x04);
      return true;
    case 'A' ... 'Z':
      *keycode = (uint8_t)(ascii - 'A' + 0x04);
      *modifier = 0x02; // left shift
      return true;
    case '1' ... '9':
      *keycode = (uint8_t)(ascii - '1' + 0x1E);
      return true;
    case '0':
      *keycode = 0x27;
      return true;
    case ' ':
      *keycode = 0x2C;
      return true;
    case '\n':
    case '\r':
      *keycode = 0x28;
      return true;
    case '\t':
      *keycode = 0x2B;
      return true;
    case '-':
      *keycode = 0x2D;
      return true;
    case '=':
      *keycode = 0x2E;
      return true;
    case '[':
      *keycode = 0x2F;
      return true;
    case ']':
      *keycode = 0x30;
      return true;
    case '\\':
      *keycode = 0x31;
      return true;
    case ';':
      *keycode = 0x33;
      return true;
    case '\'':
      *keycode = 0x34;
      return true;
    case '`':
      *keycode = 0x35;
      return true;
    case ',':
      *keycode = 0x36;
      return true;
    case '.':
      *keycode = 0x37;
      return true;
    case '/':
      *keycode = 0x38;
      return true;
    case '!':
      *keycode = 0x1E;
      *modifier = 0x02;
      return true;
    case '@':
      *keycode = 0x1F;
      *modifier = 0x02;
      return true;
    case '#':
      *keycode = 0x20;
      *modifier = 0x02;
      return true;
    case '$':
      *keycode = 0x21;
      *modifier = 0x02;
      return true;
    case '%':
      *keycode = 0x22;
      *modifier = 0x02;
      return true;
    case '^':
      *keycode = 0x23;
      *modifier = 0x02;
      return true;
    case '&':
      *keycode = 0x24;
      *modifier = 0x02;
      return true;
    case '*':
      *keycode = 0x25;
      *modifier = 0x02;
      return true;
    case '(':
      *keycode = 0x26;
      *modifier = 0x02;
      return true;
    case ')':
      *keycode = 0x27;
      *modifier = 0x02;
      return true;
    case '_':
      *keycode = 0x2D;
      *modifier = 0x02;
      return true;
    case '+':
      *keycode = 0x2E;
      *modifier = 0x02;
      return true;
    case '{':
      *keycode = 0x2F;
      *modifier = 0x02;
      return true;
    case '}':
      *keycode = 0x30;
      *modifier = 0x02;
      return true;
    case '|':
      *keycode = 0x31;
      *modifier = 0x02;
      return true;
    case ':':
      *keycode = 0x33;
      *modifier = 0x02;
      return true;
    case '"':
      *keycode = 0x34;
      *modifier = 0x02;
      return true;
    case '~':
      *keycode = 0x35;
      *modifier = 0x02;
      return true;
    case '<':
      *keycode = 0x36;
      *modifier = 0x02;
      return true;
    case '>':
      *keycode = 0x37;
      *modifier = 0x02;
      return true;
    case '?':
      *keycode = 0x38;
      *modifier = 0x02;
      return true;
    default:
      return false;
  }
}

void sendKeyboardReport(uint8_t keycode, uint8_t modifier) {
  if (!TinyUSBDevice.mounted()) return;
  uint8_t keycodes[6] = {keycode, 0, 0, 0, 0, 0};
  tud_hid_keyboard_report(1, modifier, keycodes);
  delay(10);
  uint8_t empty[6] = {0};
  tud_hid_keyboard_report(1, 0, empty);
}

void sendKeyboardKey(uint8_t ascii) {
  if (!TinyUSBDevice.mounted()) return;
  uint8_t modifier = 0, keycode = 0;
  if (ascii < 0x20) {
    keycode = ascii;
  } else if (!asciiToHid(ascii, &modifier, &keycode)) {
    return;
  }
  sendKeyboardReport(keycode, modifier);
}

void sendConsumerKey(uint16_t key) {
  if (!TinyUSBDevice.mounted()) return;
  tud_hid_report(2, &key, sizeof(key));
  delay(10);
  uint16_t empty = 0;
  tud_hid_report(2, &empty, sizeof(empty));
}
