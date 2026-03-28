#include <cstdint>
#include <omp.h>

using std::size_t;


// получение значения бита по позиции pos справа
inline uint64_t get_bit(uint64_t x, int pos) {
    return (x >> pos) & 1ULL;
}


// инвертирование отрицательных чисел, инвертирование старшего бита у положительных
inline uint64_t transform(uint64_t bits) {
    if (bits & (1ULL << 63))
        return ~bits;

    else
        return bits ^ (1ULL << 63);
}

// у "отрицательных" (изначально положительные) чисел инвертируем старший бит, 
// "положительные" (изначально отрицательные) числа инвертируем
inline uint64_t untransform(uint64_t bits) {
    if (bits & (1ULL << 63))
        return bits ^ (1ULL << 63);

    else
        return ~bits;
}


// сортировка подсчётом
// идея: делать сортировку не по битам, а по байтам
inline void counting_sort(uint64_t* arr_bits, uint64_t* temp_bits, size_t size, int bit) {
    size_t count[2] = {0, 0};
    
    for (size_t i = 0; i < size; i++)
        count[get_bit(arr_bits[i], bit)]++;
    
    count[1] += count[0];

    for (long int i = size - 1; i >= 0; i--) {
        int bit_value = get_bit(arr_bits[i], bit);
        int index = --count[bit_value];
        temp_bits[index] = arr_bits[i];
    }
}


void radix_sort(double* arr, size_t size) {
    if (size <= 1) 
        return;

    uint64_t* arr_bits = reinterpret_cast<uint64_t*>(arr);
    double* temp = new double[size];
    uint64_t* temp_bits = reinterpret_cast<uint64_t*>(temp);

    
    // преобразуем исходный массив
    for (size_t i = 0; i < size; i++)
        arr_bits[i] = transform(arr_bits[i]);

    // работаем с преобразованными double с использованием дополнительной памяти
    // идея: делить массив на n частей и сортировать их параллельно
    for (int bit = 0; bit < 64; bit++) {
        counting_sort(arr_bits, temp_bits, size, bit);

        for (size_t i = 0; i < size; i++)
            arr_bits[i] = temp_bits[i];
    }
    
    // делаем обратное преобразование
    for (size_t i = 0; i < size; i++)
        arr_bits[i] = untransform(arr_bits[i]);
            
    delete[] temp;
}
