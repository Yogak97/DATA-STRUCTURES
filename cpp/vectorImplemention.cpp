/*
author : Yoga K
Desc : Vector Implementation (std::vector<t>)

1.  Vector gives the advantages of both array(fast index based access)
    and linked lists(dynamic).

2.  vector initial size/capacity is 0. It grows by double
    when the capacity is reached. In case the vector is initialized with initializer list then the vector grows by
    twice the initial size/capacity of the vector.

********std::vector<typename T> functions*************

at(index)   = execetion safe , throws exception for out of range access.
[]          = not exception safe, if you try to access out of range value, gives garbage


PRO Note:
Note : before accessing any pointer, check if its not a nullptr.
******************************************************
*/

#include <iostream>
using namespace std;

template <typename T>
class vector
{

    size_t iCapacity;
    T *arr = nullptr;
    size_t iSize;

    void reAlloc(const size_t newCapacity)
    {

        // Allocate new memory
        //  copy/move old data
        // delete old memory

        iCapacity = newCapacity;
        T *newArr = new T[newCapacity];

        for (int i = 0; i < iSize; i++)
        {
            newArr[i] = arr[i];
        }

        delete[] arr;
        arr = newArr;
        newArr = nullptr;
    }

public:
    vector() // default constructor
    {
        iCapacity = 2;
        iSize = 0;
        arr = new T[iCapacity];
    }

    vector(const vector<T> &object) // copy constructor
    {
        cout << "Copy" << endl;
    }

    vector(const vector<T> &&object) // move constructor
    {
        cout << "Move" << endl;
    }
    vector<T> &operator=(const vector<T> &obj) // copy assign constructor
    {
        cout << "Copy Assign" << endl;

        return *this;
    }

    vector<T> &operator=(vector<T> &&obj) // move assign constructor
    {
        if (this != &obj)
        {
            delete[] arr;
            arr = obj.arr;
            iCapacity = obj.iCapacity;
            iSize = obj.iSize;
            //  obj.arr = nullptr;
        }
        cout << "Move Assign" << endl;
        return *this;
    }
    ~vector()
    {
        cout << "Destroy" << endl;
        delete[] arr;
    }

    void push_back(const T &iElement)
    {
        if (iSize == iCapacity)
        {
            reAlloc(iCapacity * 2);
        }

        arr[iSize++] = iElement;
    }

    void push_back(T &&iElement /*lvalue*/)
    {
        cout << "Inside push_back move" << endl;
        ;
        if (iSize == iCapacity)
        {
            reAlloc(iCapacity * 2);
        }

        arr[iSize++] = std::move(iElement); // std::move() here calls the move constructor
    }

    void printData() const
    {
        cout << "{ ";
        for (int i = 0; i < iSize; i++)
        {
            cout << arr[i] << " ";
        }
        cout << "}";
    }

    constexpr size_t size()
    {
        return iSize;
    }
    constexpr size_t capacity()
    {
        return iCapacity;
    }

    T &operator[](size_t i)
    {
        return arr[i];
    }

    const T &operator[](size_t i) const
    {
        return arr[i];
    }

    T at(size_t i)
    {
        try
        {
            if (i >= iSize)
            {
                throw i;
                cout << "execetion caught in at() : "
                     << "Invalid index " << i << endl;
            }
            else
            {
                return arr[i];
            }
        }
        catch (size_t i)
        {
            cout << "execetion caught in at(" << i << ") : Invalid index " << endl;
        }

        return -1;
    }
};

int main()
{
    vector<vector<int>> v3;
    // vector<vector<int>> v2 = std::move(v1);
    v3.push_back(vector<int>() /*rvalue*/); // copy/move assign
    vector<int> v1;
    // v1.push_back(4);
    // v1.push_back(5);
    // v1.push_back(6);
    // v1.push_back(6);
    // v1.push_back(6);
    //    cout << "Vector elements :" << v1[2] << endl; //<<v1.vector::at(34)<<endl;
    //     v1[2] = 10;
    // int &b = v1[2];
    // b = 100;
    // v1.printData();

    // vector<int> v2(v1);
    // size_t size = v1.size();
    // size_t capacity = v1.capacity();
    // cout<<"\nsize of the vector is : "<<size<<endl;
    // cout<<"\ncapacity of the vector is : "<<capacity<<endl;

    return 0;
}
