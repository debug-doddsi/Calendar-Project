#ifndef ADS1115_H
#define ADS1115_H


#include <stdio.h>
#include <string>
#include <cstdint>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>        // read/write usleep
#include <stdlib.h>        // exit function
#include <inttypes.h>      // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions


class ADS1115
{
public:
    ADS1115(std::string sBus, int iAddress);
    ~ADS1115();
    void Initialize();
    double ReadData(uint8_t channel);
    void ChangeConfig(uint8_t channel);

private:
    int fd;
    std::string i2cBus;
    int ads1115_address;
    uint8_t readBuf[3];
    double conversionFactor;

public:
    const uint16_t Ch1SingleEnded  = 0x4000;// AIN0(+)-AIN1(-)
    const uint16_t Ch2SingleEnded  = 0x5000;// AIN2(+)-AIN3(-)

    const uint16_t FS1024 = 0x0200;// +/-1.024V

    const uint16_t ContinuousConversion = 0x0000;

    const uint16_t SPS128 = 0x0080; // 128 Samples per Second

    uint16_t configCH1 = Ch1SingleEnded | FS1024 | ContinuousConversion | SPS128;
    uint16_t configCH2 = Ch2SingleEnded | FS1024 | ContinuousConversion | SPS128;
};

#endif // ADS1115_H