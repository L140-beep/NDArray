#include <iostream>
#include <random>

struct indexes {
    int x; //количество стобцов
    int y; //количество строк
};

class NDArray {

private:
    int* arr;
    int size_y; 
    int size_x; 

protected:
    void filling(int a) {
        for (int i = 0; i < size_x * size_y; i++) {
            arr[i] = a;
        }
    }

    void rand_filling(int start, int end) {
        for (int i = 0; i < size_x * size_y; i++) {
            arr[i] = rand() % (end - start + 1) + start;
        }
    }

    NDArray _operator(char oper, const NDArray rv) {
        NDArray temp_arr(indexes{ this->size_x,  this->size_y });
        temp_arr.zeros();
        if (rv.size_y == this->size_y && rv.size_x == this->size_x) {
            for (int i = 0; i < size_y * size_x; i++) {
                switch (oper) {
                case '+':
                    temp_arr.arr[i] = arr[i] + rv.arr[i];
                    break;
                case '-':
                    temp_arr.arr[i] = arr[i] - rv.arr[i];
                    break;
                case '/':
                    temp_arr.arr[i] = arr[i] / rv.arr[i];
                    break;
                case '*':
                    temp_arr.arr[i] = arr[i] * rv.arr[i];
                    break;
                case '=':
                    arr[i] = rv.arr[i];
                    break;
                default:
                    throw "unknown operator!";
                    break;
                }
            }

            return temp_arr;
        }
        else {
            throw "size_y != rv.size_y or size_x != rv.size_x";
        }
    }

    NDArray int_operator(char oper, int value) {
        NDArray rv(indexes{ this->size_x, this->size_y });
        rv.create();
        rv.filling(value);
        return _operator(oper, rv);
    }

    void reshape(int size_y, int size_x) {
        int* new_arr = new int[size_y * size_x];

        for (int i = 0; i < size_y * size_x; i++) {
            new_arr[i] = this->arr[i];
        }

        delete[] arr;
        arr = new_arr;
        this->size_y = size_y;
        this->size_x = size_x;
    }

    int normalize_index(int x, int y) {
        return (x * this->size_x + y);
    }
public:

    NDArray(indexes index_ = { 1, 10 }) {
        size_x = index_.x; //столбцы
        size_y = index_.y; //строки
    }

    void create() {
        arr = new int[size_y * size_x];
    }

    void ones() {
        create();
        filling(1);
    }

    void zeros() {
        create();
        filling(0);
    }

    void random(int start = 0, int end = RAND_MAX) {
        create();
        rand_filling(start, end);
    }

    const NDArray operator=(const NDArray& rv) {
        if (rv.size_y == this->size_y && rv.size_x == this->size_x) {
            return _operator('=', rv);
        }
        else {
            this->reshape(rv.size_y, rv.size_x);
            return _operator('=', rv);
        }
    }

    const NDArray operator+(const NDArray& rv) {
        return _operator('+', rv);
    }

    const NDArray operator-(const NDArray& rv) {
        return _operator('-', rv);
    }

    const NDArray operator*(const NDArray& rv) {
        return _operator('*', rv);
    }
    const NDArray operator/(const NDArray& rv) {
        return _operator('/', rv);
    }

    const NDArray operator+(int value) {
        return int_operator('+', value);
    }

    const NDArray operator-(int value) {
        return int_operator('-', value);
    }

    const NDArray operator*(int value) {
        return int_operator('*', value);
    }

    const NDArray operator/(int value) {
        return int_operator('/', value);
    }

    int& operator[](indexes index) {
        if (index.y > 0 && index.x > 0 && index.y <= size_y && index.x <= size_x) {
            return arr[(index.y - 1) * this->size_x + index.x - 1];
        }
        else {
            throw "out of index!";
        }
    }


    NDArray sum(int shape) {
        int value = 0;
        if (shape == 0) {
            NDArray new_arr({ this->size_x, 1 });
            new_arr.zeros();
            for (int i = 0; i < this->size_x; i++) {
                value = 0;
                for (int j = 0; j < this->size_y; j++) {
                    value += this->arr[normalize_index(j, i)];
                }
                new_arr[{i + 1, 1}] = value;
            }
            return new_arr;
        }
        else {
            if (shape == 1) {
                NDArray new_arr({ this->size_y, 1});
                new_arr.zeros();
                for (int i = 0; i < this->size_y; i++) {
                    value = 0;
                    for (int j = 0; j < this->size_x; j++) {
                        value += this->arr[normalize_index(i, j)];
                    }
                    new_arr[{ i + 1, 1}] = value;
                }

                return new_arr;
            }
            else {
                throw "Not supported!";
            }

        }

    }

    NDArray min(int shape) {
        int min = 0;
        switch (shape) {
        case 0:
        {
            NDArray new_arr({ this->size_x, 1 });
            new_arr.zeros();
            for (int i = 0; i < this->size_x; i++) {
                min = this->arr[normalize_index(1, i)];
                for (int j = 0; j < this->size_y; j++) {
                    if (this->arr[normalize_index(j, i)] < min) {
                        min = this->arr[normalize_index(j, i)];
                    }
                }
                new_arr[{i + 1, 1}] = min;
            }
            return new_arr;
            break;
        }
        case 1:
        {
            NDArray new_arr({ this->size_y, 1 });
            new_arr.zeros();
            for (int i = 0; i < this->size_y; i++) {
                min = this->arr[normalize_index(i, 1)];
                for (int j = 0; j < this->size_x; j++) {
                    if (this->arr[normalize_index(j, i)] < min) {
                        min = this->arr[normalize_index(i, j)];
                    }
                }
                new_arr[{ i + 1, 1}] = min;
            }

            return new_arr;
            break;
        }
        default:
            throw "Not supported!";
        }

    }

