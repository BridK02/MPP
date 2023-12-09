#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h> 
#include "HTUtils.h"


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
        free(line);
        return -1; // No more lines to read or error
    }

    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0'; // Remove newline if present
    }

    *ptr = line;
    *n = size;

    return len; // Number of characters read
}

struct Product {
    char *name;
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
    char *name;
    double budget;
    struct ProductStock shoppingList[10];
    int index;
};

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
    struct Shop shop = {0.0};

    FILE *fp = fopen("stock.csv", "r");
    if (fp == NULL) {
        printf("Error: Unable to open stock file (fopen failed)\n");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t lineLen = 0;
    ssize_t read;

    // Print a debug statement to verify that the function is being called
    printf("Reading from stock.csv...\n");

    while ((read = custom_getline(&line, &lineLen, fp)) != -1) {
        char *name = strtok(line, ",");
        char *price_str = strtok(NULL, ",");
        char *quantity_str = strtok(NULL, ",");

        if (name != NULL && price_str != NULL && quantity_str != NULL) {
            double price = atof(price_str);
            int quantity = atoi(quantity_str);

            struct Product product = {strdup(name), price};
            struct ProductStock stockItem = {product, quantity};
            shop.stock[shop.index++] = stockItem;

            // Print the product details
            printf("Read: %s, %.2f, %d\n", name, price, quantity);
        } else {
            printf("Error reading line: %s\n", line);
            printf("Tokens: %s, %s, %s\n", name, price_str, quantity_str);
        }
    }

    // Print a debug statement to verify that the loop has ended
    printf("Finished reading from stock.csv\n");

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

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        char *n = strtok(line, ",");
        char *p = strtok(NULL, ",");
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
    struct Shop shop = createAndStockShop();
    printShopToFile(shop, "shop_details.txt");

    struct Customer john;
    readCustomerOrders(&john, "customer_positive.csv");
    printCustomer(john);

    processOrder(&shop, &john);
    printShop(shop);

    return 0;
}
