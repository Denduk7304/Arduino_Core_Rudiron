/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul
  Modified for Rudiron_Arduino_Core by Daniil Ignatev on 08.03.2022
*/

#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"
#include "rudiron/gpio.h"

using namespace Rudiron;

void pinMode(uint8_t pin, uint8_t mode)
{
  PortPinName pinName = GPIO::pinMap[pin];

  switch (mode)
  {
  case INPUT:
    Rudiron::GPIO::configPinInput(pinName);
    break;
  case INPUT_PULLUP:
    Rudiron::GPIO::configPinInputPullUp(pinName);
    break;
  case INPUT_PULLDOWN:
    Rudiron::GPIO::configPinInputPullDown(pinName);
    break;
  case OUTPUT:
    Rudiron::GPIO::configPinOutput(pinName);
    break;
  default:
    break;
  }
}

void digitalWrite(uint8_t pin, uint8_t val)
{
  PortPinName pinName = GPIO::pinMap[pin];
  GPIO::writePin(pinName, (bool)val);
}

int digitalRead(uint8_t pin)
{
  PortPinName pinName = GPIO::pinMap[pin];
  return GPIO::readPin(pinName);
}
