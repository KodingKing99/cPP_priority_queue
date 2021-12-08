#include <iostream>
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
                auto temp = m_items[curr];
                m_items[curr] = m_items[parent(curr)];
                m_items[parent(curr)] = temp;
                curr = parent(curr);
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
            std::cout << "comparing " << m_items[pos1].second << " with " << m_items[pos2].second << std::endl;
            return m_items[pos1].second > m_items[pos2].second;
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
