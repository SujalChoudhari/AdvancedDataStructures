#include <iostream>

class BinaryCounter
{
private:
    int m_NumberOfBits;
    int *m_BinNumberHolderArray;
    int m_Credit = 0;
    int m_ActualCost = 0;

public:
    BinaryCounter(int numberOfBits)
        : m_NumberOfBits(numberOfBits)
    {
        m_BinNumberHolderArray = (int *)calloc(numberOfBits, sizeof(int));
    }

public:
    void Increment()
    {
        int i = 0;
        int _amortizedCost = 0;
        int _actualCost = 0;
        while (i < m_NumberOfBits && m_BinNumberHolderArray[i] == 1)
        {
            m_BinNumberHolderArray[i] = 0;
            i += 1;

            // Cost Calculations
            _actualCost++;
        }

        if (i < m_NumberOfBits)
        {
            m_BinNumberHolderArray[i] = 1;

            // Cost Calculations
            _actualCost++;
            _amortizedCost += 2;
        }

        m_ActualCost += _actualCost;
        m_Credit += _amortizedCost - _actualCost;
    }

    void DisplayNumber()
    {
        for (int i = 0; i < m_NumberOfBits; i++)
        {
            std::cout << m_BinNumberHolderArray[i] << " ";
        }
        std::cout << std::endl;
    }

    void DisplayCosts(int rotations)
    {
        std::cout << "Actual Cost: " << m_ActualCost << std::endl;
        std::cout << "Credits: " << m_Credit << std::endl;
        std::cout << "Aggregrate: " << m_ActualCost * 1.0 / rotations * 1.0 << std::endl;
    }

    ~BinaryCounter()
    {
        free(m_BinNumberHolderArray);
    }
};

int main()
{
    std::cout << "Enter No of Bits >> ";
    int _NoOfBits = 0;
    std::cin >> _NoOfBits;
    BinaryCounter binaryCounter = BinaryCounter(_NoOfBits);
    std::cout << std::endl;

    // Work here
    for (int i = 0; i < 10; i++)
    {
        binaryCounter.Increment();
        binaryCounter.DisplayNumber();
        binaryCounter.DisplayCosts(i + 1);
        std::cout << std::endl;
    }

    return 0;
}