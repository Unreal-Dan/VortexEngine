#include "Storage.h"

#include <FlashStorage.h>
#include <stdlib.h>

#include "SerialBuffer.h"
#include "Log.h"

__attribute__((__aligned__(256)))
static const uint8_t _storagedata[(STORAGE_SIZE+255)/256*256] = { };

FlashClass storage(_storagedata, STORAGE_SIZE);

Storage::Storage()
{
}

Storage::~Storage()
{
}

// store a serial buffer to storage
bool Storage::write(SerialBuffer &buffer)
{
  if (buffer.size() > STORAGE_SIZE) {
    DEBUG("ERROR buffer too big");
    return false;
  }
  // hopefully the flash storage is large enough
  SerialBuffer realBuffer(STORAGE_SIZE);
  realBuffer.serialize(buffer.size());
  realBuffer.append(buffer);
  storage.erase();
  storage.write(_storagedata, (void *)realBuffer.data(), realBuffer.size());
  DEBUGF("Wrote %u bytes to storage", realBuffer.capacity());
  return true;
}

// read a serial buffer from storage
bool Storage::read(SerialBuffer &buffer)
{
  buffer.init(STORAGE_SIZE);
  storage.read(buffer.m_pBuffer);
  buffer.m_size = *(uint32_t *)buffer.m_pBuffer;
  if (!buffer.m_size) {
    return false;
  }
  buffer.m_position += sizeof(uint32_t);
  DEBUGF("Read %u bytes from storage", buffer.m_size);
  return true;
}