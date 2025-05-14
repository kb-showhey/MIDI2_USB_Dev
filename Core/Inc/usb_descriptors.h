
#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

//from https://midi2-dev.github.io/usbMIDI2DescriptorBuilder/

uint8_t const desc_device[] = {
	0x12,	// bLength
	0x01,	// bDescriptorType = TUSB_DESC_DEVICE
	0x00,	// bcdUSBLSB
	0x02,	// bcdUSBMSB
	0xEF,	// bDeviceClass = TUSB_CLASS_MISC
	0x02,	// bDeviceSubClass = TUSB_CLASS_MISC
	0x01,	// bDeviceProtocol = MISC_PROTOCOL_IAD
	0x40,	// bMaxPacketSize0
	0x66,	// idVendorLSB
	0x66,	// idVendorMSB
	0x66,	// idProductLSB
	0x66,	// idProductMSB
	0x00,	// bcdDeviceLSB
	0x40,	// bcdDeviceMSB
	0x01,	// iManufacturer  "ACME Enterprises"
	0x02,	// iProduct  "ACME Synth"
	0x03,	// iSerialNumber - "abcd1234"
	0x01	// bNumConfigurations
};

uint8_t const desc_device_qualifier[] = {
	0x0A,	// bLength
	0x06,	// bDescriptorType = TUSB_DESC_DEVICE_QUALIFIER
	0x00,	// bcdUSBLSB
	0x02,	// bcdUSBMSB
	0xEF,	// bDeviceClass = TUSB_CLASS_MISC
	0x02,	// bDeviceSubClass = TUSB_CLASS_MISC
	0x01,	// bDeviceProtocol = MISC_PROTOCOL_IAD
	0x40,	// bMaxPacketSize0
	0x01,	// bNumConfigurations
	0x00	// bReserved
};

