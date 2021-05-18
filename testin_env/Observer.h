#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "Tool.h"

class Card;

class Observer {
 public:
  //virtual Card* notify(const Trigger t) = 0;
  virtual ~Observer() = default;
};

#endif

