#Create a relationship such that a Student has-a College Programme and a College Programme has many students.
# When a student is added to a college programme the state of that student class should be modified 
# to include a reference to the programme e.g. new_student.set_college_programme = self

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

    def __str__(self):
        return f"CollegeProgramme(name={self.name}, duration={self.duration})"


class Student:
    def __init__(self, name, age, college_programme=None):
        self.name = name
        self.age = age
        self.college_programme = college_programme

    def set_college_programme(self, college_programme):
        self.college_programme = college_programme

    def get_college_programme(self):
        return self.college_programme

# Create a CollegeProgramme object
programme = CollegeProgramme("Computer Science", 3, [])

# Create a Student object
student = Student("John", 36)

# Add the student to the college programme
programme.add_student(student)

# Set the student's college programme reference to the programme
student.set_college_programme(programme)

# Print the student's college programme
print(student.get_college_programme())
