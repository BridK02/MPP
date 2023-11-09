//Create structs to represent...
//• An employee (name, age, salary, years worked, job title)
//• A college module (module name, credits it is worth)
//• Student (name, age, an array of their college modules)
//• Manager (name, age, salary, an array of the employees they manage)
//The student structure should have internally an array of college modules. The same goes for the manager and
//their employees. In Listing 2 we have a partial solution for some of the above tasks. We have a Student structure
//which holds an array of 10 modules, this means that each student we create can have 10 modules. We add two
//modules to the student struct and we print them out. You can expand on this to add all the other information
//specified above such as name and age. It would be useful to keep track of how many modules have been added to
//each student, so that we know what index to place the next one. This can be done by storing an integer in the
//struct to keep track.
//For each struct type create a print method which outputs to the console all the information stored in the struct.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Employee {
    char name[50];
    int age;
    double salary;
    int years_worked;
    char job_title[50];
};

struct Module {
    char name[50];
    int credits;
};

struct Student {
    char name[50]; 
    int age;
    struct Module modules[10];
    int num_modules; // Keep track of the number of modules added
};

struct Manager {
    char name[50];
    int age;
    double salary;
    struct Employee employees[10];
    int num_employees; // Keep track of the number of employees managed
};

void printEmployee(struct Employee employee) {
    printf("Employee Name: %s\n", employee.name);
    printf("Age: %d\n", employee.age);
    printf("Salary: %.2lf\n", employee.salary);
    printf("Years Worked: %d\n", employee.years_worked);
    printf("Job Title: %s\n", employee.job_title);
}

void printModule(struct Module module) {
    printf("Module Name: %s\n", module.name);
    printf("Credits: %d\n", module.credits);
}

void printStudent(struct Student student) {
    printf("Student Name: %s\n", student.name);
    printf("Age: %d\n", student.age);
    printf("Modules:\n");
    for (int i = 0; i < student.num_modules; i++) {
        printModule(student.modules[i]);
    }
}

void printManager(struct Manager manager) {
    printf("Manager Name: %s\n", manager.name);
    printf("Age: %d\n", manager.age);
    printf("Salary: %.2lf\n", manager.salary);
    printf("Managed Employees:\n");
    for (int i = 0; i < manager.num_employees; i++) {
        printEmployee(manager.employees[i]);
    }
}

int main(void) {
    struct Employee employee = { "John Doe", 30, 50000.0, 5, "Software Developer" };
    printEmployee(employee);

    struct Module module = { "Multi-paradigm Programming", 3 };
    struct Module module2 = { "Introduction to Programming", 4 };

    struct Student student = { "Alice Smith", 20 };
    student.modules[student.num_modules++] = module;
    student.modules[student.num_modules++] = module2;
    printStudent(student);

    struct Employee employee1 = { "Jane Smith", 25, 60000.0, 3, "Project Manager" };
    struct Employee employee2 = { "Bob Johnson", 35, 75000.0, 7, "Senior Developer" };

    struct Manager manager = { "Eva Williams", 40, 90000.0 };
    manager.employees[manager.num_employees++] = employee1;
    manager.employees[manager.num_employees++] = employee2;
    printManager(manager);

    return 0;
}