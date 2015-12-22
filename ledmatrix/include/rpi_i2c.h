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

#ifndef __RPI_I2C_HEADER__
#define __RPI_I2C_HEADER__

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

typedef unsigned char i2cbyte;
typedef unsigned char i2caddr;

class rpi2c {
public:
  rpi2c();
  rpi2c(int code);
  virtual ~rpi2c();

public:
  int open(const char* file, i2caddr addr);
  int close();
  int write(i2caddr reg_addr, i2cbyte data);
  int read(i2caddr reg_addr, i2cbyte &data);

protected:
  int fd_;
  i2caddr addr_;
};

#endif // __RPI_I2C_HEADER__
