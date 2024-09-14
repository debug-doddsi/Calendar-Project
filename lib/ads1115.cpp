#include "ads1115.h"
#include <sys/ioctl.h>
#include <iostream>
#include <chrono>
#include <thread>

ADS1115::ADS1115(std::string sBus, int iAddress)
    : fd(0), i2cBus(sBus), ads1115_address(iAddress)
{
  Initialize(); // init when constructing
}

ADS1115::~ADS1115()
{
  close(fd);
}

void ADS1115::Initialize()
{
  // open device on /dev/i2c-1
  if ((fd = open(i2cBus.c_str(), O_RDWR)) < 0)
  {
    printf("ADS1115 Error: Couldn't open device! %d\n", fd);
  }
  // connect to ADS1115 as i2c slave
  if (ioctl(fd, I2C_SLAVE, ads1115_address) < 0)
  {
    printf("Error: Couldn't find ADS1115 on address!\n");
  }

  ChangeConfig(1);
}

void ADS1115::ChangeConfig(uint8_t channel)
{
  uint16_t configPacked = configCH1;
  if (channel == 2)
  {
    configPacked = configCH2;
  }

  char config[3] = {0};
  // Select configuration register(0x01)
  config[0] = 0x01;
  config[1] = configPacked >> 8;
  config[2] = configPacked & 0xff;
  if (write(fd, config, 3) != 3)
  {
    printf("Error: Couldn't configure ADS1115\n");
  }
}

double ADS1115::ReadData(uint8_t channel)
{
  ChangeConfig(channel);
  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  int16_t raw_adc;
  // Read 2 bytes of data from register(0x00)
  // raw_adc msb, raw_adc lsb
  char reg[1] = {0x00};
  write(fd, reg, 1);
  uint8_t data[2] = {0};
  if (read(fd, data, 2) != 2)
  {
    printf("Error : Input/Output Error \n");
    raw_adc = 0;
  }
  else
  {
    // Convert the data
    raw_adc = ((data[0] << 8) + data[1]);
  }
  return double(raw_adc);
}