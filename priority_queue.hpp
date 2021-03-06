////////////
// @author Nick Sorenson A02287085
// References used:
// - https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/Heaps.html
// - https://usu.instructure.com/courses/676471/pages/lecture-notes-and-code-samples?module_item_id=4470875
////////////
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
namespace usu
{
    template <typename T, typename P = unsigned int>
    class priority_queue
    {
      public:
        using priority_type = P;
        using value_type = T;
        using size_type = std::size_t;
        class mypair
        {
          public:
            mypair(value_type v, priority_type p) :
                value(v), priority(p) {}
            mypair() :
                mypair(0, 0) {}
            mypair(std::pair<value_type, priority_type> pair) :
                value(pair.first), priority(pair.second) {}
            value_type value;
            priority_type priority;
        };

        using pointer = std::vector<mypair>*;
        using item_ptr = mypair*;
        using reference = mypair&;
        class iterator : public std::iterator<std::forward_iterator_tag, priority_queue*>
        {

          public:
            using iterator_category = std::forward_iterator_tag;
            // typedef input_iterator_tag iterator_category;
            // using difference_type = std::ptrdiff_t;
            iterator() :
                iterator(nullptr) // DefaultConstructable
            {
            }
            iterator(pointer ptr) :
                m_pos(0),
                m_data_item(ptr)
            {
            }
            iterator(size_type pos, pointer ptr) :
                m_pos(pos),
                m_data_item(ptr) {}
            iterator(const iterator& obj);            // CopyConstructable
            iterator(iterator&& obj) noexcept;        // CopyConstructable, MoveConstructable
            iterator& operator=(const iterator& rhs); // CopyAssignable
            iterator& operator=(iterator&& rhs);      // CopyAssignable, MoveAssignable
            bool operator==(const iterator& rhs) { return m_pos == rhs.m_pos && m_data_item == rhs.m_data_item; }
            bool operator!=(const iterator& rhs) { return m_pos != rhs.m_pos || m_data_item != rhs.m_data_item; }
            iterator operator++();    // incrementable e.g., ++r
            iterator operator++(int); // incrementable e.g., r++
            reference operator*() { return (*m_data_item)[m_pos]; }
            item_ptr operator->()
            {
                mypair p = (*m_data_item)[m_pos];
                mypair* ptr = new mypair(p.value, p.priority);
                return ptr;
            }
            size_type getPosition() { return m_pos; }

          private:
            size_type m_pos;
            pointer m_data_item;
        };

        priority_queue() :
            m_items({}), r_storage(0) {}
        priority_queue(const std::initializer_list<std::pair<value_type, priority_type>>& list);
        void enqueue(T value, priority_type priority)
        {
            // std::cout << "Pushing back item with value " << value << " and priority " << priority << std::endl;
            // if (m_items.size() >= r_storage) {
            //     System.out.println("Heap is full");
            //     return;
            // }
            // Change to iterator end here
            auto curr = static_cast<int>(m_items.size());
            curr = static_cast<int>(curr);
            // Start at end of heap
            // m_items.insert(m_items.end(), std::make_pair(value, priority));
            // mypair* m = new mypair(value, priority);
            m_items.insert(m_items.end(), mypair(value, priority));

            // Now sift up until curr's parent's key > curr's key
            while ((curr != 0) && (compareTo(static_cast<int>(curr), parent(curr))))
            {
                swapItems(static_cast<int>(curr), parent(curr));
                curr = static_cast<int>(parent(curr));
            }
            // debug();
        };
        // std::pair<T, priority_type> deqeue()
        mypair dequeue()
        {
            if (m_items.size() == 0)
            { // removing from empty heap
                throw "heap is empty!";
            }
            // std::cout << "Deqeueing" << std::endl;
            // Swap maximum with last value
            swapItems(0, static_cast<int>(m_items.size() - 1));
            // std::cout << "after swap " << std::endl;
            // debug();
            auto returnPair = m_items[m_items.size() - 1];
            m_items.resize(m_items.size() - 1); // truncate array
            siftdown(0);                        // put new root in correct place
            // std::cout << "returning max value pair: ( " << returnPair.value << " , " << returnPair.priority << " ) " << std::endl;
            return returnPair; // return the pair that was at the end of the array;
        }
        iterator find(T value)
        {
            for (auto i = begin(); i != end(); ++i)
            {
                // std::cout << "in counted for loop" << (*i).value << std::endl;
                if ((*i).value == value)
                {
                    // std::cout << "found the value " << (*i).value << std::endl;
                    return i;
                }
            }
            return end();
        }
        bool empty() { return m_items.size() == 0; }
        void update(iterator i, priority_type priority)
        {
            m_items[i.getPosition()].priority = priority;
            updatePriority(static_cast<int>(i.getPosition()));
        }
        size_type size() { return m_items.size(); }
        void debug()
        {
            std::cout << " --- m_items --- " << std::endl;
            for (auto item : m_items)
            {
                std::cout << "( " << item.value << " , " << item.priority << " )" << std::endl;
            }
        }
        iterator begin()
        {
            return iterator(&m_items);
        }
        iterator end() { return iterator(m_items.size(), &m_items); }
        // std::vector<usu::priority_queue<std::__cxx11::basic_string<char> >::mypair, std::allocator<usu::priority_queue<std::__cxx11::basic_string<char> >::mypair> >&
      private:
        // std::vector<std::pair<T, priority_type>> m_items;
        std::vector<mypair> m_items;
        std::size_t r_storage;
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
            // debug();
        };
        bool isLeaf(int pos)
        {
            return (static_cast<size_type>(pos) >= m_items.size() / 2) && (static_cast<size_type>(pos) < m_items.size());
        }

