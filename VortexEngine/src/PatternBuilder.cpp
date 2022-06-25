#include "PatternBuilder.h"

#include "SerialBuffer.h"
#include "TimeControl.h"

#include "patterns/multi/TheaterChasePattern.h"
#include "patterns/multi/HueShiftPattern.h"
#include "patterns/multi/RabbitPattern.h"

#include "patterns/single/ReciprocalBlendPattern.h"
#include "patterns/single/BracketsPattern.h"
#include "patterns/single/AdvancedPattern.h"
#include "patterns/single/TracerPattern.h"
#include "patterns/single/BasicPattern.h"
#include "patterns/single/BlendPattern.h"

#include "Log.h"

Pattern *PatternBuilder::make(PatternID id)
{
  if (id > PATTERN_LAST) {
    DEBUG_LOGF("Invalid pattern id: %u", id);
    return nullptr;
  }
  if (isMultiLedPatternID(id)) {
    return makeMulti(id);
  }
  return makeSingle(id);
}

// generate a single LED pattern (nullptr if patternid is not single LED)
SingleLedPattern *PatternBuilder::makeSingle(PatternID id)
{
  if (!isSingleLedPatternID(id)) {
    return nullptr;
  }
  Pattern *pat = makeInternal(id);
  if (!pat) {
    return nullptr;
  }
  // don't set any flags on single pattersn
  return (SingleLedPattern *)pat;
}

// generate a multi LED pattern (nullptr if patternid is not multi LED)
MultiLedPattern *PatternBuilder::makeMulti(PatternID id)
{
  if (!isMultiLedPatternID(id)) {
    return nullptr;
  }
  Pattern *pat = makeInternal(id);
  if (!pat) {
    return nullptr;
  }
  return (MultiLedPattern *)pat;
}

Pattern *PatternBuilder::unserialize(SerialBuffer &buffer)
{
  Pattern *pat = make((PatternID)buffer.unserialize8());
  if (!pat) {
    return nullptr;
  }
  pat->unserialize(buffer);
  return pat;
}

Pattern *PatternBuilder::makeInternal(PatternID id)
{
  Pattern *pat = generate(id);
  if (!pat) {
    ERROR_OUT_OF_MEMORY();
    return nullptr;
  }
  // set private pattern ID via friend class relationship
  pat->m_patternID = id;
  return pat;
}

Pattern *PatternBuilder::generate(PatternID id)
{
  // NOTE: The timings of patterns are only defined at creation time
  //       which means altering the tickrate will not change how fast
  //       a pattern displays unless you re-create it
  switch (id) {
    case PATTERN_STROBE: return new BasicPattern(5, 8);
    case PATTERN_HYPERSTROBE: return new BasicPattern(25, 25);
    case PATTERN_DOPS: return new BasicPattern(2, 13);
    case PATTERN_DOPISH: return new BasicPattern(2, 7);
    case PATTERN_ULTRADOPS: return new BasicPattern(1, 3);
    case PATTERN_STROBIE: return new BasicPattern(3, 22);
  #if 0
    case PATTERN_RIBBON: return new BasicPattern(20);
    case PATTERN_MINIRIBBON: return new BasicPattern(3);
  #endif
    case PATTERN_TRACER: return new TracerPattern();
    case PATTERN_BLINKIE: return new BasicPattern(5, 8, 35);
    case PATTERN_GHOSTCRUSH: return new BasicPattern(1, 0, 50);
    case PATTERN_ADVANCED: return new AdvancedPattern(5, 5, 10, 2, 2, 1);
    case PATTERN_BLEND: return new BlendPattern();
    case PATTERN_RECIPROCAL_BLEND: return new ReciprocalBlendPattern();
    case PATTERN_BRACKETS: return new BracketsPattern();
    case PATTERN_RABBIT: return new RabbitPattern();
    case PATTERN_HUESHIFT: return new HueShiftPattern();
    case PATTERN_THEATER_CHASE: return new TheaterChasePattern();
    default: break;
  }
  DEBUG_LOGF("Unknown pattern id: %u", id);
  return nullptr;
}