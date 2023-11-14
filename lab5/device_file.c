#include "device_file.h"
#include <linux/fs.h> 	     /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <asm/uaccess.h>  /* copy_to_user() */

static const char    g_s_Hello_World_string[] = "Hello world from kernel mode! 1234567890abcdefghijklmnopqrstuvwxyz";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);

/*===============================================================================================*/
// static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer,
//    size_t count, loff_t *possition){
//    printk(KERN_NOTICE "Simple-driver: Device file is read at offset = %i, read bytes count = %u"
//             , (int)*possition //偏移量
//             , (unsigned int)count); //读取的字节数

//    if(*possition >= g_s_Hello_World_size)
//       return 0;

//    if(*possition + count > g_s_Hello_World_size)
//       count = g_s_Hello_World_size - *possition;

//    if(copy_to_user(user_buffer, g_s_Hello_World_string + *possition, count) != 0)
//       return -EFAULT;   

//    *possition += count;
//    return count;
// }



static struct file *file = NULL;

static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer,
   size_t count, loff_t *possition) {
   
   printk(KERN_NOTICE "Simple-driver: Device file is read at offset = %i, read bytes count = %u"
            , (int)*possition
            , (unsigned int)count);

   if (!file) {
      // 打开文件
      file = filp_open("/mnt/debug/Test/lab4/src/data.txt", O_RDONLY, 0);
      if (IS_ERR(file)) {
         printk(KERN_ERR "Simple-driver: Failed to open file: %ld", PTR_ERR(file));
         return -EFAULT;
      }
   }

   // 从文件中读取数据
   loff_t pos = *possition;
   mm_segment_t old_fs = get_fs();
   set_fs(get_ds());

   ssize_t bytesRead = vfs_read(file, user_buffer, count, &pos);
   

   set_fs(old_fs);

   if (bytesRead < 0) {
      printk(KERN_ERR "Simple-driver: Failed to read from file: %ld", bytesRead);
      return -EFAULT;
   }

   *possition += bytesRead;

   return bytesRead;
}
/*===============================================================================================*/
static struct file_operations simple_driver_fops = {
   .owner   = THIS_MODULE,
   .read    = device_file_read,
};

static int device_file_major_number = 202;

static const char device_name[] = "Simple-driver";

/*===============================================================================================*/
int register_device(void) {
      int result = 0;

      printk(KERN_NOTICE "Simple-driver: register_device() is called.");

      result = register_chrdev(device_file_major_number, device_name, &simple_driver_fops);
      if(result < 0){
         printk(KERN_WARNING "Simple-driver:  can\'t register char device with errorcode = %i", result);
         return result;
      }

      device_file_major_number = result;
      printk(KERN_NOTICE "Simple-driver: registered char device with major number = %i\n"
                  , device_file_major_number );

      return 0;
}

/*-----------------------------------------------------------------------------------------------*/
void unregister_device(void) {
   printk(KERN_NOTICE "Simple-driver: unregister_device() is called");
   if(device_file_major_number != 0){
      unregister_chrdev(device_file_major_number, device_name);
   }
}
