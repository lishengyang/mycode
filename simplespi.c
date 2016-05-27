// Rapsberry Pi: SPI in C - Versione 0.2 - Luglio 2013
// Copyright (c) 2013, Vincenzo Villa (http://www.vincenzov.net)
// Creative Commons | Attribuzione-Condividi allo stesso modo 3.0 Unported.
// Creative Commons | Attribution-Share Alike 3.0 Unported
// http://www.vincenzov.net/tutorial/elettronica-di-base/RaspberryPi/spi-c.htm

// Compile:  gcc spi.c -std=c99 -o spi
// Run as user with R&W right on /dev/spidev0.* (NOT ROOT!)
// Connect MOSI and MISO in loopback (see text)

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

static const char *device = "/dev/spidev0.0";// /dev/spidev0.0 or /dev/spidev0.1
static uint8_t mode = 0;			// SPI_MODE_0 (see text)
static uint8_t bits = 8;			// 8 or 9
static uint32_t speed = 500000;		// Clock frequency [Hz]
static uint16_t delay = 0;			// Time between end of data and CS de-assert

static void exit_on_error (const char *s)	// Exit and print error code
{ 	perror(s);
  	abort();
} 

int main(int argc, char *argv[])
{
	int fd;
	int Buffer_size;
	unsigned int i;
	uint8_t tx[] = {0x11, 0x22, 0x44, 0x88, 0x55, 0x00,0xAA, 0xFF, };
                                            	// Data to send
       	Buffer_size = sizeof(tx);		// Size of TX (and RX) buffer
      	uint8_t rx[sizeof(tx)] = {0, };    	// RX buffer
                        
      	struct spi_ioc_transfer tr = 
      	{	.tx_buf = (unsigned long)tx,         
          	.rx_buf = (unsigned long)rx,        
          	.len = Buffer_size,               
          	.delay_usecs = delay,
          	.speed_hz = speed,   
          	.bits_per_word = bits,
          	.cs_change = 0,
       	};
        
       	// Open SPI device
       	if ((fd = open(device, O_RDWR)) < 0) exit_on_error ("Can't open SPI device");

	// Set SPI mode
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) exit_on_error ("Can't set SPI mode");

       	// Read and write data (full duplex)
       	if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) exit_on_error ("Can't send SPI message");
                
       	printf ("Data from SPI Rx buffer: ");
       	for (i = 0; i < Buffer_size; i++) printf("%.2X ", rx[i]);
       	printf ("\n");
                          
	close(fd);

	return (0);
}

