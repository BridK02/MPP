class Person:
  def __init__(self, name, age):
    self.name = name
    self.age = age

  def __repr__(self):
    return f'Person("{self.name}", {self.age})'
  
p1 = Person("John", 36)

print(p1.name)
print(p1.age)
print(p1)

#Modify the Person class, such that a person can have multiple addresses.
# You can use a list for this purpose. Add a method to the Person class to add a new address

class Person:
    def __init__(self, name, age, addresses=[]):
        self.name = name
        self.age = age
        self.addresses = addresses

    def add_address(self, address):
        self.addresses.append(address)

    def __repr__(self):
        return f'Person("{self.name}", {self.age}, {self.addresses})'

p1 = Person("John", 36)

p1.add_address("123 Main Street")
p1.add_address("456 Elm Street")

print(p1)