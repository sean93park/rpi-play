/*
  Copyright 2015 sean93.park@gmail.com

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef __MCP23017_HEADER__
#define __MCP23017_HEADER__

#define MCP23017_ADDR     0x20  // I2C address

#define MCP23017_IODIRA   0x00  // A direction
#define MCP23017_IODIRB   0x01  // B direction

#define MCP23017_GPIOA    0x12
#define MCP23017_GPIOB    0x13

#define MCP23017_OLATA    0x14  // A output latch
#define MCP23017_OLATB    0x15  // B output latch


#endif // __MCP23017_HEADER__
