/*
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dwc3-uboot.h>
#include <usb.h>
#include <i2c_eeprom.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_USB_DWC3
static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_HIGH,
	.base = 0xfcc00000,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
	.dis_u2_susphy_quirk = 1,
	.usb2_phyif_utmi_width = 16,
};

int usb_gadget_handle_interrupts(void)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	return dwc3_uboot_init(&dwc3_device_data);
}
#endif

static int get_ethaddr_from_eeprom(u8 *addr)
{
        int ret;
        struct udevice *dev;

        ret = uclass_first_device_err(UCLASS_I2C_EEPROM, &dev);
        if (ret)
                return ret;

        return i2c_eeprom_read(dev, 0, addr, 6);
}

int rk3568_board_late_init(void)
{
        u8 ethaddr[6];

        if (get_ethaddr_from_eeprom(ethaddr))
                return 0;

        if (is_valid_ethaddr(ethaddr))
                eth_env_set_enetaddr("ethaddr", ethaddr);

        return 0;
}
