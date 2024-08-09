#include <iostream>
#include <vector>

class PotentialBasedBinaryCounter
{
private:
    int m_NumberOfBits;
    std::vector<int> m_BinNumberHolderArray;
    int m_ActualCost = 0;
    int m_Potential = 0;

public:
    PotentialBasedBinaryCounter(int numberOfBits)
        : m_NumberOfBits(numberOfBits), m_BinNumberHolderArray(numberOfBits, 0) {}

    void Increment()
    {
        int i = 0;
        int _actualCost = 0;
        int initialPotential = CalculatePotential();
        while (i < m_NumberOfBits && m_BinNumberHolderArray[i] == 1)
        {
            m_BinNumberHolderArray[i] = 0;
            i += 1;

            _actualCost++;
        }

        if (i < m_NumberOfBits)
        {
            m_BinNumberHolderArray[i] = 1;
            _actualCost++;
        }
        m_ActualCost += _actualCost;
        int finalPotential = CalculatePotential();
        m_Potential += (finalPotential - initialPotential);
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
        std::cout << "Potential: " << m_Potential << std::endl;
        std::cout << "Aggregate Cost: " << (m_ActualCost * 1.0 / rotations) << std::endl;
    }

private:
    /**
        Potential function is proportional to the no of 1's in the sequence
     */
    int CalculatePotential()
    {
        int potential = 0;
        for (int bit : m_BinNumberHolderArray)
        {
            if (bit == 1)
            {
                potential++;
            }
        }
        return potential;
    }
};

int main()
{
    std::cout << "Enter No of Bits for Potential Based ANalysis >> ";
    int _NoOfBits = 0;
    std::cin >> _NoOfBits;
    PotentialBasedBinaryCounter binaryCounter = PotentialBasedBinaryCounter(_NoOfBits);
    std::cout << std::endl;

    for (int i = 0; i < 10; i++)
    {
        binaryCounter.Increment();
        binaryCounter.DisplayNumber();
        binaryCounter.DisplayCosts(i + 1);
        std::cout << std::endl;
    }

    return 0;
}