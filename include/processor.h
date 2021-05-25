#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
  public:
  float Utilization(Processor processor0);  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  
};

#endif
