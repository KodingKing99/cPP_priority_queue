#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>
namespace usu
{
    template <typename T, typename P = unsigned int>
    class priority_queue
    {
        using priority_type = P;
        using size_type = std::size_t;

      private:
        std::vector<std::pair<T, priority_type>> m_items;
        std::size_t r_storage;

      public:
        priority_queue() :
            m_items({}), r_storage(0) {}
        void enqueue(T value, priority_type priority)
        {
            std::cout << "Pushing back item with value " << value << " and priority " << priority << std::endl;
            // if (m_items.size() >= r_storage) {
            //     System.out.println("Heap is full");
            //     return;
            // }
            // Change to iterator end here
            auto curr = m_items.size();
            // Start at end of heap
            m_items.insert(m_items.end(), std::make_pair(value, priority));

            // Now sift up until curr's parent's key > curr's key
            while ((curr != 0) && (compareTo(curr, parent(curr)) > 0))
            {
                swapItems(curr, parent(curr));
                curr = parent(curr);
            }
            debug();
        };
        std::pair<T, priority_type> deqeue()
        {
            if (m_items.size() == 0)
            { // removing from empty heap
                throw std::runtime_error("heap is empty");
            }
            // Swap maximum with last value
            swapItems(0, m_items.size() - 1);
            siftdown(0); // put new root in correct place
            auto returnPair = m_items[m_items.size() - 1];
            m_items.resize(m_items.size() - 1); // truncate array
            return returnPair;                  // return the pair that was at the end of the array;
        }
        void buildheap()
        {
            for (auto i = m_items.size() / 2 - 1; i >= 0; i--)
            {
                siftdown(i);
            }
        }
        void siftdown(int pos)
        {
            if ((pos < 0) || (static_cast<size_type>(pos) >= m_items.size()))
            {
                return;
            } // Illegal position
            while (!isLeaf(pos))
            {
                int j = leftchild(pos);
                if ((static_cast<size_type>(j) < (m_items.size() - 1)) && (!compareTo(j, j + 1)))
                {
                    j++; // j is now index of child with greater value
                }
                if (compareTo(pos, j))
                {
                    return;
                }
                swapItems(pos, j);
                pos = j; // Move down
            }
            debug();
        };
        bool isLeaf(int pos)
        {
            return (pos >= m_items.size() / 2) && (pos < m_items.size());
        }

        // Return position for left child of pos
        int leftchild(int pos)
        {
            if (pos >= m_items.size() / 2)
            {
                return -1;
            }
            return 2 * pos + 1;
        }

        // Return position for right child of pos
        int rightchild(int pos)
        {
            if (pos >= (m_items.size() - 1) / 2)
            {
                return -1;
            }
            return 2 * pos + 2;
        }

        // Return position for parent
        int parent(int pos)
        {
            if (pos <= 0)
            {
                return -1;
            }
            return (pos - 1) / 2;
        }
        bool compareTo(int pos1, int pos2)
        {
            // std::cout << "comparing " << m_items[pos1].second << " with " << m_items[pos2].second << std::endl;
            return m_items[pos1].second > m_items[pos2].second;
        }
        void swapItems(int pos1, int pos2)
        {
            // std::cout << "swapping " << m_items[pos1].second << " with " << m_items[pos2].second << std::endl;
            auto temp = m_items[pos1];
            m_items[pos1] = m_items[pos2];
            m_items[pos2] = temp;
        }
        void debug()
        {
            std::cout << " --- m_items --- " << std::endl;
            for (auto item : m_items)
            {
                std::cout << "( " << item.first << " , " << item.second << " )" << std::endl;
            }
        }
        size_type size() { return m_items.size(); }
    };

} // namespace usu
