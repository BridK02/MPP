def bubble_sort(lst):
    """
    Sort a list of numbers in ascending order using the bubble sort algorithm.

    :param lst: A list of numbers to be sorted.
    """
    for i in range(0, len(lst) - 1):
        for j in range(len(lst) - 1):
            if lst[j] > lst[j + 1]:
                temp = lst[j]
                lst[j] = lst[j + 1]
                lst[j + 1] = temp