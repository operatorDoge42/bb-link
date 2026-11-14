# B.B. Link on the M5Stack Atom Lite

> **Unofficial experimental port.** This guide is for the original M5Stack Atom Lite with ESP32-PICO-D4. It is not the Atom Matrix, the TinyPICO reference build, or an official release from the upstream project.

## Download

Download the [Atom Lite factory firmware](https://github.com/operatorDoge42/bb-link/releases/download/atom-lite-v0.1.0-alpha.1/bb-link-atom-lite-v0.1.0-alpha.1-factory.bin) and its [SHA-256 checksum](https://github.com/operatorDoge42/bb-link/releases/download/atom-lite-v0.1.0-alpha.1/bb-link-atom-lite-v0.1.0-alpha.1-factory.bin.sha256.txt) from this fork's experimental [Atom Lite release](https://github.com/operatorDoge42/bb-link/releases/tag/atom-lite-v0.1.0-alpha.1).

The factory image contains the ESP32 bootloader, partition table, initial OTA data, and application, combined into one 4 MB image. Flashing it replaces the Atom Lite's full flash contents, including previously saved radio pairing and adapter settings. The matching source code and this guide are included in the release's source archive; the source is also browsable [at the release tag](https://github.com/operatorDoge42/bb-link/tree/atom-lite-v0.1.0-alpha.1).

## Install on a Mac

This procedure is for a fresh USB installation or recovery. It uses Espressif's browser flasher. The flasher requires Chrome or Edge; Safari is not supported.

1. Download the factory `.bin` above.
2. Connect the Atom Lite to the Mac with a USB-C data cable. Close any app that may already be using its serial port.
3. Open [Espressif ESP Tool](https://espressif.github.io/esptool-js/) in Chrome or Edge and choose **Connect**. Select the Atom Lite's serial port.
4. In the Program area, set the flash address to `0x0` and select the factory `.bin` file. Set flash mode to `DIO`, frequency to `40 MHz`, and size to `4 MB`.
5. Start programming and wait for it to finish. The full image overwrites the device flash, so its saved pairing and settings are reset.
6. Unplug and reconnect the Atom Lite. It should start advertising as **B.B. Link** over BLE.

## Pair and use

The adapter bridges the radio's Bluetooth Classic serial connection to the iPhone's BLE connection. The radio side connects to the Kenwood TH-D74/TH-D75; the phone side is selected in a compatible packet app.

1. Turn on Bluetooth on the phone and the radio. Put the radio in Bluetooth pairing mode.
2. Open the B.B. Link Configurator app and pair the radio. Accept the pairing prompt on the Kenwood when it appears.
3. Close the Configurator app so it releases its BLE connection.
4. In RadioMail, open **Settings → KISS TNC Modem → Default TNC**, choose **B.B. Link**, then connect to a packet station. Other compatible BLE KISS clients may work as well.

## Atom Lite hardware and status light

- One RGB status LED on GPIO 27. It is not a 5×5 display.
- The Atom Lite's button is unused by this port.
- There is no built-in LiPo battery, battery monitoring, or battery shutdown. It runs while powered; unplug USB to turn it off.
- Amber: powered and waiting. Blue: searching for or connected to the radio. Cyan: phone and radio ready. Green: receiving. Red: transmitting or error. Purple: simultaneous RX/TX or firmware update.
- The BLE OTA service is present, and this build's partition table has two app slots. OTA updating has not been tested end to end on the Atom Lite, so the factory USB method above is the known installation and recovery path.

## Build from source

The code is Arduino firmware. To compile it yourself, install Arduino CLI, Espressif Arduino core 2.0.15, ArduinoLog 1.1.1, ArduinoQueue 1.2.5, and Adafruit NeoPixel 1.15.5. Then run `scripts/build-atom-lite.sh` from the repository. This compiles but does not upload to the board. `scripts/package-atom-lite.sh` builds the complete 4 MB factory image used above; it also needs Espressif's `esptool` and the `boot_app0.bin` file from the same Arduino ESP32 core installation.

The Atom Lite changes are maintained in this fork. The upstream README welcomes people to share builds, improve documentation, report issues, and submit changes by pull request; this port keeps those invitations and has not been represented as an upstream-approved change.

## Credit and license

This is a modification of [B.B. Link by islandmagic](https://github.com/islandmagic/bb-link), distributed under the [GNU General Public License v3](../LICENSE). The original project and license notices are retained. The Atom Lite hardware adaptation and its documentation are modifications made in this fork, dated 2026-11-13. This fork is unofficial and is not affiliated with or endorsed by the upstream project or M5Stack. The source corresponding to the downloadable firmware is available at the release tag and in the release's source archive. The firmware is provided without warranty under the GPLv3 terms.
