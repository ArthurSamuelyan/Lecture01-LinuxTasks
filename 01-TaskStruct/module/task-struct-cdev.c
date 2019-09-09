#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#include "task-struct-cdev.h"

/* Initialize major/minor but let the user have an argument */
static int custom_cdev_major = CUSTOM_CDEV_MAJOR;
static int custom_cdev_minor = CUSTOM_CDEV_MINOR;
module_param(custom_cdev_major, int, S_IRUGO);
module_param(custom_cdev_minor, int, S_IRUGO);

static int custom_cdev_nr_devs = 1;

struct custom_cdev_dev *custom_cdev_devices;

/* open function */
static int custom_cdev_open(struct inode *inode, struct file *filp)
{
  struct custom_cdev_dev *dev; /* device information */

  dev = container_of(inode->i_cdev, struct custom_cdev_dev, cdev);
  filp->private_data = dev; /* for other methods */

  return 0;
}

/* close operation */
static int custom_cdev_release(struct inode *inode, struct file *flip)
{
  return 0;
}

/* read function */
static ssize_t custom_cdev_read(struct file *flip, char __user *buf, size_t count, loff_t *f_pos)
{
  /* =========================
   * Insert payload code here. 
   * ========================= */

  /* current - 
   * kernel macros, allows to get the task_struct ptr
   * (while in thead context) */
  struct task_struct *cur_task_struct = current;

  printk(KERN_INFO "task-struct-cdev: ==================\n");

  /* ProcessID (honestly is a task id)
   * ThreadGroupID (== PID of the first thread)*/
  printk(KERN_INFO "task-struct-cdev: PID = %d\n", cur_task_struct->pid);
  printk(KERN_INFO "task-struct-cdev: TGID = %d\n", cur_task_struct->tgid);

  switch (cur_task_struct->state)
  {
    case -1:
      printk(KERN_INFO "task-struct-cdev: state = UNRUNNABLE\n");
      break;
    case 0:
      printk(KERN_INFO "task-struct-cdev: state = RUNNABLE\n");
      break;
    default:
      printk(KERN_INFO "task-struct-cdev: state = STOPPED\n");
      break;
  }

  printk(KERN_INFO "task-struct-cdev: priority = %d\n", cur_task_struct->prio);
  printk(KERN_INFO "task-struct-cdev: comm = %s\n", cur_task_struct->comm);

  printk(KERN_INFO "task-struct-cdev: sighand = %p\n", (const void*)cur_task_struct->sighand);
  printk(KERN_INFO "task-struct-cdev: mm = %p\n", (const void*)cur_task_struct->mm);
  printk(KERN_INFO "task-struct-cdev: active_mm = %p\n", (const void*)cur_task_struct->active_mm);

  printk(KERN_INFO "task-struct-cdev: ==================\n");

  return count;
}

/* llseek function */
static loff_t custom_cdev_llseek(struct file *flip, loff_t count, int whence)
{
  /* =========================
   * Insert payload code here. 
   * ========================= */

  return count;
}

/* write function */
static ssize_t custom_cdev_write(struct file *flip, const char __user *buf, size_t count, loff_t *f_pos)
{
  /* =========================
   * Insert payload code here. 
   * ========================= */

  return count;
}

/* File operations structure */
static struct file_operations custom_cdev_fops = {
  .owner = THIS_MODULE, /* used to prevent unload while operations are used */
  .open = custom_cdev_open,
  .release = custom_cdev_release,
  .read = custom_cdev_read,
  .write = custom_cdev_write,
  .llseek = custom_cdev_llseek,
};

/* cdev registration function */
static void custom_cdev_setup_cdev(struct custom_cdev_dev *dev, int index)
{
  int err;
  int devno = MKDEV(custom_cdev_major, custom_cdev_minor + index);

  cdev_init(&dev->cdev, &custom_cdev_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &custom_cdev_fops;
  err = cdev_add(&dev->cdev, devno, 1);
  if (err)
    printk(KERN_NOTICE 
        "task-struct-cdev: Error %d while adding task-struct-cdev cde %d", 
        err, index);
}

/* exit function */
static void __exit custom_cdev_exit(void)
{
  int i;
  dev_t devno = MKDEV(custom_cdev_major, custom_cdev_minor);

  /* remove char devices */
  if (custom_cdev_devices) {
    for (i = 0; i < custom_cdev_nr_devs; i++) {
      cdev_del(&custom_cdev_devices[i].cdev);
    }
    kfree(custom_cdev_devices);
  }

  /* We are sure register suceeded because module fails at init if otherwise */
  unregister_chrdev_region(devno, custom_cdev_nr_devs);
  printk(KERN_INFO "task-struct-cdev: Unloaded module.\n");
}

/* init function */
static int __init custom_cdev_init(void)
{
  int result, i;
  dev_t dev = 0;

  /* Allocate major and minor */
  if (custom_cdev_major) {
    dev = MKDEV(custom_cdev_major, custom_cdev_minor);
    result = register_chrdev_region(dev, custom_cdev_nr_devs, 
        "task-struct-cdev");
  } else {
    result = alloc_chrdev_region(&dev, custom_cdev_minor, custom_cdev_nr_devs, 
        "task-struct-cdev");
    custom_cdev_major = MAJOR(dev);
  }
  if (result < 0)
    printk(KERN_WARNING 
        "task-struct-cdev: Can't get major %d.\n", 
        custom_cdev_major);
  else
    printk(KERN_INFO 
        "task-struct-cdev: Got major %d.\n", 
        custom_cdev_major);

  /* allocate devices */
  custom_cdev_devices = kmalloc(custom_cdev_nr_devs * 
      sizeof(struct custom_cdev_dev), GFP_KERNEL);
  if (!custom_cdev_devices) {
    result = -ENOMEM;
    goto fail;
  }
  memset(custom_cdev_devices, 0, 
      custom_cdev_nr_devs * sizeof(struct custom_cdev_dev));

  /* init each device */
  for (i = 0; i < custom_cdev_nr_devs; i++) {
    custom_cdev_setup_cdev(&custom_cdev_devices[i], i);
  }

  printk(KERN_NOTICE 
      "task-struct-cdev: Initialized %d task-struct-cdev devices.\n", 
      custom_cdev_nr_devs);
  return 0;

fail:
  custom_cdev_exit();
  return result;
}

module_init(custom_cdev_init);
module_exit(custom_cdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arthur Samuelyan <artur.samuelyan@phystech.edu>");
