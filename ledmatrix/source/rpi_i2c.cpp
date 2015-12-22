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

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

#include "rpi_i2c.h"

rpi2c::rpi2c() {
  addr_ = 0;
  fd_ = -1;
}

rpi2c::~rpi2c() {
   close();
}

int rpi2c::open(const char* file, i2caddr addr) {
  if (!(file && *file)) {
    printf("rpi2c::open(), invalid file name\r\n");
    return -1;
  }
  addr_ = addr;
  fd_ = ::open(file, O_RDWR);
  if (fd_ < 0) {
    printf("rpi2c::open(), open failed '%s', %d\r\n", file, errno);
    return -1;
  }
  return fd_;
}

int rpi2c::close(void) {
  int retval = -1;
  if (fd_ > 0) {
    retval = ::close(fd_);
    if (retval < 0) {
      printf("rpi2c::close, close failed, %d\r\n", retval);
    }
  }
  addr_ = 0;
  fd_ = -1;
  return retval;
}

int rpi2c::write(i2caddr reg_addr, i2cbyte data) {
  if (fd_ < 0) {
    return -1;
  }

  i2cbyte buff[2];
  int retval = -1;
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[1];

  buff[0] = reg_addr;
  buff[1] = data;

  messages[0].addr = addr_;
  messages[0].flags = 0;
  messages[0].len = sizeof(buff);
  messages[0].buf = buff;

  packets.msgs  = messages;
  packets.nmsgs = 1;

  retval = ::ioctl(fd_, I2C_RDWR, &packets);
  if (retval < 0) {
    printf("rpi2c::write(), write failed %d\r\n", retval);
  }

  return retval;
}

int rpi2c::read(i2caddr reg_addr, i2cbyte &data) {
  if (fd_ < 0) {
    return -1;
  }

  i2cbyte *inbuff, outbuff;
  int retval = -1;
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[2];

  outbuff = reg_addr;
  messages[0].addr = addr_;
  messages[0].flags= 0;
  messages[0].len = sizeof(outbuff);
  messages[0].buf = &outbuff;

  inbuff = &data;
  messages[1].addr = addr_;
  messages[1].flags = I2C_M_RD;
  messages[1].len = sizeof(inbuff);
  messages[1].buf = inbuff;

  packets.msgs = messages;
  packets.nmsgs = 2;

  retval = ioctl(fd_, I2C_RDWR, &packets);
  if (retval < 0) {
    printf("rpi2c::read(), read failed, %d\n", retval);
  }

  return retval;
}
