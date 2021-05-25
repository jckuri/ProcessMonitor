#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization(Processor p0) { 
  float PrevIdle = p0.idle + p0.iowait;
  float Idle = idle + iowait;

  float PrevNonIdle = p0.user + p0.nice + p0.system + p0.irq + p0.softirq + p0.steal;
  float NonIdle = user + nice + system + irq + softirq + steal;

  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;

  float CPU_Percentage = (totald - idled) / totald;

  return CPU_Percentage; 
}
