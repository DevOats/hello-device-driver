#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>

static int hello_init(void){
   printk(KERN_ALERT "Hello world from kernel space! Let's do some driver things:) /\n");
   return 0;
}

static void hello_exit(void){
   printk(KERN_INFO "Goodbye cruel world from kernel space.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("DevOats");
MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("DevOats kernel driver example.");
