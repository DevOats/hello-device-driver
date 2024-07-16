#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>

dev_t dev = 0;
static struct class *dev_class;

static int __init hello_init(void){

   /*Allocating Major number*/
     if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
             pr_err("Cannot allocate major number for device\n");
             return -1;
     }
     pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

     /*Creating struct class*/
     dev_class = class_create(THIS_MODULE,"etx_class");
     if(IS_ERR(dev_class)){
         pr_err("Cannot create the struct class for device\n");
         goto r_class;
     }

     /*Creating device*/
     if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
         pr_err("Cannot create the Device\n");
         goto r_device;
     }
   
   printk(KERN_ALERT "Hello world from kernel space! Let's do some driver things:) /\n");
   return 0;

r_device:
   class_destroy(dev_class);
r_class:
   unregister_chrdev_region(dev,1);
   return -1;
}

static void __exit hello_exit(void){
   device_destroy(dev_class,dev);
   class_destroy(dev_class);
   unregister_chrdev_region(dev, 1);
   printk(KERN_INFO "Goodbye cruel world from kernel space.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("DevOats");
MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("DevOats kernel driver example.");
