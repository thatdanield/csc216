#include <iostream>
#include <string>

class Job
{
private:
  std::string name;
  int priority;

public:
  Job(std::string n, int p) : name(n), priority(p) {}

  void execute()
  {
    std::cout << "Running the job with name "
              << name
              << " and priority "
              << priority;
  }

  std::string getName()
  {
    return name;
  }

  int getPriority()
  {
    return priority;
  }

  bool operator<(Job& other)
  {
    return priority < other.priority;
  }

};