uint8_t const desc_fs_configuration[] = {
	0x09,	// bLength
	0x02,	// bDescriptorType = CONFIGURATION
	0x95,	// Total LengthLSB
	0x00,	// Total LengthMSB
	0x02,	// bNumInterfaces
	0x01,	// bConfigurationValue
	0x00,	// iConfiguration
	0x80,	// bmAttributes
	0x7D,	// bMaxPower (250mA)
	
  // ---------------------------
	
  // Interface Association Descriptor
	0x08,	// bLength
	0x0B,	// bDescriptorType
	0x00,	// bFirstInterface
	0x02,	// bInterfaceCount
	0x01,	// bFunctionClass
	0x03,	// bFunctionSubClass
	0x00,	// bFunctionProtocol
	0x00,	// iFunction
	
  // Interface - Audio Control
	0x09,	// bLength
	0x04,	// bDescriptorType = INTERFACE
	0x00,	// bInterfaceNumber
	0x00,	// bAlternateSetting
	0x00,	// bNumEndpoints
	0x01,	// bInterfaceClass = AUDIO
	0x01,	// bInterfaceSubClass = AUDIO_CONTROL
	0x00,	// bInterfaceProtocol
	0x00,	// iInterface
	
  // Audio AC Descriptor - Header
	0x09,	// bLength
	0x24,	// bDescriptorType = CS_INTERFACE
	0x01,	// bDescriptorSubtype = HEADER
	0x00,	// bcdACD0
	0x01,	// bcdACD1
	0x09,	// wTotalLengthLSB
	0x00,	// wTotalLengthMSB
	0x01,	// bInCollection
	0x01,	// baInterfaceNr(1)
	
  // Interface - MIDIStreaming - Alternate Setting #0
	0x09,	// bLength
	0x04,	// bDescriptorType = INTERFACE
	0x01,	// bInterfaceNumber
	0x00,	// bAlternateSetting
	0x02,	// bNumEndpoints
	0x01,	// bInterfaceClass = AUDIO
	0x03,	// bInterfaceSubClass = MIDISTREAMING
	0x00,	// bInterfaceProtocol
	0x02,	// iInterface - "ACME Synth"
	
  // Audio MS Descriptor - CS Interface - MS Header
	0x07,	// bLength
	0x24,	// bDescriptorType = CS_INTERFACE
	0x01,	// bDescriptorSubtype = MS_HEADER
	0x00,	// bcdMSCLSB
	0x01,	// bcdMSCMSB
	0x41,	// wTotalLengthLSB
	0x00,	// wTotalLengthMSB
	
  // Audio MS Descriptor - CS Interface - MIDI IN Jack (EMB) (Main In)
	0x06,	// bLength
	0x24,	// bDescriptorType = CS_INTERFACE
	0x02,	// bDescriptorSubtype = MIDI_IN_JACK
	0x01,	// bJackType = EMBEDDED
	0x01,	// bJackID (string = "MonoSynth")
	0x04,	// iJack - "MonoSynth"
	
  // Audio MS Descriptor - CS Interface - MIDI OUT Jack (EXT) (Main Out)
	0x09,	// bLength
	0x24,	// bDescriptorType = CS_INTERFACE
	0x03,	// bDescriptorSubtype = MIDI_OUT_JACK
	0x02,	// bJackType = EXTERNAL
	0x01,	// bJackID for external (string = "MonoSynth")
	0x01,	// bNrInputPins
	0x01,	// baSourceID = Embedded bJackId (string = "MonoSynth")
	0x01,	// baSourcePin
	0x04,	// iJack - "MonoSynth"
	
  // Audio MS Descriptor - CS Interface - MIDI IN Jack (EXT) (Main In)
	0x06,	// bLength
	0x24,	// bDescriptorType = CS_INTERFACE
	0x02,	// bDescriptorSubtype = MIDI_IN_JACK
	0x02,	// bJackType = EXTERNAL
	0x02,	// bJackID for external (string = "MonoSynth")
	0x04,	// iJack - "MonoSynth"
	
  // Audio MS Descriptor - CS Interface - MIDI OUT Jack (EMB) (Main Out)
	0x09,	// bLength
	0x24,	// bDescriptorType
	0x03,	// bDescriptorSubtype
	0x01,	// bJackType
	0x12,	// bJackID (string = "MonoSynth")
	0x01,	// Number of Input Pins of this Jack
	0x12,	// baSourceID (string = "MonoSynth")
	0x01,	// baSourcePin
	0x04,	// iJack - "MonoSynth"
	
  // EP Descriptor - Endpoint - MIDI OUT
	0x09,	// bLength
	0x05,	// bDescriptorType = ENDPOINT
	0x03,	// bEndpointAddress (OUT)
	0x02,	// bmAttributes
	0x40,	// wMaxPacketSizeLSB
	0x00,	// wMaxPacketSizeMSB
	0x00,	// bInterval
	0x00,	// bRefresh
	0x00,	// bSynchAddress
	
  // Audio MS Descriptor - CS Endpoint - EP General
	0x05,	// bLength
	0x25,	// bDescriptorType = CS_ENDPOINT
	0x01,	// bDescriptorSubtype = MS_GENERAL
	0x01,	// bNumEmbMIDJack
	0x01,	// Jack Id - Embedded MIDI in (string="MonoSynth")
	
  // EP Descriptor - Endpoint - MIDI IN
	0x09,	// bLength
	0x05,	// bDescriptorType = ENDPOINT
	0x83,	// bEndpointAddress (IN)
	0x02,	// bmAttributes
	0x40,	// wMaxPacketSizeLSB
	0x00,	// wMaxPacketSizeMSB
	0x00,	// bInterval
	0x00,	// bRefresh
	0x00,	// bSynchAddress
	
  // Audio MS Descriptor - CS Endpoint - MS General
	0x05,	// bLength
	0x25,	// bDescriptorType = CS_ENDPOINT
	0x01,	// bDescriptorSubtype = MS_GENERAL
	0x01,	// bNumEmbMIDJack
	0x12,	// Jack Id - Embedded MIDI Out (string = "MonoSynth")
	
  // Interface - MIDIStreaming - Alternate Setting #1
	0x09,	// bLength
	0x04,	// bDescriptorType = INTERFACE
	0x01,	// bInterfaceNumber
	0x01,	// bAlternateSetting
	0x02,	// bNumEndpoints
	0x01,	// bInterfaceClass = AUDIO
	0x03,	// bInterfaceSubClass = MIDISTREAMING
	0x00,	//  bInterfaceProtocol
	0x02,	// iInterface - "ACME Synth"
	
  // Audio MS Descriptor - CS Interface - MS Header
	0x07,	// bLength
	0x24,	// bDescriptorType = CS_INTERFACE
	0x01,	// bDescriptorSubtype = MS_HEADER
	0x00,	// bcdMSC_LSB
	0x02,	// bcdMSC_MSB
	0x07,	// wTotalLengthLSB
	0x00,	// wTotalLengthMSB
	
  // EP Descriptor - Endpoint - MIDI OUT
	0x07,	// bLength
	0x05,	// bDescriptorType = ENDPOINT
	0x03,	// bEndpointAddress (OUT)
	0x02,	// bmAttributes
	0x40,	// wMaxPacketSizeLSB
	0x00,	// wMaxPacketSizeMSB
	0x00,	// bInterval
	
  // Audio MS Descriptor - CS Endpoint - MS General 2.0
	0x05,	// bLength
	0x25,	// bDescriptorType = CS_ENDPOINT
	0x02,	// bDescriptorSubtype = MS_GENERAL_2_0
	0x01,	// bNumGrpTrmBlock
	0x01,	// baAssoGrpTrmBlkID
	
  // EP Descriptor - Endpoint - MIDI IN
	0x07,	// bLength
	0x05,	// bDescriptorType = ENDPOINT
	0x83,	// bEndpointAddress (IN)
	0x02,	// bmAttributes
	0x40,	// wMaxPacketSizeLSB
	0x00,	// wMaxPacketSizeMSB
	0x00,	// bInterval
	
  // Audio MS Descriptor - CS Endpoint - MS General 2.0
	0x05,	// bLength
	0x25,	// bDescriptorType = CS_ENDPOINT
	0x02,	// bDescriptorSubtype = MS_GENERAL_2_0
	0x01,	// bNumGrpTrmBlock
	0x01	// baAssoGrpTrmBlkID
};
uint8_t const gtb0[] = {
	0x05,	// HeaderLength
	0x26,	// bDescriptorType = CS_GR_TRM_BLOCK
	0x01,	// bDescriptorSubtype = GR_TRM_BLOCK_HEADER
	0x12,	// wTotalLengthLSB
	0x00,	// wTotalLengthMSB
	0x0D,	// bLength
	0x26,	// bDescriptorType = CS_GR_TRM_BLOCK
	0x02,	// bDescriptorSubtype = GR_TRM_BLOCK
	0x01,	// bGrpTrmBlkID
	0x00,	// birectional
	0x00,	// First Group
	0x10,	// nNumGroupTrm
	0x04,	// iBlockItem - "MonoSynth"
	0x11,	// bMIDIProtocol
	0x00,	// wMaxInputBandwidthLSB
	0x00,	// wMaxInputBandwidthMSB
	0x00,	// wMaxOutputBandwidthLSB
	0x00	// wMaxOutputBandwidthMSB
};
uint8_t const gtbLengths[] = {18};
uint8_t const epInterface[] = {1};
uint8_t const *group_descr[] = {gtb0};
char const* string_desc_arr [] = {
	"", //0
	"ACME Enterprises", //1
	"ACME Synth", //2
	"abcd1234", //3
	"MonoSynth", //4
};
uint8_t const string_desc_arr_length = 5;

#endif // USB_DESCRIPTORS_H