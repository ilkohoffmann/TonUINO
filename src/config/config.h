#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* Select the right PCB by uncommenting one of the following lines
 * Bitte die passende Platine durch entfernen der Kommentare in einer der folgenden Zeilen auswählen
 */
//#define TonUINO_Classic
//#define ALLinONE
#define ALLinONE_Plus

// TBD: Find better place for this
#define CONFIG_NUM_SHORTCUTS 4
#define DEFAULT_MAX_VOLUME 20
#define DEFAULT_MIN_VOLUME 3
#define DEFAULT_INIT_VOLUME 4
#define DEFAULT_EQ 1
#define DEFAULT_STANDBY_TIMER 0
#define DEFAULT_INVERT_VOLUME_BUTTONS true
#define DEFAULT_ADMIN_MENU_LOCKED 0
#define LONG_PRESS 1000
#define TIMEOUT 1000
#define TRACK_START_TIMEOUT 1000
#define TRACK_FINISH_TIMEOUT 1000

/* uncomment one of the below lines to enable special button support
 * um die Tasten zu konfigurieren, bitte eine der nächsten Zeilen auskommentieren
 * default: THREEBUTTONS for classic
 *          FIVEBUTTONS  for AiO and AiO+
 */
#define THREEBUTTONS
//#define FIVEBUTTONS
//#define BUTTONS3X3

/* uncomment the below line to enable serial input as additional command source
 * um den Serial Monitor als zusätzliche Kommandoquelle zu haben bitte in der nächste Zeile den Kommentar entfernen
 * -7:             -8: up         -9: upLong
 * -4: allLong     -5: pause      -6: pauseLong
 * -1: up/downLong -2: down       -3: downLong
 * number n > 0: Springe im Voice Menü zum n-ten Eintrag und selektiere ihn
 */
#define SerialInputAsCommand

/* uncomment one of the below lines to support a special chip on the DfMiniMp3 player
 * um einen speziellen Chip auf dem DfMiniMp3 Player zu ünterstützen bitte in eine der nächste Zeilen den Kommentar entfernen
 */
//#define DFMiniMp3_T_CHIP_GD3200B
//#define DFMiniMp3_T_CHIP_MH2024K16SS

/* uncomment the below line to disable shutdown via button (long press play/pause)
 * um ein Shutdown via Taste (long press Play/Pause) zu unterdrücken bitte in der nächste Zeile den Kommentar entfernen
 */
//#define DISABLE_SHUTDOWN_VIA_BUTTON

/* uncomment the below line to enable the rotary encoder for volume setting (only for AiOplus)
 * um den Drehgeber zu unterstützen bitte in der nächste Zeile den Kommentar entfernen (nur für AioPlus)
 */
//#define ROTARY_ENCODER
inline constexpr uint8_t   rotaryEncoderClkPin    = 31;
inline constexpr uint8_t   rotaryEncoderDtPin     = 32;

/* #################################################################################################
 * ##### normally, you don't have to edit lines below                   ############################
 * ##### mormalerweise müssen die folgende Zeilen nicht editiert werden ############################
 * #################################################################################################
 */

// ####### helper for level ############################

enum class level : uint8_t {
  inactive,
  active  ,
};
enum class levelType : uint8_t {
  activeHigh, // normally open
  activeLow , // normally closed
};

inline constexpr int getLevel(levelType t, level l) { return (l == level::inactive) ? (t == levelType::activeHigh ? LOW : HIGH)
                                                                                    : (t == levelType::activeHigh ? HIGH : LOW); }
// ####### rules for buttons ############################

#ifdef BUTTONS3X3
#ifdef FIVEBUTTONS
static_assert(false, "The 3x3 Button board doesn't have 5 Buttons");
#endif
inline constexpr uint8_t buttonExtSC_begin   = 101;
inline constexpr uint8_t buttonExtSC_buttons =  18;
#endif // BUTTONS3X3

inline constexpr uint32_t  buttonLongPress       = 1000; // timeout for long press button in ms
inline constexpr uint32_t  buttonLongPressRepeat =  200; // timeout for long press button repeat in ms

/***************************************************************************
 ** Classic ****************************************************************
 ***************************************************************************/

#ifdef TonUINO_Classic
// ####### buttons #####################################

inline constexpr uint8_t   CONFIG_BUTTON_PAUSE_PIN  = A0;