    NDArray max(int shape) {
        int max = 0;
        switch (shape) {
        case 0:
        {
            NDArray new_arr({ this->size_x, 1 });
            new_arr.zeros();
            for (int i = 0; i < this->size_x; i++) {
                max = this->arr[normalize_index(1, i)];
                for (int j = 0; j < this->size_y; j++) {
                    if (this->arr[normalize_index(j, i)] > max) {
                        max = this->arr[normalize_index(j, i)];
                    }
                }
                new_arr[{i + 1, 1}] = max;
            }
            return new_arr;
            break;
        }
        case 1:
        {
            NDArray new_arr({ this->size_y, 1 });
            new_arr.zeros();
            for (int i = 0; i < this->size_y; i++) {
                max = this->arr[normalize_index(i, 1)];
                for (int j = 0; j < this->size_x; j++) {
                    if (this->arr[normalize_index(i, j)] > max) {
                        max = this->arr[normalize_index(i, j)];
                    }
                }
                new_arr[{ i + 1, 1}] = max;
            }

            return new_arr;
            break;
        }
        default:
            throw "Not supported!";
        }
    }

    NDArray avg(int shape) {
        int value = 0;
        if (shape == 0) {
            NDArray new_arr({ this->size_x, 1 });
            new_arr.zeros();
            for (int i = 0; i < this->size_x; i++) {
                value = 0;
                for (int j = 0; j < this->size_y; j++) {
                    value += this->arr[normalize_index(j, i)];
                }
                new_arr[{i + 1, 1}] = value / size_y;
            }
            return new_arr;
        }
        else {
            if (shape == 1) {
                NDArray new_arr({ this->size_y, 1 });
                new_arr.zeros();
                for (int i = 0; i < this->size_y; i++) {
                    value = 0;
                    for (int j = 0; j < this->size_x; j++) {
                        value += this->arr[normalize_index(i, j)];
                    }
                    new_arr[{ i + 1, 1}] = value / size_x;
                }

                return new_arr;
            }
            else {
                throw "Not supported!";
            }

        }
    }

    NDArray transpone() {
        NDArray new_arr({ this->size_y, this->size_x });
        new_arr.create();
        for (int i = 0; i < this->size_x; i++) {
            for (int j = 0; j < this->size_y; j++) {
                new_arr[{j + 1, i + 1}] = this->arr[normalize_index(j, i)];
            }
        }

        return new_arr;
    }

    NDArray matmul(NDArray& other) {
        if (this->size_y == other.size_x && this->size_x == other.size_y) {
            NDArray new_arr({ size_y, size_y });
            new_arr.create();
            int value = 0;
            for (int i = 0; i < size_y; i++) {
                for (int j = 0; j < size_y; j++) {
                    value = 0;
                    for (int k = 0; k < size_x; k++) {
                        value += arr[normalize_index(i, k)] * other[{j + 1, k + 1}];
                    }

                    new_arr[{j + 1, i + 1}] = value;
                }
            }

            return new_arr;
        }
        else {
            throw "other size!";
        }
    }

    void display() {
        for (int i = 0; i < this->size_y; i++) {
            for (int j = 0; j < this->size_x; j++) {
                std::cout << arr[this->size_x * i + j] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

int main()
{
    indexes index = { 2, 3 }; //2 столбца, 3 строки 
    NDArray a1(index);
    a1.random(1, 5);
    NDArray a2({3, 2});
    std::cout << "ones(): " << std::endl;
    a2.ones();
    a2.display();
    NDArray a3({ 3, 2 });
    std::cout << "zeros(): " << std::endl;
    a3.zeros();
    a3.display();
    std::cout << "transpone(): " << std::endl;
    a1.display();
    a1.transpone().display();
    NDArray a4;
    a4.create();
    std::cout << "a3 + a3" << std::endl;
    a4 = a2 + a2;
    a4.display();
    a4 = a4 - a2;
    std::cout << "a4 - a2" << std::endl;
    a4.display();
    std::cout << "a4 + 2" << std::endl;
    a4 = a4 + 2;
    a4.display();
    std::cout << "a4 / (a2 + 1)" << std::endl;
    a4 = a4 / (a2 + 1);
    a4.display();
    a4.random(1, 5);
    std::cout << "matmul: " << std::endl;
    a4.matmul(a1).display();


    a4.display();
    std::cout << "min(0): " << std::endl;
    a4.min(0).display();
    std::cout << "min(1): " << std::endl;
    a4.min(1).display();

    std::cout << "sum(0): " << std::endl;
    a4.sum(0).display();
    std::cout << "sum(1): " << std::endl;
    a4.sum(1).display();
    std::cout << "max(0): " << std::endl;
    a4.max(0).display();
    std::cout << "max(1): " << std::endl;
    a4.max(1).display();
    std::cout << "average(0): " << std::endl;
    a4.avg(0).display();
    std::cout << "average(1): " << std::endl;
    a4.avg(1).display();

}
