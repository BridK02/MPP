class Person:
    def __init__(self, name, age, addresses=None):
        self.name = name
        self.age = age
        self.addresses = addresses if addresses else []

    def __repr__(self):
        return f'Person("{self.name}", {self.age}, {self.addresses})'

    def add_address(self, address):
        self.addresses.append(address)


class CollegeModule:
    def __init__(self, name, credits):
        self.name = name
        self.credits = credits

    def __repr__(self):
        return f'CollegeModule("{self.name}", {self.credits})'


class CollegeProgramme:
    def __init__(self, name, max_students=100):
        self.name = name
        self.modules = []
        self.students = []
        self.max_students = max_students

    def __repr__(self):
        return f'CollegeProgramme("{self.name}", {self.modules}, {self.students}, {self.max_students})'

    def add_module(self, module):
        self.modules.append(module)

    def get_modules(self):
        return self.modules

    def is_module_on_programme(self, module_name):
        return any(module.name == module_name for module in self.modules)

    def add_student(self, student):
        if student.age < 18:
            raise ValueError("Student must be at least 18 years old to enroll in a programme")

        if len(self.students) >= self.max_students:
            raise ValueError("Programme is full")

        student.set_college_programme(self)
        self.students.append(student)


class Student(Person):
    def __init__(self, name, age, college_course, addresses=None):
        super().__init__(name, age, addresses)
        self.college_course = college_course
        self.college_programme = None

    def __repr__(self):
        return f'Student("{self.name}", {self.age}, "{self.college_course}", {self.addresses}, {self.college_programme})'

    def set_college_programme(self, college_programme):
        self.college_programme = college_programme

    def get_home_address(self):
        if self.addresses:
            return self.addresses[0]
        else:
            return None

    def get_college_address(self):
        if len(self.addresses) > 1:
            return self.addresses[1]
        elif self.addresses:
            return self.addresses[0]
        else:
            return None
# Create modules
module1 = CollegeModule("Mathematics", 5)
module2 = CollegeModule("Computer Science", 4)
module3 = CollegeModule("Physics", 3)

# Create a college programme
programme = CollegeProgramme("Computer Science")

# Add modules to the programme
programme.add_module(module1)
programme.add_module(module2)
programme.add_module(module3)

# Create students
student1 = Student("John", 20, "Computer Science", ["123 Any St"])
student2 = Student("Jane", 18, "Computer Science", ["456 Galway St"])
student3 = Student("Bob", 17, "Computer Science", ["789 Donegal St"])

# Add addresses to students
student1.add_address("456 Mayo St")
student2.add_address("789 Sligo St")

# Test adding students to the programme
try:
    programme.add_student(student1)
    print("Student added to the programme.")
except ValueError as e:
    print(f"Error: {e}")

try:
    programme.add_student(student2)
    print("Student added to the programme.")
except ValueError as e:
    print(f"Error: {e}")

try:
    programme.add_student(student3)
    print("Student added to the programme.")
except ValueError as e:
    print(f"Error: {e}")

# Print information about the students
print(student1)
print(student2)
print(student3)

# Print programme information
print(programme)
print("Modules in the programme:", programme.get_modules())
print("Is 'Mathematics' on the programme?", programme.is_module_on_programme("Mathematics"))
print("Is 'Chemistry' on the programme?", programme.is_module_on_programme("Chemistry"))


