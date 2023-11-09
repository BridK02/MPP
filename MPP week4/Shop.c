#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getline(char **ptr, size_t *n, FILE *stream) {
  char *p = *ptr;
  size_t size = *n;
  int c;

  if (size == 0) {
    *ptr = NULL;
    *n = 0;
    return -1;
  }

  while ((c = fgetc(stream)) != EOF) {
    if (c == '\n') {
      break;
    }

    if (size == 1) {
      /*
      ** Reallocate the buffer.
      */
      size_t newsize = size * 2;
      char *newptr = realloc(p, newsize);
      if (newptr == NULL) {
        return -1;
      }

      p = newptr;
      *ptr = newptr;
      size = newsize;
    }

    *p++ = (char) c;
    --size;
  }

  *p = '\0';
  *n = size;

  return c == EOF ? -1 : (p - *ptr);
}

struct Product {
	char* name;
	double price;
};

struct ProductStock {
	struct Product product;
	int quantity;
};

struct Shop {
	double cash;
	struct ProductStock stock[20];
	int index;
};

struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};

void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
	printf("-------------\n");
}

void printCustomer(struct Customer c)
{
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
	printf("-------------\n");
	for(int i = 0; i < c.index; i++)
	{
		printProduct(c.shoppingList[i].product);
		printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
		double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price; 
		printf("The cost to %s will be %.2f\n", c.name, cost);
	}
}

struct Shop createAndStockShop()
{
	struct Shop shop = { 200 };
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s IS A LINE", line);
		char *n = strtok(line, ",");
		char *p = strtok(NULL, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		double price = atof(p);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		struct Product product = { name, price };
		struct ProductStock stockItem = { product, quantity };
		shop.stock[shop.index++] = stockItem;
		// printf("NAME OF PRODUCT %s PRICE %.2f QUANTITY %d\n", name, price, quantity);
    }
	
	return shop;
}

void printShop(struct Shop s)
{
	printf("Shop has %.2f in cash\n", s.cash);
	for (int i = 0; i < s.index; i++)
	{
		printProduct(s.stock[i].product);
		printf("The shop has %d of the above\n", s.stock[i].quantity);
	}
}

int main(void) 
{
	struct Customer dominic = { "Dominic", 100.0 };
	
	struct Product coke = { "Can Coke", 1.10 };
	struct Product bread = { "Bread", 0.7 };
	printProduct(coke);
	
	struct ProductStock cokeStock = { coke, 20 };
	struct ProductStock breadStock = { bread, 2 };
	
	dominic.shoppingList[dominic.index++] = cokeStock;
	dominic.shoppingList[dominic.index++] = breadStock;
	
	printCustomer(dominic);
	
	struct Shop shop = createAndStockShop();
	printShop(shop);
	
	printf("The shop has %d of the product %s\n", cokeStock.quantity, cokeStock.product.name);
	
    return 0;
}