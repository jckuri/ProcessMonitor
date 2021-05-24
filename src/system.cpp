#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <fstream>
#include <streambuf>
#include <iostream>
//using namespace std;

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TRIM WHITE SPACES
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

const char* whitespaces = " \t\n\r\f\v";

inline std::string& rtrim(std::string& s, const char* t = whitespaces) {
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

inline std::string& ltrim(std::string& s, const char* t = whitespaces) {
  s.erase(0, s.find_first_not_of(t));
  return s;
}

inline std::string& trim(std::string& s, const char* t = whitespaces) {
  return ltrim(rtrim(s, t), t);
}

// END OF TRIM WHITE SPACES

std::string open_text_file(std::string filename) {
  std::ifstream file(filename);
  std::string text((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());
  return text;
}

std::string grep(string string0, string pattern) {
  std::size_t index0 = string0.find(pattern);
  if(index0 == std::string::npos) return string();
  std::string string1 = string0.substr(index0);
  std::size_t index1 = string1.find("\n");
  if(index1 == std::string::npos) return string();
  std::string string2 = string1.substr(0, index1);
  return string2;
}

std::string isolate_string(string line, string left_pattern, string right_pattern) {
  std::size_t left_index = line.find(left_pattern);
  if(left_index == std::string::npos) return string();
  std::string line2 = line.substr(left_index + left_pattern.length());
  std::size_t right_index = line2.find(right_pattern);
  if(right_index == std::string::npos) return string();
  return line2.substr(0, right_index);
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  std::string text = open_text_file("/proc/version");
  std::string line = grep(text, "Linux version ");
  return isolate_string(line, "Linux version ", " (");
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  std::string text = open_text_file("/proc/meminfo");
  std::string total_line = grep(text, "MemTotal:");
  std::string free_line = grep(text, "MemFree:");
  std::string mem_total_string = isolate_string(total_line, "MemTotal:", " kB");
  std::string mem_free_string = isolate_string(free_line, "MemFree:", " kB");
  int mem_total = atoi(trim(mem_total_string).c_str());
  int mem_free = atoi(trim(mem_free_string).c_str());
  int mem_used = mem_total - mem_free;
  return (float)mem_used / (float)mem_total;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  std::string text = open_text_file("/etc/os-release");
  std::string line = grep(text, "PRETTY_NAME");
  return isolate_string(line, "\"", "\"");
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }
