#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void ParseFile(string, vector<bool>&, vector<string>&, vector<string>&);
void ShowList(const vector<bool>&, const vector<string>&, const vector<string>&);
void ShowMenu();
void AddTask(vector<bool>&, vector<string>&, vector<string>&);
void EditTask(vector<bool>&, vector<string>&, vector<string>&);
void ToggleTask(vector<bool>&);
void RemoveTask(vector<bool>&, vector<string>&, vector<string>&);
void SaveFile(string, const vector<bool>&, const vector<string>&, const vector<string>&);

ifstream fileReader;
ofstream fileWriter;

int main()
{
  vector<bool> myChecks;
  vector<string> myTasks;
  vector<string> myDates;

  ParseFile("todo.txt", myChecks, myTasks, myDates);

  while (true)
  {
    if (system("CLS")) system("clear");

    ShowList(myChecks, myTasks, myDates);
    ShowMenu();

    cout << "Choice: ";
    int choice;
    cin >> choice;
    cin.ignore(1000, 10);
    cout << endl << endl;

    switch (choice)
    {
      case 1:
        AddTask(myChecks, myTasks, myDates);
        break;
      case 2:
        ToggleTask(myChecks);
        break;
      case 3:
        RemoveTask(myChecks, myTasks, myDates);
        break;
      case 4:
        EditTask(myChecks, myTasks, myDates);
        break;
      case 5:
        return 0;
    }

    SaveFile("todo.txt", myChecks, myTasks, myDates);
  }
}

void ParseFile(string fileName,
               vector<bool>& checks,
               vector<string>& tasks,
               vector<string>& dates)
{
  fileReader.open(fileName);
  string line;
  while (getline(fileReader, line))
  {
    string check = line.substr(0, 1);
    string task = line.substr(4, line.length());
    string date = task.substr(task.find("| ") + 2, task.length());
    task = task.substr(0, task.find("|"));
    if (check == "0")
    {
      checks.push_back(false);
    }
    else
    {
      checks.push_back(true);
    }
    tasks.push_back(task);
    dates.push_back(date);
  }
  fileReader.close();
}

void ShowList(const vector<bool>& checks,
               const vector<string>& tasks,
               const vector<string>& dates)
{
  for (size_t i = 0; i < checks.size(); i++)
  {
    cout << i + 1 << ". ";
    if (checks[i] == false)
    {
      cout << "[ ] ";
    }
    else
    {
      cout << "[x] ";
    }
    cout << tasks[i] << endl;
    cout << "       " << dates[i] << endl << endl;
  }
}

void ShowMenu()
{
  cout << "Menu:" << endl
       << "1. Add a task" << endl
       << "2. Check/uncheck a task" << endl
       << "3. Remove a task" << endl
       << "4. Edit a task" << endl
       << "5. Exit" << endl;
}

void AddTask(vector<bool>& checks,
             vector<string>& tasks,
             vector<string>& dates)
{
  cout << "Add Task" << endl
       << "========" << endl;
  cout << "Description: ";
  string task;
  getline(cin, task);
  cout << "Date: ";
  string date;
  getline(cin, date);
  checks.push_back(false);
  tasks.push_back(task);
  dates.push_back(date);
}

void EditTask(vector<bool>& checks,
              vector<string>& tasks,
              vector<string>& dates)
{
  cout << "Edit Task" << endl
       << "==========" << endl;
  cout << "Number of task to edit: ";
  int num;
  cin >> num;
  cin.ignore(1000, 10);
  if (num > 0 && static_cast<size_t>(num) <= checks.size())
  {
    cout << "Current Description: " << tasks[num - 1] << endl;
    cout << "Enter new Description: ";
    getline(cin, tasks[num - 1]);
    cout << "Current Date: " << dates[num - 1] << endl;
    cout << "Enter new Date: ";
    getline(cin, dates[num - 1]);
  }
  else
  {
    cout << "Invalid task number." << endl;
  }
}

void ToggleTask(vector<bool>& checks)
{
  cout << "Toggle Task" << endl
       << "===========" << endl;
  cout << "Number: ";
  int num;
  cin >> num;
  cin.ignore(1000, 10);
  if (num > 0 && static_cast<size_t>(num) <= checks.size())
  {
    checks[num - 1] = !checks[num - 1];
  }
  else
  {
    cout << "Invalid task number." << endl;
  }
}

void RemoveTask(vector<bool>& checks,
                vector<string>& tasks,
                vector<string>& dates)
{
  cout << "Remove Task" << endl
       << "===========" << endl;
  cout << "Number: ";
  int num;
  cin >> num;
  cin.ignore(1000, 10);
  if (num > 0 && static_cast<size_t>(num) <= checks.size())
  {
    checks.erase(checks.begin() + (num - 1));
    tasks.erase(tasks.begin() + (num - 1));
    dates.erase(dates.begin() + (num - 1));
  }
  else
  {
    cout << "Invalid task number." << endl;
  }
}

void SaveFile(string fileName,
              const vector<bool>& checks,
              const vector<string>& tasks,
              const vector<string>& dates)
{
  fileWriter.open(fileName);
  for (size_t i = 0; i < checks.size(); i++)
  {
    fileWriter << (checks[i] ? "1" : "0") << " - " << tasks[i] << " | " << dates[i] << endl;
  }
  fileWriter.close();
}