#if defined(BUTTONS3X3)
inline constexpr uint8_t   button3x3Pin    = A3;
inline constexpr uint8_t   CONFIG_BUTTON_UP_PIN     = A1;
inline constexpr uint8_t   CONFIG_BUTTON_DOWN_PIN   = A2;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#elif defined(FIVEBUTTONS)
inline constexpr uint8_t   CONFIG_BUTTON_UP_PIN     = A3;
inline constexpr uint8_t   CONFIG_BUTTON_DOWN_PIN   = A4;
inline constexpr uint8_t   CONFIG_BUTTON_4_PIN   = A1;
inline constexpr uint8_t   CONFIG_BUTTON_5_PIN   = A2;
#else
inline constexpr uint8_t   CONFIG_BUTTON_UP_PIN     = A1;
inline constexpr uint8_t   CONFIG_BUTTON_DOWN_PIN   = A2;
#endif

inline constexpr levelType buttonPinType   = levelType::activeLow;
inline constexpr uint32_t  buttonDbTime    = 25; // Debounce time in milliseconds (default 25ms)

// ####### chip_card ###################################

inline constexpr uint32_t CARD_COOKIE      = 0x1337b347;
inline constexpr uint8_t  CARD_VERSION     = 0x02;
inline constexpr byte     CONFIG_RST_PIN  =  9;
inline constexpr byte     CONFIG_SS_PIN   = 10;
inline constexpr uint8_t  cardRemoveDelay =  3;

// ####### mp3 #########################################

#define DFPlayerUsesSoftwareSerial
#define CHECK_MISSING_ONPLAYFINISHED

inline constexpr uint8_t       maxTracksInFolder        = 255;
inline constexpr uint8_t       dfPlayer_receivePin      = 2;
inline constexpr uint8_t       dfPlayer_transmitPin     = 3;
inline constexpr uint8_t       CONFIG_DFPLAYER_BUSY_PIN         = 4;
inline constexpr levelType     DFPLAYER_BUSY_PIN_TYPE     = levelType::activeHigh;
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1000;

// ####### tonuino #####################################

inline constexpr uint8_t       CONFIG_SHUTDOWN_PIN     = 7;
inline constexpr levelType     SHUTDOWN_PIN_TYPE = levelType::activeHigh;
inline constexpr uint8_t       CONFIG_OPEN_ANALOG_PIN   = A7;
inline constexpr unsigned long cycleTime       = 50;
#endif /* TonUINO_Classic */

/***************************************************************************
 ** AiO plus ***************************************************************
 ***************************************************************************/

#ifdef ALLinONE_Plus
// ####### buttons #####################################

#if not defined(THREEBUTTONS) and not defined(BUTTONS3X3)
#define FIVEBUTTONS
#endif

inline constexpr uint8_t   CONFIG_BUTTON_PAUSE_PIN  = A0;

#ifdef BUTTONS3X3
inline constexpr uint8_t   button3x3Pin    = A2;
inline constexpr uint8_t   CONFIG_BUTTON_UP_PIN     = A4;
inline constexpr uint8_t   CONFIG_BUTTON_DOWN_PIN   = A3;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#else
inline constexpr uint8_t   CONFIG_BUTTON_UP_PIN     = A2;
inline constexpr uint8_t   CONFIG_BUTTON_DOWN_PIN   = A1;
#endif

#ifdef FIVEBUTTONS
inline constexpr uint8_t   CONFIG_BUTTON_4_PIN   = A4;
inline constexpr uint8_t   CONFIG_BUTTON_5_PIN   = A3;
#endif

inline constexpr levelType buttonPinType   = levelType::activeLow;
inline constexpr uint32_t  buttonDbTime    = 25; // Debounce time in milliseconds (default 25ms)

// ####### chip_card ###################################

// inline constexpr uint32_t CARD_COOKIE      = 0x1337b347;
inline constexpr uint32_t CARD_COOKIE      = 322417479;
inline constexpr uint8_t  CARD_VERSION_TEST     = 2;
inline constexpr byte     CONFIG_RST_PIN  = 11;
inline constexpr byte     CONFIG_SS_PIN   =  7;
inline constexpr uint8_t  cardRemoveDelay =  3;

// ####### mp3 #########################################

#define DFPlayerUsesHardwareSerial
#define CHECK_MISSING_ONPLAYFINISHED

