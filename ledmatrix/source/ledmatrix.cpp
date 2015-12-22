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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#include "rpi_i2c.h"
#include "mcp23017.h"
#include "fontdata.h"

static const unsigned char VVA[5] = { 0xfe, 0xfd, 0xfb, 0xf7, 0xef };
static const unsigned char VVB[5] = { 0x40, 0x20, 0x10, 0x08, 0x04 };

class led5x7matrix {
public:
  led5x7matrix();
  virtual ~led5x7matrix();

  bool init(void);
  int changechar(int ch);
  void setchardata(int ch);
  int tick(void);
  void run(void);

protected:
  unsigned char vva_[5];
  unsigned char vvb_[5];
  int col_;
  rpi2c mcp23017_;

  pthread_mutex_t mut_;
  int ch_new_;
  int ch_old_;
};


led5x7matrix::led5x7matrix()
{
  mut_ = PTHREAD_MUTEX_INITIALIZER;

  for (int i=0; i<5; i++) {
    vva_[i] = VVA[i];
    vvb_[i] = VVB[i];
  }
  col_ = 0;
}

led5x7matrix::~led5x7matrix()
{
  mcp23017_.write(MCP23017_OLATA, 0xff); // High is Off : Col
  mcp23017_.write(MCP23017_OLATB, 0x00); // Low is Off  : Row

  pthread_mutex_destroy(&mut_);
}

bool led5x7matrix::init(void)
{
  if (mcp23017_.open("/dev/i2c-1", MCP23017_ADDR) < 0) {
    printf("error, failed to open device '/dev/i2c-1'\r\n" );
    return false;
  }

  if (0 > mcp23017_.write(MCP23017_IODIRA, 0x00) || // port A to write mode
      0 > mcp23017_.write(MCP23017_IODIRB, 0x00) || // port B to write mode
      0 > mcp23017_.write(MCP23017_OLATA, 0xff) ||  // OFF
      0 > mcp23017_.write(MCP23017_OLATB, 0x00)) {  // OFF
    printf("error, failed to write I2C\r\n");
    return false;
  }

  return true;
}

void led5x7matrix::setchardata(int ch)
{
  int i, ca;
  ch -= (int)' ';
  ca = ch * 5;
  for (i=0; i<5; i++) {
    vvb_[i] = _Font5x7[ca+i];
  }
  col_ = 0;
}

int led5x7matrix::changechar(int ch)
{
  if (ch != -1) {
    if (ch < 32 || ch > 127) {
      return 0;
    }
  }
  pthread_mutex_lock(&mut_);
  ch_new_ = ch;
  pthread_mutex_unlock(&mut_);
  return ch;
}

int led5x7matrix::tick(void)
{
  register int col = col_;
  mcp23017_.write(MCP23017_OLATA, 0xff); // turn off column
  mcp23017_.write(MCP23017_OLATB, vvb_[col]); // send data for col
  mcp23017_.write(MCP23017_OLATA, vva_[col]); // turn on col

  col = (col == 4) ? 0 : col+1;
  col_ = col;

  return col;
}

void led5x7matrix::run(void)
{
  int newch, rc;

  while (true) {
    pthread_mutex_lock(&mut_);
    newch = ch_new_;
    pthread_mutex_unlock(&mut_);

    if (newch == -1) {
      // exit...
      return;
    }
    tick();
    if (ch_old_ != newch && col_ == 0) {
      setchardata(newch);
      ch_old_ = newch;
    }
    usleep(2000); // ~2msec -> ~500hz=5x100hz for each character
  }
}

void* ledmatrix_thr_proc(void* arg)
{
  led5x7matrix* led = (led5x7matrix*)arg;
  led->run();
  printf("!! thread: exit!\r\n");
  pthread_exit(NULL);
  return NULL;
}

static int _do_exit = 0;

void turnoff(int param)
{
  _do_exit = 1;
}

int main(void)
{
  signal(SIGTERM, turnoff);
  signal(SIGABRT, turnoff);
  signal(SIGINT, turnoff);

  led5x7matrix* led;
  int ch;

  led = new led5x7matrix();
  if (!led->init()) {
    delete led;
    exit(-1);
  }
  led->setchardata(' ');

  pthread_t thr;
  pthread_create(&thr, NULL, ledmatrix_thr_proc, led);

  for (ch=' '; ch<128; ch++) {
    printf(">> change ch [%c]\r\n", ch);
    if (!led->changechar(ch)) {
      printf(">> changechar failed\r\n");
      break;
    }
    usleep(800*1000);
    if (_do_exit) {
      printf(">> do exit !\r\n");
      break;
    }
  }
  led->changechar(-1);
  pthread_join(thr, NULL);
  printf(">> main: exit!\r\n");

  delete led;

  return 0;
}
