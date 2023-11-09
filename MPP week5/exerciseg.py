#Modify the logic of enrolling a student in a programme by performing some checks. For example a student could be rejected from the program if they are under 18. You could modify the CollegeProgramme
#class such that each program has a limit on how many students it can facilitate, this can be used then
#to reject students.

class CollegeProgramme:
    def __init__(self, name, duration, modules=[], students=[], max_students=100):
        self.name = name
        self.duration = duration
        self.modules = modules
        self.students = students
        self.max_students = max_students

    def add_module(self, module):
        self.modules.append(module)

    def get_modules(self):
        return self.modules

    def add_student(self, student):
        if len(self.students) >= self.max_students:
            raise ValueError("Programme is full")

        if student.age < 18:
            raise ValueError("Student must be at least 18 years old to enroll in a programme")

        self.students.append(student)
      

    def get_students(self):
        return self.students

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
programme = CollegeProgramme("Computer Science", 3, [], max_students=3)

# Create a Student object
student = Student("John", 18)

# Add the student to the programme
programme.add_student(student)

# Try to add another student to the programme
try:
    programme.add_student(Student("Jane", 19))
except ValueError as e:
    print(e)

try:
    programme.add_student(Student("Michael", 34))
except ValueError as e:
    print(e)



try:
    programme.add_student(Student("Tom", 39))
except ValueError as e:
    print(e)