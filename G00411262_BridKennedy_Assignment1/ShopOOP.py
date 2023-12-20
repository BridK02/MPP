import csv
from decimal import Decimal

class Product:
    def __init__(self, name, price=0):
        self.name = name
        self.price = price

    def __repr__(self):
        return f'NAME: {self.name} PRICE: {self.price}'

class ProductStock:
    def __init__(self, product, quantity):
        self.product = product
        self.quantity = quantity

    def __repr__(self):
        return f"{self.product} QUANTITY: {self.quantity}"

class Customer:
    def __init__(self, name, budget, shopping_list):
        self.name = name
        self.budget = budget
        self.shopping_list = shopping_list

    def calculate_costs(self, price_list):
        for list_item in self.shopping_list:
            for shop_item in price_list:
                if list_item.product.name.lower() == shop_item.product.name.lower():
                    # Create a new Product instance with the updated price
                    updated_product = Product(shop_item.product.name, shop_item.product.price)
                    list_item.product = updated_product
                    break  # Stop after finding the matching product in the shop


    def order_cost(self):
        cost = 0
        for list_item in self.shopping_list:
            cost += list_item.quantity * list_item.product.price
        return cost

    def make_purchase(self, shop_stock, shop):
        for item in self.shopping_list:
            for stock_item in shop_stock:
                if item.product.name == stock_item.product.name:
                    if item.quantity <= stock_item.quantity:
                        cost = item.quantity * item.product.price
                        if cost <= self.budget:
                            stock_item.quantity -= item.quantity
                            shop.cash += cost
                            #print(f"Purchase successful! Remaining budget: {round(self.budget, 2)}")
                        else:
                            raise ValueError(f"Not enough budget to buy {item.quantity} {item.product.name}, transaction canceled, goodbye")
                            break

        # Deduct the total cost from the budget only if the entire purchase can be completed
        self.budget -= total_cost
        # Increase shop's budget by the cost of the purchase
        print(f"Purchase successful! Remaining budget: {self.budget}")

    def __repr__(self):
        result = f"{self.name} wants to buy"
        for item in self.shopping_list:
            result += f"\nPRODUCT NAME: {item.product.name.capitalize()} PRICE: {item.product.price} QUANTITY: {item.quantity}"
            cost = item.quantity * item.product.price
            result += f" COST: {round(cost, 2)}" if cost != 0 else f" Product unavailable, cannot purchase :("
        result += f"\nThe cost would be: {round(self.order_cost(), 2)}, he would have {round(self.budget - self.order_cost(), 2)} left"
        return result

class Shop:
    def __init__(self, cash, stock):
        self.cash = cash
        self.stock = stock

    def __repr__(self):
        result = f'Shop has {self.cash} in cash\n'
        for item in self.stock:
            result += f"{item}\n"
        return result

def create_and_stock_shop():
    try:
        with open('stock.csv') as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            
            # Read the first row for the shop budget
            cash = float(next(csv_reader)[0])
            
            # Read the remaining lines in groups of three
            stock = []
            for row in csv_reader:
                product_name = row[0].strip().lower()
                price = float(row[1])
                quantity = int(row[2])

                product = Product(product_name, price)
                product_stock = ProductStock(product, quantity)
                stock.append(product_stock)

        return Shop(cash, stock)

    except Exception as e:
        print(f"Error accessing or reading stock file: {e}")
        return None

def read_customer(customer_file):
    try:
        with open(customer_file) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            customer_name, customer_budget = first_row[0], float(first_row[1])

            shopping_list = []
            for row in csv_reader:
                name = row[0]
                quantity = float(row[1])
                p = Product(name)
                ps = ProductStock(p, quantity)
                shopping_list.append(ps)

        return Customer(customer_name, customer_budget, shopping_list)

    except Exception as e:
        print(f"Error reading customer file: {e}")
        return None

def main():
    try:
        # Initialize shop and customer
        shop = create_and_stock_shop()
        '''
        # Uncomment/comment the following test scenarios as needed
        #customer = read_customer('customer.csv')
        customer = read_customer('customer_insufficientbudget.csv')
        #customer = read_customer('customer_positive.csv')
        #customer = read_customer('customer_negative.csv")
        try:
            customer.calculate_costs(shop.stock)
            customer.make_purchase(shop.stock, shop)  # Pass the shop instance
            print(f"{customer.name} wants to buy")
            for item in customer.shopping_list:
                cost = item.quantity * item.product.price
                print(f"PRODUCT NAME: {item.product.name} PRICE: {item.product.price} QUANTITY: {item.quantity} COST: {cost:.2f}")

            # Print the total cost and remaining budget for the customer
            print("The cost would be: {:.2f}, he would have {:.2f} left".format(customer.order_cost(), customer.budget - customer.order_cost()))

            # Print the state of the shop after the transaction
            print(shop)
        
        
        except ValueError as e:
            print(str(e))
        '''
        
        
        # Live mode (comment out if not needed)
        
        def live_mode(shop: Shop):
            while True:
                product_name = input("Enter product name (or 'exit' to end): ")
                if product_name.lower() == 'exit':
                    break

                try:
                    quantity = Decimal(input(f"Enter quantity of {product_name}: "))
                    product_in_stock = next((item for item in shop.stock if item.product.name.lower() == product_name.lower()), None)

                    if product_in_stock is None or product_in_stock.quantity < quantity:
                        print(f"Sorry, the shop does not have enough stock for {quantity} units of {product_name}")
                        continue

                    cost = quantity * Decimal(product_in_stock.product.price)  # Convert product price to Decimal
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
        # Uncomment the next line to run live mode
        live_mode(shop)
        

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()