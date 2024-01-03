#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "HTUtils.h"
#include <unistd.h>
#include <ctype.h>
#include <wchar.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/sysinfo.h>
#endif


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

ssize_t custom_getline(char **ptr, size_t *n, FILE *stream, int skipFirstLine) {
  size_t size = *n;
  char *line = *ptr;

  if (line == NULL) {
    line = malloc(size);
    if (line == NULL) {
      return -1; // Error in allocation
    }
  }

  // Skip the first line if specified
  if (skipFirstLine) {
    skipFirstLine = 0;
    free(line);
    line = NULL;
    size = 0;
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

  *ptr = line;
  *n = size;

  return len; // Number of characters read
  printf("custom_getline: Read %zd characters: %s\n", len, line);
}


void printProduct(struct Product p) {
    printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
    printf("-------------\n");
}

void printCustomer(struct Customer c) {
    printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.shoppingList[0].product.price);
    printf("-------------\n");
    for (int i = 1; i < c.index; i++) {
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
    //printf("Cash in shop is %.2f\n", cash);

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
        //printf("Name: %s, Price: %.2f, Quantity: %d\n", name, price, quantity);
    }

    free(line);
    fclose(fp);

    return shop;
}

void processOrder(struct Shop *shop, struct Customer *customer) {
    for (int i = 0; i < customer->index; i++) {
        struct ProductStock orderItem = customer->shoppingList[i];
        struct ProductStock *shopItem = NULL;

        // Find the corresponding product in the shop (case-insensitive comparison)
        for (int j = 0; j < shop->index; j++) {
        #ifdef _WIN32
                    if (_stricmp(orderItem.product.name, shop->stock[j].product.name) == 0) {
        #else
                    if (strcasecmp(orderItem.product.name, shop->stock[j].product.name) == 0) {
        #endif
                        shopItem = &(shop->stock[j]);
                        break;
                    }
                }

        // Check if the shop can fill the order
        if (shopItem == NULL || shopItem->quantity < orderItem.quantity) {
            printf("Error: Insufficient stock for %s. Available: %d, Ordered: %d\n",
                    orderItem.product.name, shopItem ? shopItem->quantity : 0, orderItem.quantity);
        } else {
            // Update the shop's cash and stock
            double cost = orderItem.quantity * orderItem.product.price;
            shop->cash += cost;
            shopItem->quantity -= orderItem.quantity;

            printf("Order for %s processed. Cost: %.2f\n", orderItem.product.name, cost);
            printf("Updated shop cash: %.2f\n", shop->cash);
            printf("Updated stock for %s: %d\n", orderItem.product.name, shopItem->quantity);
        }
    }
}


 // Function to get available system memory (cross-platform)
int getFreeMemory() {
    int freeMemory = 0;

#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    freeMemory = (int)(memInfo.ullAvailPhys / (1024 * 1024));  // Convert to megabytes
#else
    struct sysinfo info;
    sysinfo(&info);
    freeMemory = (int)(info.freeram / (1024 * 1024));  // Convert to megabytes
#endif

    return freeMemory;
}
 
void readCustomerOrders(struct Customer *customer, const char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open customer file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    read = getline(&line, &len, fp); // Skip the header line

    // Process the remaining lines
    while (getline(&line, &len, fp) != -1) {
        char *productName = strtok(line, ",");
        int quantity = atoi(strtok(NULL, ","));

        struct Product product = {strdup(productName), 0.0}; // The product code is not specified in the customer file
        struct ProductStock stockItem = {product, quantity};
        customer->shoppingList[customer->index++] = stockItem;

        // Print details of the order being read
        printf("Order: %s, Quantity: %d\n", productName, quantity);
    }

    fclose(fp);
    if (line)
        free(line);

    printf("Finished reading from %s\n", filename);
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

void lowercaseString(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Function to read a line from the standard input
char *readLine() {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Read the line
    if ((read = getline(&line, &len, stdin)) != -1) {
        // Remove newline character, if present
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        return line;
    } else {
        free(line);
        return NULL;
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void liveMode(struct Shop *shop) {
    char productName[50];
    int quantity;

    while (1) {
        printf("Enter product name (or 'exit' to end): ");

        // Read the line and trim leading/trailing whitespaces
        char *input = readLine();
        if (input == NULL) {
            printf("Error reading input. Exiting live mode.\n");
            break;
        }

        // Convert the input to lowercase
        for (int i = 0; input[i]; i++) {
            input[i] = tolower((unsigned char)input[i]);
        }

        if (strcmp(input, "exit") == 0) {
            free(input);
            break; // Exit the loop if 'exit' is entered
        }

        strcpy(productName, input);
        free(input);

        printf("Enter quantity of %s: ", productName);
        if (scanf("%d", &quantity) != 1) {
            printf("Invalid quantity. Exiting live mode.\n");
            break;
        }

        clearInputBuffer(); // Clear the input buffer after scanf

        // Find the corresponding product in the shop (case-insensitive comparison)
        struct ProductStock *shopItem = NULL;
        for (int j = 0; j < shop->index; j++) {
            char stockProductName[50];
            strcpy(stockProductName, shop->stock[j].product.name);

            // Convert the stock product name to lowercase
            for (int i = 0; stockProductName[i]; i++) {
                stockProductName[i] = tolower((unsigned char)stockProductName[i]);
            }

            if (strcmp(productName, stockProductName) == 0) {
                shopItem = &(shop->stock[j]);
                break;
            }
        }

        if (shopItem == NULL) {
            printf("Sorry, the shop does not have this product in stock\n");
        } else {
            // Check if the stock is sufficient
            if (quantity > shopItem->quantity) {
                printf("Sorry, the shop does not have enough stock for %d units of %s\n", quantity, productName);
            } else {
                // Calculate the cost of the order
                double cost = quantity * shopItem->product.price;

                printf("The cost for %d units of %s is %.2f EUR\n", quantity, productName, cost);

                printf("Do you want to buy this? (yes/no): ");
                char confirm[3];
                if (scanf("%s", confirm) != 1) {
                    printf("Invalid input. Exiting live mode.\n");
                    break;
                }

                clearInputBuffer(); // Clear the input buffer after scanf

                if (strcasecmp(confirm, "yes") == 0) {
                    // Update the shop's cash and stock
                    shop->cash += cost;
                    shopItem->quantity -= quantity;

                    printf("Purchase successful! Shop's cash: %.2f EUR, %s in stock: %d\n", shop->cash, productName, shopItem->quantity);
                } else {
                    printf("Purchase canceled.\n");
                }
            }
        }
    }
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

    //struct Customer emer;
    //readCustomerOrders(&emer, "customer_negative.csv");
    //printCustomer(emer);

    // Add a debug print here to check if the program reaches this point
    //printf("main: Finished processing customer orders\n");

    //processOrder(&shop, &emer);
    //printShop(shop);

    liveMode(&shop);

     return 0;
}