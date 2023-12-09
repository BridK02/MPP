#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *line = NULL;
size_t len = 0;
ssize_t read;

// Read the first line of the file
read = getline(&line, &len, fp);
if (read == -1) {
  printf("Error: Unable to read the initial cash value from the stock file\n");
  free(line);
  exit(EXIT_FAILURE);
}

// Check if the line is empty
if (line[0] == '\0') {
  printf("Error: Invalid cash value: empty line\n");
  free(line);
  exit(EXIT_FAILURE);
}

// Convert the line to a double
double cash;
if (sscanf(line, "%lf", &cash) != 1) {
  printf("Error: Invalid cash value: not a decimal number\n");
  free(line);
  exit(EXIT_FAILURE);
}

// Check if the converted value is positive
if (cash <= 0.0) {
  printf("Error: Invalid cash value: not a positive number\n");
  free(line);
  exit(EXIT_FAILURE);
}