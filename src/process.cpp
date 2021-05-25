#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_usage; }

// TODO: Return the command that generated this process
string Process::Command() {
  return command + "                       "; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return username; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return cpu_usage < a.cpu_usage;
}
