class Person:
  def __init__(self, name, age):
    self.name = name
    self.age = age

  def __repr__(self):
    return f'Person("{self.name}",{self.age})'

class Student(Person):
  def __init__(self, name, age, college_course):
    Person.__init__(self, name, age)
    self.college_course = college_course

  def __repr__(self):
    return f'Student({self.name}, {self.age}, {self.college_course})'
  
p1 = Student("John", 36, "Computer Science")
print(p1)

#Modify the Student class to extend the Person class which has been modified above. This means the
#student should send an address to the parent class
class Person:
    def __init__(self, name, age, addresses=[]):
        self.name = name
        self.age = age
        self.addresses = addresses

    def add_address(self, address):
        self.addresses.append(address)

    def __repr__(self):
        return f'Person("{self.name}", {self.age}, {self.addresses})'

class Student(Person):
    def __init__(self, name, age, college_course, address):
        super().__init__(name, age, [address])
        self.college_course = college_course

    def __repr__(self):
        return f'Student({self.name}, {self.age}, {self.college_course}, {self.addresses[0]})'

p1 = Student("John", 36, "Computer Science", "123 Main Street")
print(p1)
