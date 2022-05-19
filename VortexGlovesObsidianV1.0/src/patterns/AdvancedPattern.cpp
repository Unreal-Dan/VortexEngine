#include "AdvancedPattern.h"

#include "../TimeControl.h"
#include "../LedControl.h"
#include "../Colorset.h"
#include "../Log.h"

AdvancedPattern::AdvancedPattern(uint32_t onDuration, uint32_t offDuration, uint32_t gapDuration,
                                 uint32_t groupSize, uint32_t skipCols, uint32_t repeatGroup) :
  GapPattern(onDuration, offDuration, gapDuration),
  m_groupSize(groupSize),
  m_skipCols(skipCols),
  m_repeatGroup(repeatGroup),
  m_groupCounter(0),
  m_repeatCounter(repeatGroup)
{
}

AdvancedPattern::~AdvancedPattern()
{
}

void AdvancedPattern::init(Colorset *colorset, LedPos pos)
{
  if (!m_groupSize || m_groupSize > colorset->numColors()) {
    m_groupSize = colorset->numColors();
  }
  m_groupCounter = 0;
  m_repeatCounter = m_repeatGroup;
  // run base pattern init logic
  GapPattern::init(colorset, pos);
}

void AdvancedPattern::play()
{
  // the advanced pattern is just a gap pattern but
  // with some of the callbacks overridden to perform
  // actions at certain times in the pattern
  GapPattern::play();
}

void AdvancedPattern::triggerGap()
{
  // This is an override from GapPattern::triggerGap()
  // When the gap triggers in the gap pattern we need to 
  // reset the group counter in the advanced pattern
  // because the only way for the gap to trigger is via
  // the group counter logic in endGap
  GapPattern::triggerGap();
  m_groupCounter = 0;
}

void AdvancedPattern::endGap()
{
  // This is an override for the GapPattern callback endGap()
  GapPattern::endGap();
  // Here we perform logic for repeating groups
  if (m_repeatCounter > 0) {
    // the repeat counter starts at group size and counts down
    // each time an entire group has been displayed
    m_repeatCounter--;
    // to "repeat" we simply move the colorset back one group size
    m_pColorset->skip(-(int32_t)m_groupSize);
    // nothing more to do
    return;
  }
  if (m_skipCols > 0) {
    m_pColorset->skip(m_skipCols);
  }
  if (!m_repeatCounter) {
    m_repeatCounter = m_repeatGroup;
  }
}

void AdvancedPattern::onBasicEnd()
{
  // This is overridding GapPattern::onBasicEnd which itself is
  // an override of BasicPatterns onBasicEnd callback. This is
  // so that we don't run GapPattern::onBasicEnd to prevent default 
  // gap logic at end of blinks because we will be inserting gaps
  // at different locations based on the group size
  BasicPattern::onBasicEnd();
}

void AdvancedPattern::onBlinkOff()
{
  BasicPattern::onBlinkOff();
  // count a blink in the group
  m_groupCounter++;
  // check if the group has reached the intended size
  if (m_groupCounter >= m_groupSize) {
    triggerGap();
  }
}

void AdvancedPattern::serialize() const
{
  GapPattern::serialize();
  Serial.print(m_onDuration);
  Serial.print(m_offDuration);
}

void AdvancedPattern::unserialize()
{
}