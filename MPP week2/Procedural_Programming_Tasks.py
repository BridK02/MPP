
import csv
from Sorting_algorithm import bubble_sort

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

def get_maximum_value(lst):
    """
    Get the maximum value from a list of numbers.

    :param lst: A list of numbers.
    :return: The maximum value from the list.
    """
    maximum = lst[0]
    for l in lst:
        if maximum > l:
            maximum = l
    return maximum

def get_minimum_value(lst):
    """
    Get the minimum value from a list of numbers.

    :param lst: A list of numbers.
    :return: The minimum value from the list.
    """
    minimum = lst[0]
    for l in lst:
        if minimum < l:
            minimum = l

def get_average(lst):
    """
    Calculate the average of a list of numbers.

    :param lst: A list of numbers.
    :return: The numerical average of the list.
    """
    total = 0
    for l in lst:
        total += l
    average = total / len(lst)
    return average

def get_median_value(lst):
    """
    Calculate the median value from a list of numbers.

    :param lst: A list of numbers.
    :return: The median value from the list.
    """
    list1 = lst.copy()
    bubble_sort(list1)
    median = list1[int(len(list1) / 2)]
    return median

def get_range(lst):
    """
    Calculate the range of a list of numbers.

    :param lst: A list of numbers.
    :return: The range, which is the difference between the maximum and minimum values.
    """
    if not lst:
        return 0  # Handle empty list
    maximum = minimum = lst[0]
    for l in lst:
        if l > maximum:
            maximum = l
        elif l < minimum:
            minimum = l
    return maximum - minimum

def get_variance(lst):
    """
    Calculate the variance of a list of numbers.

    :param lst: A list of numbers.
    :return: The variance of the list.
    """
    if not lst:
        return 0  # Handle empty list
    mean = get_average(lst)
    squared_differences = [(x - mean) ** 2 for x in lst]
    variance = sum(squared_differences) / len(lst)
    return variance


def get_mode(lst):
    """
    Calculate the mode value from a list of numbers.

    :param lst: A list of numbers.
    :return: The mode value from the list, or None if there's no mode.
    """
    highest_freq = 0
    mode = lst[0]
    for score in lst:
        frequency = 0
        for score2 in lst:
            if score == score2:
                frequency += 1
        if frequency > highest_freq:
            mode = score
            highest_freq = frequency
    return mode

def read_scores_from_csv(filename):
    """
    Read scores from a CSV file and return them as a list of integers.

    :param filename: The name of the CSV file to read.
    :return: A list of integer scores read from the CSV file.
    """
    scores = []
    with open(filename, mode='r') as file:
        csvFile = csv.DictReader(file)
        for lines in csvFile:
            score = int(lines["Score"])
            scores.append(score)
    return scores

if __name__ == "__main__":
    scores = read_scores_from_csv('example.csv')

    average = get_average(scores)
    minimum = get_minimum_value(scores)
    maximum = get_maximum_value(scores)
    median = get_median_value(scores)
    mode = get_mode(scores)
    data_range = get_range(scores)
    data_variance = get_variance(scores)
    scores = read_data_from_csv('example.csv', column_name="Score")
    # Or, to read data from a different column (e.g., "OtherColumn"):
    # other_data = read_data_from_csv('example.csv', column_name="OtherColumn")

    # Perform calculations or analysis on the 'scores' or 'other_data' list as needed.
    # For example, calculating the average:
    average = get_average(scores)


    print(f'Average: {average}')
    print(f'Average: {average} Median: {median} Smallest: {minimum} Largest: {maximum} mode: {mode}')
    print(f'Data Range: {data_range} Data Variance: {data_variance}')
