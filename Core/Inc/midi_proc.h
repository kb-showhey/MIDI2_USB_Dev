#ifndef MIDI_PROC_H
#define MIDI_PROC_H

#include <stdio.h>
#include "tusb.h"
#include "ump_device.h"
#include "ump.h"
#include "tusb_config.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define UART_MIDI_BUFFER_SIZE 128
extern uint8_t UartMidiData[UART_MIDI_BUFFER_SIZE];

#ifdef __cplusplus

struct MIDIPacket_t {
  public:
    MIDIPacket_t() {
      byte[0] = 0;
      byte[1] = 0;
      byte[2] = 0;
    }
  public:
      uint8_t byte[3];
};

class CMIDIFIFO {
  private:
    static const int FIFO_SIZE = UART_MIDI_BUFFER_SIZE;
    MIDIPacket_t buffer[FIFO_SIZE];
    int head;
    int tail;
    int count;
  
  public:
    CMIDIFIFO() : head(0), tail(0), count(0) {}
    ~CMIDIFIFO() {}
  
  private:
    CMIDIFIFO(const CMIDIFIFO&) = delete;
    CMIDIFIFO& operator=(const CMIDIFIFO&) = delete;
    CMIDIFIFO(CMIDIFIFO&&) = delete;
    CMIDIFIFO& operator=(CMIDIFIFO&&) = delete;
    
  public:
    void clear() {
      head = 0;
      tail = 0;
      count = 0;
    }
  
    bool isFull() const {
      return count == FIFO_SIZE;
    }
  
    bool isEmpty() const {
      return count == 0;
    }
  
    inline bool enqueue(const MIDIPacket_t& packet) {
      if (isFull()) {
        return false;
      }
      buffer[head] = packet;
      head = ((head + 1) % FIFO_SIZE);
      count++;
      return true;
    }
  
    inline bool dequeue(MIDIPacket_t &packet) {
      if (isEmpty()) {
        return false;
      }
      packet = buffer[tail];
      tail = ((tail + 1) % FIFO_SIZE);
      count--;
      return true;
    }
};

class CReadMidiBuffer {
  public:
    CReadMidiBuffer(const uint8_t *uartbuffer, const int buffersize)
     : pMidiData(uartbuffer), mSize(buffersize), mIndexRead(0), mIndexMonitor(0), initialized(false) {}
    ~CReadMidiBuffer() {}

  private:
    CReadMidiBuffer(const CReadMidiBuffer&) = delete;
    CReadMidiBuffer& operator=(const CReadMidiBuffer&) = delete;
    CReadMidiBuffer(CReadMidiBuffer&&) = delete;
    CReadMidiBuffer& operator=(CReadMidiBuffer&&) = delete;
  
  public:
    void Init(UART_HandleTypeDef* huart) {
      hmidi = huart;
      mIndexRead = 0;
      mIndexMonitor = 0;
      initialized = true;
    }

    int ReadSerial(uint8_t &data) {
      if(!initialized) {
        return 0;
      }

      int unread = GetDataNum();
      if (unread > 0) {
        data = UartMidiData[mIndexRead];
        mIndexRead = (mIndexRead + 1) % mSize;
      }
      return unread;
    }

    bool GetDataMonitor() {
      bool state = (mIndexMonitor != mIndexRead) ? true : false;
      mIndexMonitor = mIndexRead;
      return state;
    }
  
  private:
    int GetDataNum() {
      int remaining = hmidi->hdmarx->Instance->NDTR;
      int current = mSize - remaining;
      int unread = current - mIndexRead;
      if (unread < 0)
      {
        unread += mSize;
      }
      return unread;
    }

  private:
    const uint8_t *pMidiData;
    const int mSize;
    
    int mIndexRead;
    int mIndexMonitor;
    bool initialized;

    UART_HandleTypeDef* hmidi;
};
#endif

#ifdef __cplusplus
extern "C"
{
#endif
void SetMidiUart(UART_HandleTypeDef* huart);
bool ProcessMidiData();
void ReceiveUmpData(uint32_t* ump, int size);
void SendUmpData();
#ifdef __cplusplus
}
#endif

#endif // MIDI_PROC_H