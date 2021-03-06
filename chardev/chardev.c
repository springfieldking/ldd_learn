//
// Created by wangchunye on 5/17/17.
//

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

/*
 *  Prototypes - this would normally go in a .h file
 */
int chardev_init(void);
void chardev_cleanup(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int Major;
static int Device_Open = 0;

static char msg[BUF_LEN]={0};
static char *msg_Ptr;

static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release,
};

/*
 * Functions
 */

int chardev_init(void) {
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if(Major < 0) {
        printk("Registering the character device failed with %d\n", Major);
        return Major;
    }

    printk("<1>I was assigned major number %d.  To talk to\n", Major);
    printk("<1>the driver, create a dev file with\n");
    printk("'mknod /dev/hello c %d 0'.\n", Major);
    printk("<1>Try various minor numbers.  Try to cat and echo to\n");
    printk("the device file.\n");
    printk("<1>Remove the device file and module when done.\n");

    return 0;
}

void chardev_cleanup(void) {
    unregister_chrdev(Major, DEVICE_NAME);
    return;
}

/*
 * Methods
 */
static int device_open(struct inode *inode, struct file *file) {
    static int counter = 0;
    if(Device_Open)
        return -EBUSY;
    Device_Open ++;
    sprintf(msg, "I already told you %d times Hello world!\n", counter++);
    msg_Ptr = msg;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file) {
    Device_Open --;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t
device_read(struct file *filep, char *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;

    if(*msg_Ptr == 0)
        return 0;

    while(length && *msg_Ptr) {
        put_user(*(msg_Ptr++), buffer++);
        length --;
        bytes_read++;
    }
    return bytes_read;
}

static ssize_t
device_write(struct file *filp, const char * buff, size_t len, loff_t *off) {
    printk("<1>Sorry, this operation isn't supported.\n");
    return -EINVAL;
}


module_init(chardev_init);
module_exit(chardev_cleanup);

MODULE_AUTHOR("wangchunye");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple char device");
MODULE_ALIAS("simple char device");



