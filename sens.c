/**
 * Sensor handling implementation
 */

#include "sens.h"
#include <fcntl.h>
#include <stdlib.h>

/**
 * Read data from a sysfs file.
 *
 * This function opens a sysfs file, reads its contents,
 * and closes the file.  It returns the number of bytes read
 * or a negative value if an error occurred.
 *
 * @param device The path to the sysfs file.
 * @param buffer A buffer to store the data read from the file.
 * @param size The maximum number of bytes to read.
 * @return The number of bytes read, or a negative value on error.
 */
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

/**
 * @brief Read an integer value from a sysfs file
 *
 * @param device Path to the sysfs file
 * @param value Pointer to store the read integer value
 * @return true if successful, false otherwise
 */
static bool read_int_sysfs(char *device, int *value) {
  char buf[8] = {0};
  ssize_t len = read_sysfs(device, buf, sizeof(buf));

  if (len > 0) {
    *value = strtol(buf, NULL, 10);
    return true;
  }

  return false;
}

/**
 * @brief Read the enable status from the sysfs file for a given device.
 *
 * This function reads an integer value from the sysfs file corresponding to
 * the specified device. If the read is successful, it returns true if the
 * value is non-zero and false otherwise.
 *
 * @param device The path to the sysfs file for the device.
 * @return bool True if the device is enabled, false otherwise.
 */
static bool read_enable_sysfs(char *device) {
  int value = 0;
  bool valid = read_int_sysfs(device, &value);

  if (valid) {
    return value ? true : false;
  }

  return false;
}

/**
 * @brief Write data to a specified sysfs file.
 *
 * This function enables writing of data to a sysfs file,
 * allowing interaction with kernel subsystems.
 *
 * @param device The name of the sysfs file to write to.
 * @param buffer A pointer to the data to be written to the sysfs file.
 * @param size The number of bytes to write from the buffer.
 * @return The number of bytes successfully written to the sysfs file,
 *         or -1 if an error occurred during the operation.
 */
static ssize_t write_sysfs(char *device, char *buffer, ssize_t size) {
  int fd = open(device, O_RDWR);
  ssize_t len = -1;
  if (fd < 0) {
    perror("Failed to open sysfs for writing");
    return -1;
  }

  len = write(fd, buffer, size);
  if (len < 0) {
    perror("Failed to write to sysfs");
    close(fd);
    return -1;
  }

  close(fd);
  return len;
}

/**
 * Write the enable state to the sysfs file for a given device.
 *
 * @param device The sysfs file path to the device.
 * @param enable True to enable the device, false to disable.
 */
static void write_enable_sysfs(char *device, bool enable) {
  char buf[2] = {0};
  buf[0] = enable ? '1' : '0';

  write_sysfs(device, buf, sizeof(buf));
}

// Public functions

//----------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------

void sens_get_enable(struct enable *data) {
  data->i2c = read_enable_sysfs("/sys/bus/i2c/devices/1-0036/enable");
  data->spi = read_enable_sysfs("/sys/bus/spi/devices/spi0.0/enable");
  data->mm = read_enable_sysfs("/sys/class/mmsens/mmsens0/enable");
}

//----------------------------------------------------------------------------------

void sens_set_enable(struct enable *data) {
  write_enable_sysfs("/sys/bus/i2c/devices/1-0036/enable", data->i2c);
  write_enable_sysfs("/sys/bus/spi/devices/spi0.0/enable", data->spi);
  write_enable_sysfs("/sys/class/mmsens/mmsens0/enable", data->mm);
}
