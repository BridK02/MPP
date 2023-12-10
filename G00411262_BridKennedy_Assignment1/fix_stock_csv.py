import re

with open('stock.csv', 'r') as f:
  data = f.read()

new_data = re.sub(r'(\r|\n)', ',', data)

with open('stock.csv', 'w') as f:
  f.write(new_data)