        // Return position for left child of pos
        int leftchild(int pos)
        {
            if (static_cast<size_type>(pos) >= m_items.size() / 2)
            {
                return -1;
            }
            return 2 * pos + 1;
        }

        // Return position for right child of pos
        int rightchild(int pos)
        {
            if (static_cast<size_type>(pos) >= (m_items.size() - 1) / 2)
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
            return m_items[pos1].priority > m_items[pos2].priority;
        }
        void swapItems(int pos1, int pos2)
        {
            // std::cout << "swapping " << m_items[pos1].second << " with " << m_items[pos2].second << std::endl;
            auto temp = m_items[pos1];
            m_items[pos1] = m_items[pos2];
            m_items[pos2] = temp;
        }
        void updatePriority(int pos)
        {
            // If it is a big value, push it up
            // while ((pos > 0) && (Heap[pos].compareTo(Heap[parent(pos)]) > 0)) {
            while ((pos > 0) && (compareTo(pos, parent(pos))))
            {
                swapItems(pos, parent(pos));
                pos = parent(pos);
            }
            // Swap.swap(Heap, pos, parent(pos));
            // pos = parent(pos);
            // }
            siftdown(pos); // If it is little, push down
        }
    };
    template <typename T, typename P>
    priority_queue<T, P>::priority_queue(const std::initializer_list<std::pair<value_type, priority_type>>& list)
    {

        // if (list.size() > size())
        // {
        //     throw "Initializer list contains too many elements";
        // }
        // unsigned int pos = 0;
        for (auto i = list.begin(); i != list.end(); i++)
        {
            // enqueue(i*);
            this->enqueue(i->first, i->second);
        }
    }
    template <typename T, typename P>
    priority_queue<T, P>::iterator::iterator(const iterator& obj)
    {
        m_data_item = obj.m_data_item;
        m_pos = obj.m_pos;
    }
    template <typename T, typename P>
    priority_queue<T, P>::iterator::iterator(iterator&& obj) noexcept
    {
        m_data_item = obj.m_data_item;
        m_pos = obj.m_pos;
        obj.m_data_item = nullptr;
        obj.m_pos = 0;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator& priority_queue<T, P>::iterator::operator=(const iterator& rhs)
    {
        m_data_item = rhs.m_data_item;
        m_pos = rhs.m_pos;
        return *this;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator& priority_queue<T, P>::iterator::operator=(iterator&& rhs)
    {
        // if (this != rhs)
        // {
        //     std::swap(this->m_data_item, rhs.m_data_item);
        //     std::swap(this->m_pos, rhs.m_pos);
        // }
        std::swap(m_data_item, rhs.m_data_item);
        std::swap(m_pos, rhs.m_pos);
        return *this;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator priority_queue<T, P>::iterator::operator++()
    {
        m_pos++;

        // m_pos = rightchild(m_pos);
        return *this;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator priority_queue<T, P>::iterator::operator++(int)
    {
        iterator i = *this;
        m_pos++;
        return i;
    }
} // namespace usu
