#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "hello_char_device"
#define CLASS_NAME "hello_class"

static int major_number;
static struct class* example_class = NULL;
static struct device* example_device = NULL;
static struct cdev example_cdev;

// Function prototypes
static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

// File operations structure
static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init example_init(void) {
    printk(KERN_INFO "HelloDevice: Initializing the Example LKM\n");

    // Dynamically allocate a major number
    if (alloc_chrdev_region(&major_number, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "HelloDevice failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "HelloDevice: registered correctly with major number %d\n", MAJOR(major_number));

    // Register the device class
    example_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(example_class)) {
        unregister_chrdev_region(major_number, 1);
        printk(KERN_ALERT "HelloDevice: Failed to register device class\n");
        return PTR_ERR(example_class);
    }
    printk(KERN_INFO "HelloDevice: device class registered correctly\n");

    // Register the device driver
    example_device = device_create(example_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(example_device)) {
        class_destroy(example_class);
        unregister_chrdev_region(major_number, 1);
        printk(KERN_ALERT "HelloDevice: Failed to create the device\n");
        return PTR_ERR(example_device);
    }
    printk(KERN_INFO "HelloDevice: device class created correctly\n");

    // Initialize the cdev structure and add it to the kernel
    cdev_init(&example_cdev, &fops);
    if (cdev_add(&example_cdev, MKDEV(major_number, 0), 1) < 0) {
        device_destroy(example_class, MKDEV(major_number, 0));
        class_destroy(example_class);
        unregister_chrdev_region(major_number, 1);
        printk(KERN_ALERT "HelloDevice: Failed to add cdev\n");
        return -1;
    }

    return 0;
}

static void __exit example_exit(void) {
    cdev_del(&example_cdev);
    device_destroy(example_class, MKDEV(major_number, 0));
    class_destroy(example_class);
    unregister_chrdev_region(major_number, 1);
    printk(KERN_INFO "HelloDevice: Goodbye from the LKM!\n");
}

static int dev_open(struct inode* inodep, struct file* filep) {
    printk(KERN_INFO "HelloDevice: Device has been opened\n");
    return 0;
}

static int dev_release(struct inode* inodep, struct file* filep) {
    printk(KERN_INFO "HelloDevice: Device successfully closed\n");
    return 0;
}

static ssize_t dev_read(struct file* filep, char* buffer, size_t len, loff_t* offset) {
    printk(KERN_INFO "HelloDevice: Device read\n");
    return 0;
}

static ssize_t dev_write(struct file* filep, const char* buffer, size_t len, loff_t* offset) {
    printk(KERN_INFO "HelloDevice: Device write\n");
    return len;
}

module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("DevOats");
MODULE_DESCRIPTION("A hello world character device driver");
MODULE_VERSION("0.1");
