#include <algorithm>
#include <omp.h>


void insertion_sort(double* arr, int64_t left, int64_t right) {
    for (int i = left + 1; i <= right; i++) {
        double key = arr[i];
        int64_t j = i - 1;

        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];

            if (--j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
        }

        arr[j + 1] = key;
    }
}


int64_t partition(double* arr, int64_t left, int64_t right) {
	const int64_t mid = (left + right) / 2;

	if (arr[mid] < arr[left])
		std::swap(arr[left], arr[mid]);

	if (arr[right] < arr[left])
		std::swap(arr[left], arr[right]);

	if (arr[right] < arr[mid])
		std::swap(arr[right], arr[mid]);

	const double pivot = arr[mid];

	int64_t i = left - 1, j = right + 1;


	while (true) {
        do
            i++;
        while (arr[i] < pivot);

        do
            j--;
        while (arr[j] > pivot);
		

        if (i >= j)
			return j;

		std::swap(arr[i], arr[j]);
	}
}


#define insertion_threshold (int64_t)64
void quick_sort(double* arr, int64_t left, int64_t right, int64_t max_threads) {
	int64_t sequential_threshold = (right - left) / (48 * max_threads);

	if (sequential_threshold < 10000)
		sequential_threshold = 10000;

	if (left < right) {
		if ((right - left) < insertion_threshold) {
            insertion_sort(arr, left, right);
            return;
        }

		else if ((right - left) < sequential_threshold) {
			while (left < right) {
				int64_t partition_index = partition(arr, left, right);

				if (partition_index - left < right - partition_index) {
					quick_sort(arr, left, partition_index, max_threads);
					left = partition_index + 1;
				}

				else {
					quick_sort(arr, partition_index + 1, right, max_threads);
					right = partition_index;
				}
			}
		}

		else {
			int64_t partition_index = partition(arr, left, right);

			#pragma omp task
			quick_sort(arr, left, partition_index, max_threads);

			#pragma omp task
			quick_sort(arr, partition_index + 1, right, max_threads);
		}
	}
}


void parallel_quick_sort(double* arr, int64_t size) {
	#pragma omp parallel
	{
		#pragma omp single
		{
			quick_sort(arr, 0, size - 1, omp_get_max_threads());
		}
	}
}
