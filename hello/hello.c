//
// Created by wangchunye on 5/17/17.
//

#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void)
{
    printk(KERN_INFO "[init] Can you feel me?\n");
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_INFO "[exit] Yes.\n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("wangchunye");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("A simple module");