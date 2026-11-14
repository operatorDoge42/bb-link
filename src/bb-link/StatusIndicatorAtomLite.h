#pragma once

#include "StatusIndicatorBase.h"
#include <Adafruit_NeoPixel.h>

#define ATOM_LITE_RGB_PIN 27
#define ATOM_LITE_RGB_BRIGHTNESS 20

class StatusIndicatorAtomLite : public StatusIndicatorBase
{
public:
  StatusIndicatorAtomLite() : pixel(1, ATOM_LITE_RGB_PIN, NEO_GRB + NEO_KHZ800) {}

  void init() override
  {
    pixel.begin();
    pixel.setBrightness(ATOM_LITE_RGB_BRIGHTNESS);
    pixel.clear();
    pixel.show();
    status = disconnected;
    lastUpdate = millis();
    ledOn = true;
    shownColor = 0xFFFFFFFF;
    render();
  }

  void set(status_t newStatus) override
  {
    if (status != newStatus)
    {
      status = newStatus;
      lastUpdate = millis();
      ledOn = true;
    }
  }

  void render() override
  {
    const unsigned long now = millis();
    uint32_t color = colorForStatus();
    bool shouldBeOn = true;
    unsigned long interval = 0;

    switch (status)
    {
    case scanning:
    case error:
      interval = 500;
      break;
    case actionRegistered:
    case batteryLow:
    case batteryShutdown:
    case otaFlash:
      interval = 160;
      break;
    case shutdown:
      interval = 300;
      break;
    default:
      break;
    }

    if (interval > 0 && now - lastUpdate >= interval)
    {
      ledOn = !ledOn;
      lastUpdate = now;
    }
    shouldBeOn = ledOn;
    if (!shouldBeOn)
      color = 0;

    if (color != shownColor)
    {
      shownColor = color;
      pixel.setPixelColor(0, color);
      pixel.show();
    }
  }

  void sleep() override
  {
    shownColor = 0;
    pixel.clear();
    pixel.show();
  }

private:
  Adafruit_NeoPixel pixel;
  status_t status = disconnected;
  uint32_t shownColor = 0xFFFFFFFF;
  unsigned long lastUpdate = 0;
  bool ledOn = true;

  uint32_t colorForStatus() const
  {
    switch (status)
    {
    case disconnected:
    case shutdown:
    case actionRegistered:
      return pixel.Color(255, 80, 0); // amber: powered, waiting or action
    case batteryFull:
    case batteryLow:
    case rx:
      return pixel.Color(0, 255, 0); // green: receive
    case batteryShutdown:
    case tx:
    case error:
      return pixel.Color(255, 0, 0); // red: transmit or error
    case connected:
    case scanning:
      return pixel.Color(0, 0, 255); // blue: radio linked or searching
    case ready:
      return pixel.Color(0, 160, 255); // cyan: radio and phone linked
    case duplex:
    case otaFlash:
      return pixel.Color(180, 0, 255); // purple: simultaneous activity or update
    }
    return 0;
  }
};
