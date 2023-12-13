from dataclasses import dataclass, field
from typing import List
from decimal import Decimal
from typing import Union  
import csv

@dataclass
class Product:
    name: str
    price: Decimal
    clean_name: str = field(init=False)  # Add clean_name attribute

    def __post_init__(self):
        self.clean_name = self.name.strip().lower()

@dataclass 
class ProductStock:
    product: Product
    quantity: int

@dataclass 
class Shop:
    cash: float = 0.0
    stock: List[ProductStock] = field(default_factory=list)

@dataclass
class Customer:
    name: str
    budget: float
    shopping_list: List[List[Union[str, int]]]

def create_and_stock_shop():
    try:
        with open('stock.csv') as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            
            # Read the first row for the shop budget
            cash = float(next(csv_reader)[0])
            
            # Read the remaining lines in groups of three
            stock = []
            for row in csv_reader:
                product_name = row[0].strip().lower()  # Convert to lowercase and remove leading/trailing whitespace
                price = Decimal(row[1])  # Use Decimal for precision
                quantity = int(row[2])

                #print(f"Read product: {product_name}")
                
                product = Product(product_name, price)
                product_stock = ProductStock(product, quantity)
                stock.append(product_stock)

        # Create a Shop object
        shop = Shop(cash, stock)

        # Print all items in the stock
        #print("All items in stock:")
        #for product_stock in shop.stock:
           # print(f"Product: {product_stock.product.name}, Price: {product_stock.product.price}, Quantity: {product_stock.quantity}")

        return shop

    except Exception as e:
        print(f"Error accessing or reading stock file: {e}")
        return None

def read_customer(stock_file, customer_file):
    try:
        with open(customer_file) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')

            # Read the first row for customer information
            customer_info = next(csv_reader)
            customer_name, customer_budget = customer_info[0], float(customer_info[1])

            # Read the remaining lines for the shopping list
            shopping_list = [row for row in csv_reader]

            # Create a Customer object
            customer = Customer(customer_name, customer_budget, shopping_list)
            return customer

    except Exception as e:
        print(f"Error reading customer file: {e}")
        return None

def print_product(product):
    print(f'\nPRODUCT NAME: {product.name} \nPRODUCT PRICE: {product.price}')

def print_customer(customer):
    if customer is not None:
        print(f'CUSTOMER NAME: {customer.name} \nCUSTOMER BUDGET: {customer.budget}')
    else:
        print("Error: No valid customer.")

    for item in customer.shopping_list:
        print_product(item.product)
        print(f'{customer.name} ORDERS {item.quantity} OF ABOVE PRODUCT')
        cost = item.quantity * item.product.price
        print(f'The cost to {customer.name} will be €{cost:.2f}')

def print_shop(shop):
    print(f'Shop has {shop.cash} in cash')
    for item in shop.stock:
        print_product(item.product)
        print(f'The Shop has {item.quantity} of the above')

def process_order(shop, customer):
    if customer is None or customer.shopping_list is None:
        print("Error: No valid customer or shopping list.")
        return

    for order in customer.shopping_list:
        product_name = order[0].strip().lower()
        quantity = int(order[1])

        # Check if the product is in stock
        product_in_stock = next((ps for ps in shop.stock if ps.product.name == product_name), None)
        if product_in_stock is None or product_in_stock.quantity < quantity:
            print(f"Error: Not enough stock for {quantity} units of '{product_name}'. Order not completed.")
            continue

        # Check if the customer has enough budget
        product_price = float(product_in_stock.product.price)
        total_cost = quantity * product_price
        if total_cost > float(customer.budget):
            print(f"Error: Not enough budget for {quantity} units of '{product_name}'. Order not completed.")
            continue

        # Process the customer's request
        print(f"Processing customer '{customer.name}' request for {quantity} units of '{product_name}'.")
        
        # Deduct the purchased quantity from the stock
        product_in_stock.quantity -= quantity

        # Calculate the total cost of the purchase
        total_cost = quantity * product_price

        # Update the shop's cash balance based on the purchase
        shop.cash += total_cost
        print(f"Customer '{customer.name}' spent {total_cost:.2f}. Customer budget remaining: {float(customer.budget) - total_cost:.2f}")
        print(f"Shop's budget increased by {total_cost:.2f}. Shop budget now: {shop.cash:.2f}")

def live_mode(shop: Shop):
    while True:
        product_name = input("Enter product name (or 'exit' to end): ")
        if product_name.lower() == 'exit':
            break

        try:
            quantity = Decimal(input(f"Enter quantity of {product_name}: "))
            product_in_stock = next((item for item in shop.stock if item.product.name == product_name), None)

            if product_in_stock is None or product_in_stock.quantity < quantity:
                print(f"Sorry, the shop does not have enough stock for {quantity} units of {product_name}")
                continue

            cost = Decimal(quantity) * product_in_stock.product.price
            print(f'The cost for {quantity} units of {product_name} is €{cost:.2f}')

            confirm = input("Do you want to buy this? (yes/no): ")
            if confirm.lower() == 'yes':
                shop.cash += float(cost)  # Convert cost to float before adding
                product_in_stock.quantity -= quantity
                print(f"Purchase successful! Shop's cash: €{shop.cash:.2f}, {product_name} in stock: {product_in_stock.quantity}")
            else:
                print("Purchase canceled.")

        except ValueError as e:
            print(f"Error: {e}")

# Usage
shop = create_and_stock_shop()
#customer = read_customer("stock.csv", "customer.csv")
#customer = read_customer("stock.csv", "customer_positive.csv")
#customer = read_customer("stock.csv", "customer_negative.csv")
#customer = read_customer("stock.csv", "customer_insufficientbudget.csv")

# Create a dummy customer for live mode
customer = Customer("LiveModeCustomer", 100.0, [])

try:
    process_order(shop, customer)
    print("Order processed successfully!")
    print_shop(shop)
except ValueError as e:
    print(f"Error processing order: {e}")



# Uncomment the following line to test live mode
live_mode(shop)