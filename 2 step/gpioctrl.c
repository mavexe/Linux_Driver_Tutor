#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>

static struct gpio_desc *led, *button;

#define IO_LED 21
#define IO_BUTTON 20

#define IO_OFFSET 0

static int __init my_init(void)
{
  int status;
  
  led = gpiod_to_desc(IO_LED+IO_OFFSET);
  if(!led) {
    printk(KERN_INFO "gpioctrl - erro getting pin 21\n");
    return -ENODEV;
  }
  button = gpiod_to_desc(IO_BUTTON+IO_OFFSET);
  if(!button){
    printk(KERN_INFO "gpioctrl error getting pin 20\n");
  }

  status = gpio_direction_output(led,0);
  if(status) {
    printk (KERN_INFO "error output\n");
    return status;
  }
  gpiod_set_value(led,1);
  printk (KERN_INFO "button pressed\n", gpiod_get_value(button)?"":"not");

  return 0;
}
static void __exit my_exit(void){

  gpiod_set_value(led,0);
}

MODULE_LICENSE ("GPL");
MODULE_AUTHOR("damir");
