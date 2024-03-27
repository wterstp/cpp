#include <cstring>
#include <iostream>
using namespace std;
class Person {
public:
  Person(const char *name, int age)
      : _name(new char[strlen(name) + 1]()), _age(age) {
    strcpy(_name, name);
    cout << "person(string,int)" << endl;
  }
  ~Person() {
    if (_name) {
      delete[] _name;
      _name = nullptr;
    }
    cout << "!person()" << endl;
  };
  void display() const {
    cout << "name:" << _name << " "
         << "age" << _age << endl;
  }
  Person(const Person &other) {
    _name = new char[strlen(other._name) + 1];
    strcpy(_name, other._name);
    _age = other._age;
  }
  Person &operator=(const Person &other) {
    if (this != &other) {
      delete[] _name;
      _name = new char[strlen(other._name) + 1];
      strcpy(_name, other._name);
      _age = other._age;
    }
    return *this;
  }

protected:
  char *_name;
  int _age;
};

class Employee : public Person {
public:
  Employee(const char *name, int age, const char *department, int salary)
      : Person(name, age), _department(new char[strlen(department) + 1]),
        _salary(salary) {
    strcpy(_department, department);
  }

  ~Employee() { delete[] _department; }
  Employee(const Employee &other) : Person(other), _salary(other._salary) {
    _department = new char[strlen(other._department) + 1];
    strcpy(_department, other._department);
  }

  Employee &operator=(const Employee &other) {
    if (this != &other) {
      Person::operator=(other); // Call base class assignment operator
      delete[] _department;
      _department = new char[strlen(other._department) + 1];
      strcpy(_department, other._department);
      _salary = other._salary;
    }
    return *this;
  }
  void display() const {
    Person::display();
    std::cout << "Department: " << _department << ", Salary: " << _salary
              << std::endl;
  }

private:
  char *_department;
  int _salary;
};

void test0() {
  Employee emp1("Alice", 30, "HR", 50000.0);
  Employee emp2 = emp1;
  Employee emp3("Bob", 35, "Engineering", 60000.0);
  cout << "emp3:";
  emp3.display();

  emp3 = emp2;
  cout << "emp1:";
  emp1.display();
  cout << "emp2:";

  emp2.display();
  cout << "emp3:";

  emp3.display();
};

int main() {
  test0();

  return 0;
}
