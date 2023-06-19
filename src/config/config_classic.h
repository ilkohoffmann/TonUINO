#ifndef CONFIG_H
#define CONFIG_H

// Buttons
#define CONFIG_BUTTON_PAUSE_PIN A0
#define CONFIG_BUTTON_UP_PIN A1
#define CONFIG_BUTTON_DOWN_PIN A2
#define CONFIG_BUSY_PIN 4
#define CONFIG_SHUTDOWN_PIN 7
#define CONFIG_OPEN_ANALOG_PIN A7

#ifdef FIVEBUTTONS
#define CONFIG_BUTTON_4_PIN A3
#define CONFIG_BUTTON_5_PIN A4
#endif

// AdminSettings
// default values

#define DEFAULT_MAX_VOLUME 25
#define DEFAULT_MIN_VOLUME 5
#define DEFAULT_INIT_VOLUME 15
#define DEFAULT_EQ 1
#define DEFAULT_STANDBY_TIMER 0
#define DEFAULT_INVERT_VOLUME_BUTTONS true
#define DEFAULT_ADMIN_MENU_LOCKED 0

// Global config
#define VERSION 2
#define CARD_COOKIE 322417479
#define LONG_PRESS 1000
#define TIMEOUT 1000
#define TRACK_START_TIMEOUT 1000
#define TRACK_FINISH_TIMEOUT 1000

// MISC
#define CONFIG_NUM_SHORTCUTS 4

// MFRC522
#define CONFIG_RST_PIN 9                 // Configurable, see typical pin layout above
#define CONFIG_SS_PIN 10                 // Configurable, see typical pin layout above

#endif