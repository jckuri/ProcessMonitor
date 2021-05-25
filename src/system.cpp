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
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TRIM WHITE SPACES
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

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
  std::size_t right_index = line.find(right_pattern, left_index + left_pattern.length());
  if(right_index == std::string::npos) return string();
  return substring(line, left_index + left_pattern.length(), right_index);
}

string substring(string s, std::size_t index0, std::size_t index1) {
  return s.substr(index0, index1 - index0);
}

long pick_long(string s, std::size_t index0, std::size_t index1) {
  return atol(substring(s, index0, index1).c_str());
}

// TODO: Return the system's CPU
Processor System::Cpu() {
  bool verbose = 0;
  std::string text = open_text_file("/proc/stat");
  std::string line = grep(text, "cpu ") + " ";
  if(verbose) std::cout << line << "\n\r";
  line = line.substr(3);
  std::size_t index0 = line.find_first_not_of(whitespaces);
  line = line.substr(index0);
  Processor proc;
  std::size_t index1 = line.find(" ", 0);
  proc.user = pick_long(line, 0, index1);
  std::size_t index2 = line.find(" ", index1 + 1);
  proc.nice = pick_long(line, index1, index2);
  std::size_t index3 = line.find(" ", index2 + 1);
  proc.system = pick_long(line, index2, index3);
  std::size_t index4 = line.find(" ", index3 + 1);
  proc.idle = pick_long(line, index3, index4);
  std::size_t index5 = line.find(" ", index4 + 1);
  proc.iowait = pick_long(line, index4, index5);
  std::size_t index6 = line.find(" ", index5 + 1);
  proc.irq = pick_long(line, index5, index6);
  std::size_t index7 = line.find(" ", index6 + 1);
  proc.softirq = pick_long(line, index6, index7);
  std::size_t index8 = line.find(" ", index7 + 1);
  proc.steal = pick_long(line, index7, index8);
  std::size_t index9 = line.find(" ", index8 + 1);
  proc.guest = pick_long(line, index8, index9);
  std::size_t index10 = line.find(" ", index9 + 1);
  proc.guest_nice = pick_long(line, index9, index10);
  //user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  if(verbose) std::cout
    << " Processor: user=" << proc.user
    << " nice=" << proc.nice
    << " system=" << proc.system
    << " idle=" << proc.idle
    << " iowait=" << proc.iowait
    << " irq=" << proc.irq
    << " softirq=" << proc.softirq
    << " steal=" << proc.steal
    << " guest=" << proc.guest
    << " guest_nice=" << proc.guest_nice
    << "\n";
  return proc;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pIds = LinuxParser::Pids();
  vector<Process> *processes = new vector<Process>();
  for(unsigned int i = 0; i < pIds.size(); i++) {
    Process proc;
    proc.pid = pIds[i];
    proc.user_id = LinuxParser::Uid(proc.pid);
    proc.username = LinuxParser::User(proc.user_id);
    proc.command = LinuxParser::Command(proc.pid);
    proc.ram = LinuxParser::Ram(proc.pid);    
    LinuxParser::CpuAndTime ct = LinuxParser::compute_cpu_usage(proc.pid);
    proc.cpu_usage = ct.cpu_usage;
    proc.up_time = ct.uptime_in_seconds;
    processes->push_back(proc);
  }
  return *processes;
}

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
int System::RunningProcesses() {
  std::string text = open_text_file("/proc/stat");
  std::string line = grep(text, "procs_running") + "\n";
  std::string string0 = isolate_string(line, "procs_running ", "\n");
  return atoi(string0.c_str());
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  std::string text = open_text_file("/proc/stat");
  std::string line = grep(text, "processes") + "\n";
  std::string string0 = isolate_string(line, "processes ", "\n");
  return atoi(string0.c_str());
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  std::string text = open_text_file("/proc/uptime");
  std::string line = "uptime:" + text;
  std::string string0 = isolate_string(line, "uptime:", ".");
  return atoi(string0.c_str());
}
