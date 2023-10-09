import csv
import statistics  # Import the statistics module for median and mode calculations

def read_data_from_csv(filename, column_name="Score"):
    """
    Read data from a CSV file column and return it as a list of numeric values.

    :param filename: The name of the CSV file to read.
    :param column_name: The name of the column to extract data from (default is "Score").
    :return: A list of numeric values from the specified column in the CSV file.
    """
    data = []
    try:
        with open(filename, mode='r') as file:
            csv_reader = csv.DictReader(file)
            for row in csv_reader:
                value = row.get(column_name)
                if value is not None:
                    try:
                        numeric_value = float(value)  # Convert the value to a float (or int)
                        data.append(numeric_value)
                    except ValueError:
                        print(f"Warning: Non-numeric value '{value}' found in column '{column_name}'. Skipping.")
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
    except Exception as e:
        print(f"An error occurred: {str(e)}")

    return data

if __name__ == "__main__":
    scores = read_data_from_csv('example.csv')

    if scores:
        average = statistics.mean(scores)  # Use statistics.mean() for average
        minimum = min(scores)  # Use min() for minimum value
        maximum = max(scores)  # Use max() for maximum value
        median = statistics.median(scores)  # Use statistics.median() for median
        mode_val = statistics.mode(scores)  # Use statistics.mode() for mode

        print(f'Average: {average} Median: {median} Smallest: {minimum} Largest: {maximum} Mode: {mode_val}')
    else:
        print("No scores found in the CSV file.")
