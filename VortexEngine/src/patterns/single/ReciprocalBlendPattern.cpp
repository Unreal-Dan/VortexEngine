#include "ReciprocalBlendPattern.h"

#include "../../Colorset.h"
#include "../../Leds.h"

ReciprocalBlendPattern::ReciprocalBlendPattern(uint8_t onDuration, uint8_t offDuration, uint8_t blendSpeed) :
  BlendPattern(onDuration, offDuration, blendSpeed),
  m_showingComplement(false)
{
}

ReciprocalBlendPattern::~ReciprocalBlendPattern()
{
}

void ReciprocalBlendPattern::init()
{
  BlendPattern::init();
  m_showingComplement = false;
}

void ReciprocalBlendPattern::serialize(SerialBuffer &buffer) const
{
  //DEBUG_LOG("Serialize");
  BlendPattern::serialize(buffer);
}

void ReciprocalBlendPattern::unserialize(SerialBuffer &buffer)
{
  //DEBUG_LOG("Unserialize");
  BlendPattern::unserialize(buffer);
}

void ReciprocalBlendPattern::onBlinkOn()
{
  // every other tick show the complement color
  m_showingComplement = !m_showingComplement;
  if (m_showingComplement) {
    // generate an inverse hue
    Leds::setIndex(m_ledPos, HSVColor((m_cur.hue + 128) % 256, m_cur.sat, m_cur.val));
  } else {
    BlendPattern::onBlinkOn();
  }
}