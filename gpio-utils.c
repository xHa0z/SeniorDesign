/* Copyright (c) 2011, RidgeRun
 * All rights reserved.
 *
From https://www.ridgerun.com/developer/wiki/index.php/Gpio-int-test.c

 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gpio-utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
 
	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, const char* dir)
{
	int fd, len;
	char buf[MAX_BUF];
	char chip;
	
	if(gpio < 32) {
		chip = 'A';
	} else if(gpio < 64) {
		chip = 'B';
	} else if(gpio < 96) {
		chip = 'C';
	} else if(gpio < 128) {
		chip = 'D';
	} else {
		chip = 'E';
	}
 
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/pio%c%d/direction", chip, gpio - (32*(chip-'A')));
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	write(fd, dir, sizeof(dir)+1);

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd, len;
	char buf[MAX_BUF];
	char chip;
	
	if(gpio < 32) {
		chip = 'A';
	} else if(gpio < 64) {
		chip = 'B';
	} else if(gpio < 96) {
		chip = 'C';
	} else if(gpio < 128) {
		chip = 'D';
	} else {
		chip = 'E';
	}
 
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/pio%c%d/value", chip, gpio - (32*(chip-'A')));
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}
 
	if (value)
		write(fd, "1", 2);
	else
		write(fd, "0", 2);
 
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd, len;
	char buf[MAX_BUF];
	char ch;
	char chip;

	if(gpio < 32) {
		chip = 'A';
	} else if(gpio < 64) {
		chip = 'B';
	} else if(gpio < 96) {
		chip = 'C';
	} else if(gpio < 128) {
		chip = 'D';
	} else {
		chip = 'E';
	}

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/pio%c%d/value", chip, gpio - (32*(chip-'A')));
 
	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror(buf);
		return fd;
	}
 
	read(fd, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}
 
	close(fd);
	return 0;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, const char *edge)
{
	int fd, len;
	char buf[MAX_BUF];
	char chip;

	if(gpio < 32) {
		chip = 'A';
	} else if(gpio < 64) {
		chip = 'B';
	} else if(gpio < 96) {
		chip = 'C';
	} else if(gpio < 128) {
		chip = 'D';
	} else {
		chip = 'E';
	}

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/pio%c%d/edge", chip, gpio - (32*(chip-'A')));
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}
 
	write(fd, edge, strlen(edge) + 1); 
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio, unsigned int dir)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
 
	fd = open(buf, dir | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
	return close(fd);
}

/****************************************************************
 * ain_get_value (from Mike McDonald)
 * https://groups.google.com/forum/#!topic/beagleboard-ece497/SLJ5nQQ_GoU
 ****************************************************************/
int ain_get_value(unsigned int ain, unsigned int *value)
{
    int fd, len, bytesRead;
    char buf[MAX_BUF];
    char adc_in[ADC_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_AIN_DIR "/AIN%d", ain);
 
    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror(buf);
        return fd;
    }
    
    // Read from the 
    bytesRead = read(fd, adc_in, ADC_BUF);

    // Turn the buffer value (a string) into an integer
    if (bytesRead != -1) {
        *value = atoi(adc_in);
        adc_in[bytesRead] = (int)NULL;
        lseek(fd, 0, SEEK_SET);
    }

    // Sleep for a little to ensure that we get good ADC values
    usleep(1000);
 
    close(fd);
    return bytesRead;
}

