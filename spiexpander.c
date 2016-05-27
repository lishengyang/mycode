#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#define CMD_WRITE 0x40
#define CMD_READ 0x41
static char *spiDevice = "/dev/spidev0.0" ;
static uint8_t spiMode = 0 ;
static uint8_t spiBPW = 8 ;
static uint32_t spiSpeed = 5000000 ;
static uint16_t spiDelay = 0;
// MCP23S17 Registers
#define IOCON 0x0A
#define IODIRA 0x00
#define IPOLA 0x02
#define GPINTENA 0x04
#define DEFVALA 0x06
#define INTCONA 0x08
#define GPPUA 0x0C
#define INTFA 0x0E
#define INTCAPA 0x10
#define GPIOA 0x12
#define OLATA 0x14
#define IODIRB 0x01
#define IPOLB 0x03
#define GPINTENB 0x05
#define DEFVALB 0x07
#define INTCONB 0x09
#define GPPUB 0x0D
#define INTFB 0x0F
#define INTCAPB 0x11
#define GPIOB 0x13
#define OLATB 0x15
int spi_fd;
static uint8_t readByte (uint8_t reg)
{
uint8_t tx [4] ;
uint8_t rx [4] ;
struct spi_ioc_transfer spi ;
tx [0] = CMD_READ ;
tx [1] = reg ;
tx [2] = 0 ;
spi.tx_buf = (unsigned long)tx ;
spi.rx_buf = (unsigned long)rx ;
spi.len = 3 ;
spi.delay_usecs = spiDelay ;
spi.speed_hz = spiSpeed ;
spi.bits_per_word = spiBPW ;
ioctl (spi_fd, SPI_IOC_MESSAGE(1), &spi) ;
return rx [2] ;
}
static void writeByte (uint8_t reg, uint8_t data)
{
uint8_t spiBufTx [3] ;
uint8_t spiBufRx [3] ;
struct spi_ioc_transfer spi ;
spiBufTx [0] = CMD_WRITE ;
spiBufTx [1] = reg ;
spiBufTx [2] = data ;
spi.tx_buf = (unsigned long)spiBufTx ;
spi.rx_buf = (unsigned long)spiBufRx ;
spi.len = 3 ;
spi.delay_usecs = spiDelay ;
spi.speed_hz = spiSpeed ;
spi.bits_per_word = spiBPW ;
ioctl (spi_fd, SPI_IOC_MESSAGE(1), &spi) ;
}
/*spi_open
* - Open the given SPI channel and configures it.
* - there are normally two SPI devices on your PI:
* /dev/spidev0.0: activates the CS0 pin during transfer
* /dev/spidev0.1: activates the CS1 pin during transfer
*
*/
int spi_open(char* dev)
{
if((spi_fd = open(dev, O_RDWR)) < 0){
printf("error opening %s\n",dev);
return -1;
}
return 0;
}
int main(int argc, char* argv[])
{
unsigned char data = 0xAF;
if(argc <= 1){
printf("too few args, try %s /dev/spidev0.0\n",argv[0]);
return -1;
}
// open and configure SPI channel. (/dev/spidev0.0 for example)
if(spi_open(argv[1]) < 0){
printf("spi_open failed\n");
return -1;
}
writeByte (IODIRA, 0x00) ; // Port A -> Outputs
writeByte (GPIOA, 0xFF) ;
//writeByte (IPOLB, 0x01) ; // invert lsb
//writeByte (GPPUB, 0xFF) ; // enable pullups.
writeByte (IODIRB, 0xFF) ; // Port B -> Inputs
data = readByte (GPIOB) ;
printf("RECEIVED: %.2X\n",data);
close(spi_fd);
return 0;
}
