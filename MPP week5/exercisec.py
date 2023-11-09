#Modify the program such that the Student has methods to access their home address and college
#address. This should use the list from the parent class. If there is only 1 address then the college
#address will be the same as the home address.

class Person:
    def __init__(self, name, age, addresses=[]):
        self.name = name
        self.age = age
        self.addresses = addresses

    def add_address(self, address):
        self.addresses.append(address)

    def get_home_address(self):
        if len(self.addresses) > 1:
            return self.addresses[0]
        else:
            return self.addresses[0]

class Student(Person):
    def __init__(self, name, age, college_course, address):
        super().__init__(name, age, [address])
        self.college_course = college_course

    def get_college_address(self):
        if len(self.addresses) > 1:
            return self.addresses[1]
        else:
            return self.get_home_address()

p1 = Student("John", 36, "Computer Science", "123 Main Street")

print(p1.get_home_address())
print(p1.get_college_address())

