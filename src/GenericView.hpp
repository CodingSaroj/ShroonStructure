#pragma once

#include <iomanip>
#include <iostream>

#include "GenericIterator.hpp"

/**
 * @brief The namespace in which all the projects under Shroon exist.
 */
namespace Shroon
{
    /**
     * @brief The namespace in which all the ShroonStructure API exists.
     */
    namespace Structure
    {
        /**
         * @defgroup DataStructures
         * This group contains all the data structures in this library.
         * @{
         */

        /**
         * @brief Provides a view into a block of memory containing \p UnitType s.
         *
         * @tparam UnitType The type of data which the provided memory block contains.
         */
        template <typename UnitType>
        class GenericView
        {
        public:
            /**
             * @brief The type of iterator used to iterate this view.
             */
            using Iterator = GenericIterator<UnitType, GenericView<UnitType>>;

            /**
             * @brief The type of iterator used to iterate a const version this view.
             */
            using ConstIterator = GenericIterator<const UnitType, const GenericView<UnitType>>;

            /**
             * @brief Constructs a \p GenericView with memory block as \p data of size \p size bytes.
             *
             * @param data Pointer to the memory block this \p GenericView will view.
             * @param size Size of the memory block this \p GenericView will view.
             */
            GenericView(UnitType * data, size_t size)
                : m_Data(data), m_Size(size)
            {
            }

            /**
             * @brief Returns the pointer to the memory this object views.
             *
             * @return A pointer to \p UnitType which points to the memory this object views.
             */
            inline UnitType * Data()
            {
                return m_Data;
            }

            /**
             * @brief Returns a read-only (const) pointer to the memory this object views.
             *
             * @return A const pointer to \p UnitType which points to the memory this object views.
             */
            inline const UnitType * Data() const
            {
                return m_Data;
            }

            /**
             * @brief Returns the size of memory block this object views.
             *
             * @return Size of memory block this object views.
             */
            inline size_t Size() const
            {
                return m_Size;
            }

            /**
             * @brief Returns a <tt>UnitType &</tt> at index \p index inside the memory block.
             *
             * @param index The index inside the memory block from which data needs to be fetched.
             *
             * @return A <tt>UnitType &</tt> at \p index inside the memory block.
             */
            inline UnitType & At(size_t index)
            {
                return m_Data[index];
            }

            /**
             * @brief Returns a <tt>const UnitType &</tt> at index \p index inside the memory block.
             *
             * @param index The index inside the memory block from which data needs to be fetched.
             *
             * @return A <tt>const UnitType &</tt> at \p index inside the memory block.
             */
            inline const UnitType & At(size_t index) const
            {
                return m_Data[index];
            }

            /**
             * @brief Returns another \p GenericView whose view begins at index \p begin and is of
             * size \p size.
             *
             * @param begin The index inside memory block from where the new \p GenericView will begin.
             * @param size The size of the memory block which the new \p GenericView will view.
             * Defaults to \p SIZE_MAX.
             *
             * @return A \p GenericView viewing memory \p begin bytes after start of this memory
             * block and of size \p size.
             */
            inline GenericView<UnitType> View(size_t begin, size_t size = SIZE_MAX)
            {
                if (size == SIZE_MAX)
                {
                    size = m_Size - begin;
                }
                
                return GenericView<UnitType>(m_Data + begin, size);
            }

            /**
             * @brief Returns another read-only (const) \p GenericView whose view begins at index \p begin and is of size \p size.
             *
             * @param begin The index inside memory block from where the new \p GenericView will begin.
             * @param size The size of the memory block which the new \p GenericView will view.
             * Defaults to \p SIZE_MAX.
             *
             * @return A <tt>const GenericView</tt> viewing memory \p begin bytes after start of this
             * memory block and of size \p size.
             */
            inline const GenericView<UnitType> View(size_t begin, size_t size = SIZE_MAX) const
            {
                if (size == SIZE_MAX)
                {
                    size = m_Size - begin;
                }
                
                return GenericView<UnitType>(m_Data + begin, size);
            }

