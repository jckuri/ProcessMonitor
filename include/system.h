#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#define whitespaces " \t\n\r\f\v"

std::string open_text_file(std::string filename);
std::string grep(std::string string0, std::string pattern);
std::string isolate_string(std::string line, std::string left_pattern, std::string right_pattern);
std::string substring(std::string s, std::size_t index0, std::size_t index1);
long pick_long(std::string s, std::size_t index0, std::size_t index1);

#endif
