#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h> 
#include "HTUtils.h"
#include <unistd.h> 


/* Read up to (and including) a newline from STREAM into *LINEPTR
   (and null-terminate it). *LINEPTR is a pointer returned from malloc (or
   NULL), pointing to *N characters of space.  It is realloc'd as
   necessary.  Returns the number of characters read (not including the
   null terminator), or -1 on error or EOF.  */

int getline(char **lineptr, size_t *n, FILE *stream)
{
static char line[256];
char *ptr;
unsigned int len;

   if (lineptr == NULL || n == NULL)
   {
      errno = EINVAL;
      return -1;
   }

   if (ferror (stream))
      return -1;

   if (feof(stream))
      return -1;
     
   fgets(line,256,stream);

   ptr = strchr(line,'\n');   
   if (ptr)
      *ptr = '\0';

   len = strlen(line);
   
   if ((len+1) < 256)
   {
      ptr = realloc(*lineptr, 256);
      if (ptr == NULL)
         return(-1);
      *lineptr = ptr;
      *n = 256;
   }

   strcpy(*lineptr,line); 
   return(len);
}

struct Product {
  char *name;
  double price;
  char *productCode; 
};

struct OrderItem {
  char *productName;
  int quantity;
};

struct ProductStock {
    struct Product product;
    int quantity;
    int index; 
};

struct Shop {
    double cash;
    struct ProductStock stock[100];
    int index;
};

struct Customer {
    char *name;
    double budget;
    struct ProductStock shoppingList[100];
    int index;
};

ssize_t custom_getline(char **ptr, size_t *n, FILE *stream) {
  size_t size = *n;
  char *line = *ptr;

  if (line == NULL) {
    line = malloc(size);
    if (line == NULL) {
      return -1; // Error in allocation
    }
  }

  if (fgets(line, size, stream) == NULL) {
    if (line[0] != '\0') {
      // Free the partially allocated line
      free(line);
    }
    return -1; // No more lines to read or error
  }

  size_t len = strlen(line);

  if (len > 0 && line[len - 1] == '\n') {
    line[len - 1] = '\0'; // Remove newline if present
  }

  if (line[0] == ',' || line[0] == '0') {
    // Skip the first line (cash amount) if it's an empty or invalid cash amount
    free(line);
    line = NULL;
    size = 0;
  } else {
    // Check if the first line is a valid cash amount
    char *temp = strdup(line);
    if (strlen(temp) < 10) {
      free(temp);
      fprintf(stderr, "Error: Invalid cash amount in stock.csv\n");
      free(line);
      return -1;
    }

    // Convert the temporary buffer to a double value
    double cashAmount = atof(temp);

    if (cashAmount == 0.0) {
      // Handle any invalid cash amount
      fprintf(stderr, "Error: Invalid cash amount in stock.csv\n");
      free(temp);
      free(line);
      return -1;
    }

    struct Shop shop = {0.0};
    shop.cash = cashAmount;
    printf("Read cash amount: %.2f\n", shop.cash);

    // Free the temporary buffer
    free(temp);
  }

  *ptr = line;
  *n = size;

  return len; // Number of characters read
}



void printProduct(struct Product p) {
    printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
    printf("-------------\n");
}

void printCustomer(struct Customer c) {
    printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
    printf("-------------\n");
    for (int i = 0; i < c.index; i++) {
        printProduct(c.shoppingList[i].product);
        printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
        double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
        printf("The cost to %s will be %.2f\n", c.name, cost);
    }
}
struct Shop createAndStockShop() {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("C:/Users/bridc/repo/MultiParadigmProgramming/MPP/G00411262_BridKennedy_Assignment1/stock.csv", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open stock file\n");
        exit(EXIT_FAILURE);
    }

    read = getline(&line, &len, fp);
    float cash = atof(line);
    printf("Cash in shop is %.2f\n", cash);

    struct Shop shop = {cash};

    while ((read = getline(&line, &len, fp)) != -1) {
        char *n = strtok(line, ",");
        char *p = strtok(NULL, ",");
        char *q = strtok(NULL, ",");
        int quantity = atoi(q);
        double price = atof(p);
        char *name = malloc(sizeof(char) * 50);
        if (name == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(name, n);
        struct Product product = {name, price};
        struct ProductStock stockItem = {product, quantity};
        shop.stock[shop.index++] = stockItem;
        printf("Name: %s, Price: %.2f, Quantity: %d\n", name, price, quantity);
    }

    free(line);
    fclose(fp);

    return shop;
}


void readCustomerOrders(struct Customer *customer, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Unable to open customer file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    printf("Reading from %s...\n", filename);

    // Initialize the index member of the customer
    customer->index = 0;

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        char *n = strtok(line, ",");
        int quantity = atoi(strtok(NULL, ","));

        char *name = malloc(sizeof(char) * 50);
        strcpy(name, n);

        struct Product product = {name, 0.0}; // The product code is not specified in the customer file
        struct ProductStock stockItem = {product, quantity};
        customer->shoppingList[customer->index++] = stockItem;

        // Print details of the order being read
        printf("Order: %s, Quantity: %d\n", name, quantity);
    }

    fclose(fp);
    if (line)
        free(line);

    printf("Finished reading from %s\n", filename);
}



void processOrder(struct Shop *shop, struct Customer *customer) {
    for (int i = 0; i < customer->index; i++) {
        struct ProductStock orderItem = customer->shoppingList[i];
        struct ProductStock *shopItem = NULL;

        // Find the corresponding product in the shop
        for (int j = 0; j < shop->index; j++) {
            if (strcmp(orderItem.product.name, shop->stock[j].product.name) == 0) {
                shopItem = &(shop->stock[j]);
                break;
            }
        }

        // Check if the shop can fill the order
        if (shopItem == NULL || shopItem->quantity < orderItem.quantity) {
            printf("Error: Unable to fill order for %s\n", orderItem.product.name);
        } else {
            // Update the shop's cash and stock
            double cost = orderItem.quantity * orderItem.product.price;
            shop->cash += cost;
            shopItem->quantity -= orderItem.quantity;

            printf("Order for %s processed. Cost: %.2f\n", orderItem.product.name, cost);
        }
    }
}


void printShop(struct Shop s) {
    printf("Shop has %.2f in cash\n", s.cash);
    for (int i = 0; i < s.index; i++) {
        printProduct(s.stock[i].product);
        printf("The shop has %d of the above\n", s.stock[i].quantity);
    }
}

void printShopToFile(struct Shop s, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to open file %s for writing\n", filename);
        return;
    }

    printf("Writing to file: %s\n", filename);

    fprintf(file, "Shop has %.2f in cash\n", s.cash);
    for (int i = 0; i < s.index; i++) {
        fprintf(file, "Product: %s, Price: %.2f, Quantity: %d\n",
                s.stock[i].product.name, s.stock[i].product.price, s.stock[i].quantity);
    }

    // Close the file
    fclose(file);

    // Reopen the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s for reading\n", filename);
        return;
    }

    // Print the contents of the file
    printf("Contents of the file:\n");
    int c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    // Close the file
    fclose(file);
}

int main(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        fprintf(stderr, "Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }

    struct Shop shop = createAndStockShop();
    printShopToFile(shop, "shop_details.txt");

     return 0;
}