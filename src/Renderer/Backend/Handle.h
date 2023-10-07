#ifndef VULKANPROJECT_HANDLE_H
#define VULKANPROJECT_HANDLE_H

#include "Types.h"

template<HandleType type>
class Handle
{
public:
    Handle(uint16_t id, uint16_t generation)
    {
        uint32_t i = (uint32_t(id) << 16);
        uint32_t g = uint32_t{generation};
        m_idAndGeneration = i | g;
    }

    bool operator()(Handle o1, Handle o2)
    {
        return o1.m_idAndGeneration == o2.m_idAndGeneration;
    };
    uint16_t getId()
    {
        return static_cast<uint16_t>(m_idAndGeneration >> 16);
    }
    uint16_t getGeneration()
    {
        return static_cast<uint16_t>(m_idAndGeneration & uint32_t(std::numeric_limits<uint16_t>::max()));
    }
private:
    uint32_t m_idAndGeneration;
};

/**
 * Class for storing object data that is managed with handles.
 * @tparam type Type of used handles, for example HandleType::Framebuffer
 * @tparam T Type of contained data, for example VkFramebuffer
 */
template<HandleType type, typename T>
class HandleStorage
{
public:
    struct Element
    {
        uint16_t dataGeneration; // Is smaller than generation when data has been destroyed and new data has not yet been added
        uint16_t generation;
        T data;
    };

    HandleStorage(size_t initialSize = 10, size_t bumpAllocationSize = 10) :
        m_bumpAllocationSize(bumpAllocationSize)
    {
        m_list.reserve(initialSize);
    }

    /**
     * Store data in the first free slot. If there are no free slots bump allocate more space
     * @element type Data
     */
    Handle<type> insertElement(const T& element)
    {
        if (m_freeIndices.empty())
        {
            const size_t listSize = m_list.size();
            if (listSize == m_list.capacity())
            {
                m_list.reserve(listSize + m_bumpAllocationSize);
            }
            m_list.push_back(Element{0u, 0u, element});
            return Handle<type>{static_cast<uint16_t>(m_list.size() - 1), m_list.back().generation};
        }
        const uint16_t index = m_freeIndices.top();
        m_freeIndices.pop();
        m_list[index].dataGeneration == m_list[index].generation;
        m_list[index].data = element;
        return Handle<type>{index, m_list[index].generation};
    }

    /**
     * Make the slot free and return the data. Caller is responsible for actually destroying the object
     * @element type Handle
     */
    T popElement(Handle<type> handle)
    {
        const uint16_t id = handle.getId();
        ++m_list[handle.getId()].generation; // Bump generation, dataGeneration is now one smaller
        m_freeIndices.emplace(id);
        return m_list[handle.getId()].second;
    }

    /**
     * Return the data associated with this handle. Exception is thrown if the object has been destroyed.
     * @element handle Handle
     */
    T getElement(Handle<type> handle)
    {
        if (m_list[handle.getId()].generation != handle.getGeneration())
        {
            throw std::runtime_error("Trying to fetch an element that has been destroyed!");
        }
        return m_list[handle.getId()].scond;
    }

    /**
     * Get data that is alive. Useful at desctruction if creator has not destoyed the data
     */
    std::vector<T*> getAliveData()
    {
        std::vector<T*> aliveData;
        aliveData.reserve(m_list.size());
        for (size_t i = 0; i < m_list.size(); ++i)
        {
            if (m_list[i].generation == m_list[i].dataGeneration)
            {
                aliveData.push_back(&m_list[i].data);
            }
        }
        return aliveData;
    }

    void clear()
    {
        while (!m_freeIndices.empty())
        {
            m_freeIndices.pop();
        }
        m_list.clear();
    }
private:
    size_t m_bumpAllocationSize;
    std::vector<Element> m_list;
    std::stack<uint16_t> m_freeIndices;
};

#endif // VULKANPROJECT_HANDLE_H
