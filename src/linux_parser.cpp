#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include <iostream>

#include <unistd.h>

#include "linux_parser.h"
#include "system.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  char filename[100];
  sprintf(filename, "/proc/%d/cmdline", pid);
  std::string command = open_text_file(filename) + "/0";
  return command;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  char filename[100];
  sprintf(filename, "/proc/%d/status", pid);
  std::string text = open_text_file(filename);
  if(text == "") return string();
  std::string line = grep(text, "VmSize:");
  if(line == "") return string();
  
  std::string digits = "0123456789";
  std::size_t index0 = line.find_first_of(digits);
  if(index0 == std::string::npos) return string();
  std::size_t index1 = line.find_last_of(digits);
  if(index1 == std::string::npos) return string();
  std::string ram_str = substring(line, index0, index1 + 1);
  //return ram_str;
  float ram_mb = atof(ram_str.c_str()) / 1024;
  char ram_mb_str[100];
  sprintf(ram_mb_str, "%.2f", ram_mb);
  return string(ram_mb_str);
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  char filename[100];
  sprintf(filename, "/proc/%d/status", pid);
  std::string text = open_text_file(filename);
  if(text == "") return string();
  std::string line = grep(text, "Uid:");
  std::size_t index0 = 5;
  std::size_t index1 = line.find_first_of("\t", index0);
  std::string user_id = substring(line, index0, index1);
  return user_id;
}

vector<string> get_lines(string textfile) {
  std::string text = open_text_file(textfile), line;
  std::size_t index0 = 0, index1;
  vector<string> lines;
  if(text == "") return lines;
  while(1) {
    index1 = text.find("\n", index0);
    line = substring(text, index0, index1);
    lines.push_back(line);
    if(index1 >= text.length() - 1) break;
    index0 = index1 + 1;
  }
  return lines;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(std::string user_id_to_find) { 
  vector<string> lines = get_lines("/etc/passwd");
  if(lines.size() == 0) return "";
  string line, username, user_id;
  for(std::size_t i = 0; i < lines.size(); i++) {
    line = "\n" + lines[i];
    username = isolate_string(line, "\n", ":x:");
    user_id = isolate_string(line, ":x:", ":");
    if(user_id == user_id_to_find) return username;
  }
  return std::string();
}

vector<string> get_process_numbers(int pid) {
  char filename[100];
  sprintf(filename, "/proc/%d/stat", pid);
  std::string text = open_text_file(filename);
  std::string process_number;
  std::size_t index0, index1;
  index0 = text.find(")") + 2;
  vector<string> process_numbers;
  if(text == "") return process_numbers;
  process_numbers.push_back("");
  process_numbers.push_back("");
  while(1) {
    index1 = text.find(" ", index0);
    process_number = substring(text, index0, index1);
    process_numbers.push_back(process_number);
    if(process_numbers.size() >= 22) break;
    index0 = index1 + 1;
  }
  return process_numbers;
}

long get_hertz() {
  long Hertz = sysconf(_SC_CLK_TCK);
  return Hertz;
}

float get_system_uptime() {
  std::string text = open_text_file("/proc/uptime");
  if(text == "") return 0.0;
  std::size_t index = text.find(" ");
  std::string uptime_str = text.substr(0, index - 1);
  return atof(uptime_str.c_str());
}

LinuxParser::CpuAndTime LinuxParser::compute_cpu_usage(int pid) {
  long Hertz = get_hertz();
  float system_uptime = get_system_uptime();
  vector<string> process_numbers = get_process_numbers(pid);
  if(process_numbers.size() == 0) return CpuAndTime {0, 0};
  long utime = atol(process_numbers[14 - 1].c_str());
  long stime = atol(process_numbers[15 - 1].c_str());
  long cutime = atol(process_numbers[16 - 1].c_str());
  long cstime = atol(process_numbers[17 - 1].c_str());
  long starttime = atol(process_numbers[22 - 1].c_str());
  long total_time = utime + stime + cutime + cstime;
  float seconds = system_uptime - (starttime / Hertz);
  float cpu_usage = (total_time / Hertz) / seconds;
  return CpuAndTime {cpu_usage, seconds};
}
