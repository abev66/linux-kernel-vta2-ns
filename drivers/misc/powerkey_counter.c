/* drivers/misc/powerkey_counter.c
 *
 * Copyright 2012  Lai, Wei-Chen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/powerkey_counter.h>
#include <linux/input.h>

#define POWERKEY_COUNTER_VERSION	1

static unsigned long counter = 0;

static ssize_t powerkey_counter_version(struct device * dev, struct device_attribute * attr, char * buf)
{
    return sprintf(buf, "%u\n", POWERKEYCOUNTER_VERSION);
}

static ssize_t powerkey_counter_read(struct device * dev, struct device_attribute * attr, char * buf)
{
    return sprintf(buf, "%lu time(s) since boot.\n", counter);
}

static DEVICE_ATTR(version, S_IRUGO , powerkey_counter_version, NULL);
static DEVICE_ATTR(counter, S_IRUGO , powerkey_counter_read, NULL);

static struct attribute *powerkey_counter_attributes[] = 
{
    &dev_attr_version.attr,
    &dev_attr_counter.attr,
    NULL
};
    
static struct attribute_group powerkey_counter_group = 
{
    .attrs  = powerkey_counter_attributes,
};
    
static struct miscdevice powerkey_counter_device = 
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "powerkey_counter",
};

void powerkey_count(void)
{   
    counter++;

    return;
}

EXPORT_SYMBOL(powerkey_count);


static int __init powerkey_counter_init(void)
{
  pr_info("%s misc_register(%s)\n", __FUNCTION__, powerkey_counter_device.name);
  
  if(misc_register(&powerkey_counter_device)){
    pr_err("%s misc_register(%s) fail\n", __FUNCTION__, powerkey_counter_device.name);
    return 1;
  } 
  if (sysfs_create_group(&powerkey_counter_device.this_device->kobj, &powerkey_counter_group) < 0) 
  {
      pr_err("%s sysfs_create_group fail\n", __FUNCTION__);
      pr_err("Failed to create sysfs group for device (%s)!\n", powerkey_counter.name);
  }
  
  return 0;
}

device_initcall(powerkey_counter_init);