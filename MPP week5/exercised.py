#Create a class called “CollegeProgramme” which represents a programme of study which a student can be enrolled. 
#This class should have a “has many” relationship with another new class called “CollegeModule”. 
#Add other appropriate information and functionality to both classes, for example module name, number of credits,
# add a module to a programme etc. 



class CollegeProgramme:
    def __init__(self, name, duration, modules=[]):
        self.name = name
        self.duration = duration
        self.modules = modules

    def add_module(self, module):
        self.modules.append(module)

    def get_modules(self):
        return self.modules

class CollegeModule:
    def __init__(self, name, credits):
        self.name = name
        self.credits = credits

    def get_name(self):
        return self.name

    def get_credits(self):
        return self.credits

# Create a CollegeProgramme object
programme = CollegeProgramme("Computer Science", 3, [])

# Create a CollegeModule object
module1 = CollegeModule("Programming", 30)

# Add the CollegeModule object to the CollegeProgramme object
programme.add_module(module1)

# Get the list of CollegeModule objects in the CollegeProgramme object
modules = programme.get_modules()

# Print the name and credits of each CollegeModule object in the list
for module in modules:
    print(f"Module name: {module.get_name()}")
    print(f"Module credits: {module.get_credits()}")
