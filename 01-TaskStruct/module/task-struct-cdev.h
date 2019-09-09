#ifndef CUSTOM_CDEV_MAJOR
#define CUSTOM_CDEV_MAJOR 0
#endif

#ifndef CUSTOM_CDEV_MINOR
#define CUSTOM_CDEV_MINOR 0
#endif

/* Scull device */
struct custom_cdev_dev {
  struct cdev cdev; /* Char device structure */
};
