
#include "midi_proc.h"
#include "bytestreamToUMP.h"
#include "umpToBytestream.h"
#include "umpToMIDI2Protocol.h"
#include "umpMessageCreate.h"

uint8_t UartMidiData[UART_MIDI_BUFFER_SIZE] = {0};

static CReadMidiBuffer ReadMidiBuffer(UartMidiData, UART_MIDI_BUFFER_SIZE);
static UART_HandleTypeDef* hmidi_uart;
static CMIDIFIFO MidiFIFO;
static bytestreamToUMP bsToUMP;
static umpToBytestream umptoBS;
static umpToMIDI2Protocol umpToMIDI2;

static MIDIPacket_t midi_buffer;
static uint8_t midi_index = 0;
static uint8_t running_status = 0;

void SetMidiUart(UART_HandleTypeDef *huart)
{
  hmidi_uart = huart;
  ReadMidiBuffer.Init(huart);
}

void UmpTxRxMonior(const char* dir, uint32_t* data, int size)
{
  if (size != 0) {
    printf("%s: ", dir);
    for (int i = 0; i < size; i++) {
      printf("0x%08X ", (unsigned int)data[i]);
    }
    printf("\r\n");
  }
}

void EndpointInfoNotification()
{
  uint8_t state = 1;
  uint8_t numOfFuncBlock = 1;
  bool midi2 = true;
  bool midi1 = false;
  bool rxjr = false;
  bool txjr = false;

  std::array<uint32_t, 4> mess = UMPMessage::mtFMidiEndpointInfoNotify( (state<<7) | numOfFuncBlock , midi2 , midi1, rxjr , txjr);
  tud_ump_write(BOARD_TUD_RHPORT, mess.data(), mess.size());
  UmpTxRxMonior("UMP TX", mess.data(),  mess.size());
}

void FunctionBlockInfoNotification()
{
  uint8_t fbIdx = 0;
  bool active = true;
  uint8_t direction = 2;//bidirectional
  bool sender = true;
  bool recv = false;
  uint8_t firstGroup = 0;
  uint8_t groupLength = 1;
  uint8_t midiCISupport = 0;
  uint8_t isMIDI1 = false;
  uint8_t maxS8Streams = 1;

  std::array<uint32_t, 4> mess = UMPMessage::mtFFunctionBlockInfoNotify(fbIdx, active, direction, sender, recv, firstGroup, groupLength, midiCISupport, isMIDI1, maxS8Streams);
  tud_ump_write(BOARD_TUD_RHPORT, mess.data(), mess.size());
  UmpTxRxMonior("UMP TX", mess.data(),  mess.size());
}

void ReceiveUmpData(uint32_t* UMP, int size)
{
  // Convert the UMP message to MIDI 1.0 format
  umptoBS.resetBuffer();
  for (int i = 0; i < size; i++)
  {
    umptoBS.UMPStreamParse(UMP[i]);
  }

  // Convert and send the MIDI 1.0 message to byte stream
  while(umptoBS.availableBS()){
    uint8_t midiMessage = umptoBS.readBS();
    HAL_UART_Transmit(hmidi_uart, &midiMessage, 1, HAL_MAX_DELAY);
  } 

  //Debug print
  UmpTxRxMonior("UMP RX", UMP, size);
 
  // Check if the first UMP message is an Endpoint Discovery message
  uint8_t messageType = UMP[0] >> 28;
  if(messageType == 0xF){
    uint16_t status = (UMP[0] >> 16) & 0x3FF;
    if( status == MIDIENDPOINT ){
      //Endpoint Discovery
      EndpointInfoNotification();
    }else if ( status == FUNCTIONBLOCK ){
      //Function Block Discovery
      FunctionBlockInfoNotification();
    }
  }

}

void SendUsbUmp(MIDIPacket_t& packet)
{
  // Convert the MIDI byte message to UMP format(MIDI1.0 Protocol)
  bsToUMP.resetBuffer();
  for(auto&& byte : packet.byte){
    bsToUMP.bytestreamParse(byte);
  }

  // Convert the UMP message to MIDI 2.0 format
  umpToMIDI2.UMPStreamParse(bsToUMP.readUMP());
  
  // Send the MIDI2.0 message to USB Host
  uint32_t UMP[4];
  int size_index = 0;
  while(umpToMIDI2.availableUMP()){
    UMP[size_index++] = umpToMIDI2.readUMP();
  }

  if(size_index != 0){
    tud_ump_write(BOARD_TUD_RHPORT, UMP, size_index);
  }

  //Debug print
  UmpTxRxMonior("UMP TX", UMP, size_index);

}

// This function is called from HAL_TIM_PeriodElapsedCallback
void SendUmpData()
{
  MIDIPacket_t packet;
  if(MidiFIFO.dequeue(packet) != false){
     SendUsbUmp(packet);
  }
}

// This function is called from HAL_TIM_PeriodElapsedCallback
bool ProcessMidiData()
{
  uint8_t midi_byte = 0;
  while (ReadMidiBuffer.ReadSerial(midi_byte) > 0) {

    // Prevent buffer overflow
    if (midi_index >= 3) {
      midi_index = 0;
    }

    // Check for status byte
    if (midi_byte & 0x80) {
      // Status byte
      midi_buffer.byte[0] = midi_byte;
      midi_index = 1;
      running_status = midi_byte;
    } else {
      // Data byte
      if (midi_index == 0) {
        // Use running status
        midi_buffer.byte[0] = running_status;
        midi_index = 1;
      }
      midi_buffer.byte[midi_index++] = midi_byte;
    }

    switch (midi_buffer.byte[0] & 0xF0) {
      case NOTE_ON:
      case NOTE_OFF:
      case CC:
      case PITCH_BEND:
        if (midi_index == 3) {
          MIDIPacket_t packet(midi_buffer);
          MidiFIFO.enqueue(packet);
          midi_index = 0;
        }
      break;
      default:
      break;
    }
  }

  return ReadMidiBuffer.GetDataMonitor();
}

