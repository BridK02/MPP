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
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // Read the initial cash value from the "stock.csv" file
    read = getline(&line, &len, fp);
    if (read == -1)
        exit(EXIT_FAILURE);

    struct Shop shop = { .cash = atof(line) };

    // Rest of the code for reading stock details
    while ((read = getline(&line, &len, fp)) != -1) {
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
    }

    fclose(fp);
    if (line)
        free(line);

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

void readCustomerOrders(struct Customer *customer, const char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("Error: Unable to open customer file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1)
  {
    char *n = strtok(line, ",");
    char *p = strtok(NULL, ",");
    int quantity = atoi(p);
    char *name = malloc(sizeof(char) * 50);
    strcpy(name, n);

    struct Product product = {name, 0.0 }; // The product code is not specified in the customer file
    struct ProductStock stockItem = { product, quantity };
    customer->shoppingList[customer->index++] = stockItem;
  }

  fclose(fp);
  if (line)
    free(line);
}

// Function prototype
//void processOrder(struct Shop *shop, struct Customer *customer);

int main(void)
{
    struct Shop shop = createAndStockShop();
    printShop(shop);

    struct Customer dominic = {"Dominic", 100.0};
    readCustomerOrders(&dominic, "customer.csv");
    printCustomer(dominic);

    // Call to the processOrder function
    // processOrder(&shop, &dominic);

    printShop(shop);

    return 0;
}