            /**
             * @brief Returns a <tt>UnitType &</tt> at index \p index inside the memory block.
             *
             * @param index The index inside the memory block from which data needs to be fetched.
             *
             * @return A <tt>UnitType &</tt> at \p index inside the memory block.
             */
            inline UnitType & operator[](size_t index)
            {
                return m_Data[index];
            }

            /**
             * @brief Returns a <tt>const UnitType &</tt> at index \p index inside the memory block.
             *
             * @param index The index inside the memory block from which data needs to be fetched.
             *
             * @return A <tt>const UnitType &</tt> at \p index inside the memory block.
             */
            inline const UnitType & operator[](size_t index) const
            {
                return m_Data[index];
            }

        private:
            UnitType * m_Data;
            size_t m_Size;

            inline bool IsIndexValid(size_t index)
            {
                return index < m_Size;
            }

            inline UnitType & Get(size_t index)
            {
                return m_Data[index];
            }

            inline const UnitType & Get(size_t index) const
            {
                return m_Data[index];
            }

            friend Iterator;
            friend ConstIterator;
        };

        /**
         * @brief It specifically views the contents of memory as bytes (\p uint8_t).
         */
        using MemoryView = GenericView<uint8_t>;

        /**
         * @}
         */
    }
}

/**
 * @defgroup OutputStreamOperators
 * This group contains all output stream operators defined in this library for data structures.
 * @{
 */

/**
 * @brief Outputs the \p view to \p out.
 *
 * @tparam UnitType The type of data which the provided memory block contains. Used for template
 * parameters of \p GenericView.
 *
 * @param out The <tt>std::ostream &</tt> to output the content of \p view to.
 * @param view The \p GenericView<UnitType> whose content needs to be written to \p out.
 *
 * @return \p out after outputting content to it.
 *
 * Suppose the memory block of the view of \p char is:
 * 
 * ```
 * Index: 0 1 2 3 4
 * Value: a b c d e
 * ```
 * 
 * then what <tt>std::cout<<view;</tt> will print to \p stdout is this:
 * 
 * ```
 * 0: a 
 * 1: b
 * 2: c
 * 3: d
 * 4: e
 * ```
 */
template <typename UnitType>
std::ostream & operator<<(std::ostream & out, const Shroon::Structure::GenericView<UnitType> & view)
{
    for (size_t i = 0; i < view.Size(); i++)
    {
        out<<i<<": "<<view[i]<<"\n";
    }

    return out;
}

/**
 * @brief Specialization which outputs the \p view to \p out in a specific way for only \p MemoryView.
 *
 * @param out The <tt>std::ostream &</tt> to output the content of \p view to.
 * @param view The \p MemoryView whose content needs to be written to \p out.
 *
 * @return \p out after outputting content to it.
 *
 * Suppose the contents of memory are:
 *
 * ```
 * Index| 0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16
 * -----+-------------------------------------------------------------------------------------
 * Value| 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10
 * ```
 * 
 * then the output of <tt>std::cout<<memoryView;</tt> will be:
 * 
 * ```
 * 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f
 * 0x10
 * ```
 * 
 * As you can see this type of output is much more expected when memory is being serialized.
 * 
 * <b>Note</b> - The case of the alphabets can be controlled in the usual way using \p std::uppercase
 * and \p std::nouppercase.
 */
template <>
std::ostream & operator<<(std::ostream & out, const Shroon::Structure::MemoryView & view)
{
    for (size_t i = 0; i < view.Size(); i++)
    {
        uint16_t byte = view[i];

        out<<"0x"<<std::hex<<std::setw(2)<<std::setfill('0')<<byte<<std::dec;

        if (i == 0 || (i + 1) % 16 != 0)
        {
            out<<" ";
        }

        if ((i != 0 && (i + 1) % 16 == 0) || i + 1 == view.Size())
        {
            out<<"\n";
        }
    }

    return out;
}

/**
 * @}
 */
