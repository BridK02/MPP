#“CollegeProgramme” should have methods to return all the modules which are part of the programme. 
#You also create another method which checks if a module is in a programme by name e.g. is_module_on_programme(name) 
from exercised import CollegeModule

class CollegeProgramme:
    def __init__(self, name, duration, modules=[], students=[]):
        self.name = name
        self.duration = duration
        self.modules = modules
        self.students = students

    def add_module(self, module):
        self.modules.append(module)

    def get_modules(self):
        return self.modules

    def add_student(self, student):
        self.students.append(student)

    def get_students(self):
        return self.students

    def __repr__(self):
        return f"CollegeProgramme({self.name}, {self.duration}, {self.modules}, {self.students})"

class Student:
    def __init__(self, name, age, college_programme=None):
        self.name = name
        self.age = age
        self.college_programme = college_programme

    def set_college_programme(self, college_programme):
        self.college_programme = college_programme

    def get_college_programme(self):
        return self.college_programme

    def __repr__(self):
        return f"Student({self.name}, {self.age}, {self.college_programme})"

# Create a CollegeProgramme object
programme = CollegeProgramme("Computer Science", 3)

# Create a Student object
student = Student("John", 36)

# Add the student to the programme
programme.add_student(student)

# Set the student's college programme reference to the programme
student.set_college_programme(programme)

# Print the student and the programme
print(student)
print(programme)