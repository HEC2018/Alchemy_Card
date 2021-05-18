#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include "Card.h"

class Observer {
 public:
  virtual const Card* const notify(Trigger t) = 0;
  virtual ~Observer() = default;
};
#endif

