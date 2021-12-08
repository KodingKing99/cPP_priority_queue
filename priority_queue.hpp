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
            m_items.push_back(std::make_pair(value, priority));
        };
        size_type size() { return m_items.size(); }
    };

} // namespace usu
