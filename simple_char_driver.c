

#include<linux/init.h>
#include<linux/module.h>
#include <linux/uaccess.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<asm/uaccess.h>

#define BUFFER_SIZE 1024
MODULE_LICENSE("GPL");
/* Define device_buffer and other global data structures you will need here */
static int major;
char *filebegin;
int countOpen = 0;
int countClose = 0;

//int copy_to_user(void __user, const void*, int);
//int copy_from_user(const void*, void __user, int);

ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	int mBytes;
	int btor;
	printk(KERN_ALERT "Inside function\n");
       	mBytes = BUFFER_SIZE - *offset;
        
        if(mBytes <= length)
        {
                btor = mBytes;
        }
        else
        {
                btor = length;
        }

        if(btor == 0)
        {
                printk(KERN_ALERT "End of divice\n");
        }
 
        //bytesread =btor -
	copy_to_user(buffer, filebegin +*offset, btor);
                
        printk(KERN_ALERT "Divice read: %d bytes of data\n", btor);
        *offset += btor;   
        
        return btor;
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	int mBytes;
	int btow;
	printk(KERN_ALERT "Inside function\n");
        mBytes = BUFFER_SIZE - *offset;
        
        if(mBytes <= length)
        {
                btow = mBytes;
        }
        else
        {
                btow = length;
        }



        //byteswrite = btow - 
	copy_from_user(filebegin +*offset, buffer, btow);
        printk(KERN_ALERT "Succesfully wrote %d bytes\n", btow);
        *offset += btow;

        return btow;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	countOpen++;
	printk(KERN_ALERT "This file has been opened %d times\n", countOpen);
	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	countClose++;
	printk(KERN_ALERT "This file has been closed %d times\n", countClose);
	return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	loff_t newOffset;
	printk(KERN_ALERT "Inside function\n"); 
	newOffset = 0;

        switch(whence)
        {
                case 0:
                        newOffset = offset;
                        break;
                case 1:
                        newOffset = pfile->f_pos + offset;
                        break;
                case 2:
                        newOffset = BUFFER_SIZE - offset;
                        break;

        }


        if(newOffset < 0)
        {
                printk(KERN_ALERT "BIG ERROR BOI Can not seek before the buffer\n");
        }
        else if(newOffset > BUFFER_SIZE)
        {
                printk(KERN_ALERT " BIG ERROR BOI Can not seek past the buffer\n");
        }
        pfile->f_pos = newOffset;

        return newOffset;
}

struct file_operations simple_char_driver_file_operations = {
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.owner = THIS_MODULE,
        .read = simple_char_driver_read,
        .write = simple_char_driver_write,
        .open =  simple_char_driver_open,
        .release = simple_char_driver_close,
        .llseek = simple_char_driver_seek,
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	filebegin = kmalloc(BUFFER_SIZE, GFP_KERNEL);
        printk(KERN_ALERT "Simple init\n");
	memset(filebegin, 0, BUFFER_SIZE);
        major = register_chrdev(0, "simple_char_driver", &simple_char_driver_file_operations);
	printk(KERN_ALERT "Device registerd under: %d\n", major);
	return 0;
}

static void simple_char_driver_exit(void)
{
	kfree(filebegin);

        unregister_chrdev(major, "simple_char_driver");
        printk(KERN_ALERT "Exit function\n");
/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
}

/* add module_init and module_exit to point to the corresponding init and exit function*/

module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);