inline constexpr uint8_t        maxTracksInFolder        = 255;
// inline constexpr uint8_t        dfPlayer_receivePin      =  2;
// inline constexpr uint8_t        dfPlayer_transmitPin     =  3;
inline constexpr uint8_t        CONFIG_DFPLAYER_BUSY_PIN         = 13;
inline constexpr levelType      DFPLAYER_BUSY_PIN_TYPE     = levelType::activeHigh;
inline constexpr unsigned long  dfPlayer_timeUntilStarts = 1000;
inline constexpr HardwareSerial &dfPlayer_serial         = Serial3;
// ####### tonuino #####################################

inline constexpr uint8_t       CONFIG_SHUTDOWN_PIN      = 27;
inline constexpr levelType     SHUTDOWN_PIN_TYPE  = levelType::activeLow;
inline constexpr uint8_t       ampEnablePin     = 19;
inline constexpr levelType     ampEnablePinType = levelType::activeLow;
inline constexpr uint8_t       usbAccessPin     = 20;
inline constexpr levelType     usbAccessPinType = levelType::activeHigh;
inline constexpr uint8_t       CONFIG_OPEN_ANALOG_PIN    = A7;
inline constexpr unsigned long cycleTime        = 50;
#endif /* ALLinONE_Plus */

/***************************************************************************
 ** AiO ********************************************************************
 ***************************************************************************/

#ifdef ALLinONE
// ####### buttons #####################################

#if not defined(THREEBUTTONS) and not defined(BUTTONS3X3)
#define FIVEBUTTONS
#endif

inline constexpr uint8_t   CONFIG_BUTTON_PAUSE_PIN  = A0;

#ifdef BUTTONS3X3
inline constexpr uint8_t   button3x3Pin    = A2;
inline constexpr uint8_t   CONFIG_BUTTON_UP_PIN     = A4;
inline constexpr uint8_t   CONFIG_BUTTON_DOWN_PIN   = A3;
inline constexpr uint32_t  button3x3DbTime = 50; // Debounce time in milliseconds (default 50ms)
#else
inline constexpr uint8_t   CONFIG_BUTTON_UP_PIN     = A2;
inline constexpr uint8_t   CONFIG_BUTTON_DOWN_PIN   = A1;
#endif

#ifdef FIVEBUTTONS
inline constexpr uint8_t   CONFIG_BUTTON_4_PIN   = A4;
inline constexpr uint8_t   CONFIG_BUTTON_5_PIN   = A3;
#endif

inline constexpr levelType buttonPinType   = levelType::activeLow;
inline constexpr uint32_t  buttonDbTime    = 25; // Debounce time in milliseconds (default 25ms)

// ####### chip_card ###################################

inline constexpr uint32_t CARD_COOKIE      = 0x1337b347;
inline constexpr uint8_t  CARD_VERSION     = 0x02;
inline constexpr byte     CONFIG_RST_PIN  =  9;
inline constexpr byte     CONFIG_SS_PIN   = 10;
inline constexpr uint8_t  cardRemoveDelay =  3;

// ####### mp3 #########################################

#define DFPlayerUsesSoftwareSerial
#define CHECK_MISSING_ONPLAYFINISHED

inline constexpr uint8_t       maxTracksInFolder        = 255;
inline constexpr uint8_t       dfPlayer_receivePin      = 2;
inline constexpr uint8_t       dfPlayer_transmitPin     = 3;
inline constexpr uint8_t       CONFIG_DFPLAYER_BUSY_PIN         = 4;
inline constexpr levelType     DFPLAYER_BUSY_PIN_TYPE     = levelType::activeHigh;
inline constexpr unsigned long dfPlayer_timeUntilStarts = 1000;

// ####### tonuino #####################################

inline constexpr uint8_t       CONFIG_SHUTDOWN_PIN      = 7;
inline constexpr levelType     SHUTDOWN_PIN_TYPE  = levelType::activeLow;
inline constexpr uint8_t       ampEnablePin     = 8;
inline constexpr levelType     ampEnablePinType = levelType::activeLow;
inline constexpr uint8_t       usbAccessPin     = A5;
inline constexpr levelType     usbAccessPinType = levelType::activeHigh;
inline constexpr uint8_t       CONFIG_OPEN_ANALOG_PIN    = A7;
inline constexpr unsigned long cycleTime        = 50;
#endif /* ALLinONE */

// ####### some helper fuctions #####################################

template <typename T> void PROGMEM_read(const T * sce, T& dest)
{
  memcpy_P (&dest, sce, sizeof (T));
}



#endif /* CONFIG_H */
