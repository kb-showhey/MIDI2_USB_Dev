/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

 #include "bsp/board_api.h"
 #include "tusb.h"
 
 /* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
  * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
  *
  * Auto ProductID layout's Bitmap:
  *   [MSB]         HID | MSC | CDC          [LSB]
  */
 #define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
 #define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                            _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )
 
 //--------------------------------------------------------------------+
 // Device Descriptors
 //--------------------------------------------------------------------+

 #include "usb_descriptors.h"
 
 // Invoked when received GET DEVICE DESCRIPTOR
 // Application return pointer to descriptor
 uint8_t const * tud_descriptor_device_cb(void)
 {
   return (uint8_t const *) &desc_device;
 }

 
 //--------------------------------------------------------------------+
 // Configuration Descriptor
 //--------------------------------------------------------------------+
 
 enum
 {
   ITF_NUM_MIDI = 0,
   ITF_NUM_MIDI_STREAMING,
   ITF_NUM_TOTAL
 };
 
 #define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_MIDI_DESC_LEN)
 
 #if CFG_TUSB_MCU == OPT_MCU_LPC175X_6X || CFG_TUSB_MCU == OPT_MCU_LPC177X_8X || CFG_TUSB_MCU == OPT_MCU_LPC40XX
   // LPC 17xx and 40xx endpoint type (bulk/interrupt/iso) are fixed by its number
   // 0 control, 1 In, 2 Bulk, 3 Iso, 4 In etc ...
   #define EPNUM_MIDI_OUT   0x02
   #define EPNUM_MIDI_IN   0x02
 #elif CFG_TUSB_MCU == OPT_MCU_FT90X || CFG_TUSB_MCU == OPT_MCU_FT93X
   // On Bridgetek FT9xx endpoint numbers must be unique...
   #define EPNUM_MIDI_OUT   0x02
   #define EPNUM_MIDI_IN   0x03
 #else
   #define EPNUM_MIDI_OUT   0x01
   #define EPNUM_MIDI_IN   0x01
 #endif

 #if TUD_OPT_HIGH_SPEED
 uint8_t const desc_hs_configuration[] =
 {
   // Config number, interface count, string index, total length, attribute, power in mA
   TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
 
   // Interface number, string index, EP Out & EP In address, EP size
   TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 0, EPNUM_MIDI_OUT, (0x80 | EPNUM_MIDI_IN), 512)
 };
 #endif
 
 // Invoked when received GET CONFIGURATION DESCRIPTOR
 // Application return pointer to descriptor
 // Descriptor contents must exist long enough for transfer to complete
 uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
 {
   (void) index; // for multiple configurations
 
 #if TUD_OPT_HIGH_SPEED
   // Although we are highspeed, host may be fullspeed.
   return (tud_speed_get() == TUSB_SPEED_HIGH) ?  desc_hs_configuration : desc_fs_configuration;
 #else
   return desc_fs_configuration;
 #endif
 }

 
 uint8_t const* tud_descriptor_device_qualifier_cb(void)
 {
  return (uint8_t const*) &desc_device_qualifier;
 }
 
 //--------------------------------------------------------------------+
 // String Descriptors
 //--------------------------------------------------------------------+
 
 // String Descriptor Index
 enum {
   STRID_LANGID = 0,
   STRID_MANUFACTURER,
   STRID_PRODUCT,
   STRID_SERIAL,
 };
 
 static uint16_t _desc_str[32 + 1];
 
 // Invoked when received GET STRING DESCRIPTOR request
 // Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
 uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
   (void) langid;
   size_t chr_count;
 
   switch ( index ) {
     case STRID_LANGID:
       memcpy(&_desc_str[1], string_desc_arr[0], 2);
       chr_count = 1;
       break;
 
     case STRID_SERIAL:
       chr_count = board_usb_get_serial(_desc_str + 1, 32);
       break;
 
     default:
       // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
       // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
 
       if ( !(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) ) return NULL;
 
       const char *str = string_desc_arr[index];
 
       // Cap at max char
       chr_count = strlen(str);
       size_t const max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
       if ( chr_count > max_count ) chr_count = max_count;
 
       // Convert ASCII string into UTF-16
       for ( size_t i = 0; i < chr_count; i++ ) {
         _desc_str[1 + i] = str[i];
       }
       break;
   }
 
   // first byte is length (including header), second byte is string type
   _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));
 
   return _desc_str;
 }
 
 //--------------------------------------------------------------------+
// Application Callback API Implementations
//--------------------------------------------------------------------+

// Invoked when audio class specific get request received for an interface
bool tud_ump_get_req_itf_cb(uint8_t rhport, tusb_control_request_t const * p_request)
{
  uint16_t length = p_request->wLength;
  if ( length > sizeof( group_descr ) )
  {
    length = sizeof( group_descr );
  }
  tud_control_xfer(rhport, p_request, (void *)group_descr, length );
  return true;
}

