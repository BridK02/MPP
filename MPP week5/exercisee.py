#“CollegeProgramme” should have methods to return all the modules which are part of the programme. 
#You also create another method which checks if a module is in a programme by name e.g. is_module_on_programme(name) 
from exercised import CollegeModule


class CollegeProgramme:
    def __init__(self, name, duration, modules=[]):
        self.name = name
        self.duration = duration
        self.modules = modules

    def add_module(self, module):
        self.modules.append(module)

    def get_modules(self):
        return self.modules

    def is_module_on_programme(self, module_name):
        for module in self.modules:
            if module.get_name() == module_name:
                return True
        return False

# Create a CollegeProgramme object
programme = CollegeProgramme("Computer Science", 3, [])

# Create a CollegeModule object
module1 = CollegeModule("Programming", 30)

# Add the CollegeModule object to the CollegeProgramme object
programme.add_module(module1)

# Check if the module is in the programme by name
is_module_on_programme = programme.is_module_on_programme("Programming")

# Print the result
print(is_module_on_programme)
