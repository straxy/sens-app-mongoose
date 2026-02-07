/**
 * Sensor handling implementation
 */

#include "sens.h"
#include <fcntl.h>
#include <stdlib.h>

static ssize_t read_sysfs(char *device, char *buffer, ssize_t size) {
  ssize_t len = 0;
  int fd = open(device, O_RDONLY);

  if (fd < 0) {
    perror("Could not open sysfs for reading");
    return fd;
  }

  len = read(fd, (void *)buffer, size);

  close(fd);

  return len;
}

static bool read_int_sysfs(char *device, int *value) {
  char buf[8] = {0};
  ssize_t len = read_sysfs(device, buf, sizeof(buf));

  if (len > 0) {
    *value = strtol(buf, NULL, 10);
    return true;
  }

  return false;
}

static bool read_enable_sysfs(char *device) {
  int value = 0;
  bool valid = read_int_sysfs(device, &value);

  if (valid) {
    return value ? true : false;
  }

  return false;
}

static ssize_t write_sysfs(char *device, char *buffer, ssize_t size) {
  int fd = open(device, O_RDWR);
  ssize_t len = 0;

  if (fd < 0) {
    perror("Could not open sysfs for writing");
    return fd;
  }

  len = write(fd, buffer, size);

  close(fd);

  return len;
}

static void write_enable_sysfs(char *device, bool enable) {
  char buf[2] = {0};
  buf[0] = enable ? '1' : '0';

  write_sysfs(device, buf, sizeof(buf));
}

// glue data
void sens_get_data(struct data *data) {
  struct data s_data = {.i2c = 127500, .spi = 127500, .mm = 0};
  bool valid = false;
  int value = 0;

  printf("Called get data\n");

  // Read I2C data
  valid = read_int_sysfs("/sys/bus/i2c/devices/1-0036/data", &value);
  if (valid) {
    s_data.i2c = value / 1000.;
  }
  // Read SPI data
  valid = read_int_sysfs("/sys/bus/spi/devices/spi0.0/data", &value);
  if (valid) {
    s_data.spi = value / 1000.;
  }
  // Read MMsens data
  valid = read_int_sysfs("/sys/class/mmsens/mmsens0/data", &value);
  if (valid) {
    s_data.mm = value;
  }

  *data = s_data;
}
// glue enable
void sens_get_enable(struct enable *data) {
  printf("Called get enable\n");

  data->i2c = read_enable_sysfs("/sys/bus/i2c/devices/1-0036/enable");
  data->spi = read_enable_sysfs("/sys/bus/spi/devices/spi0.0/enable");
  data->mm = read_enable_sysfs("/sys/class/mmsens/mmsens0/enable");
}
void sens_set_enable(struct enable *data) {
  printf("Called set enable\n");

  write_enable_sysfs("/sys/bus/i2c/devices/1-0036/enable", data->i2c);
  write_enable_sysfs("/sys/bus/spi/devices/spi0.0/enable", data->spi);
  write_enable_sysfs("/sys/class/mmsens/mmsens0/enable", data->mm);
}
