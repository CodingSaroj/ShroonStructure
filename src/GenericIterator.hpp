#pragma once

#include <iostream>

#include <cstddef>
#include <cstdint>

#include "ErrorReporter.hpp"

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
         * @brief Generic iterator interface that works with any data structure class that exposes
         * certain functions.
         *
         * @tparam ValueType The type of an element inside \p StructureType.
         * @tparam StructureType The type of the structure the iterator will be iterating.
         *
         * A data structure which needs to be iterated by this iterator needs to follow the following
         * criteria:
         * 
         * 1. The structure should have a way to access all the elements using unique continuous
         *    unsigned indices.
         * 2. Implement <tt>bool IsIndexValid(size_t index) const</tt> function which returns if the
         *    given index is valid.
         * 3. Implement <tt>ValueType & Get(size_t index)</tt> which returns the element which belongs
         *    to the given index. \p ValueType must be the type which is passed as first template
         *    parameter to \p GenericIterator. A const version of this function i.e.
         *    <tt>const ValueType & Get(size_t index) const</tt> is also required.
         * 
         * The iterator declares the \p StructureType as a friend class so, the data structure can
         * access private members of the iterator.
         * 
         * Unless the required functions are part of the API for the structure, it is recommended to implement
         * these functions as private members and add \p GenericIterator as a friend class.
         */
        template <typename ValueType, typename StructureType>
        class GenericIterator
        {
        public:
            /**
             * @brief Checks if the iterator points to a valid element.
             *
             * @return \p true if the iterator points to a valid element and \p false if it is
             * invalid.
             */
            inline bool Valid()
            {
                return m_Valid;
            }

            /**
             * @brief Returns an l-value reference to the data pointed by this iterator if it is
             * valid otherwise reports an error.
             *
             * @return An l-value reference to \p ValueType which is pointed by this iterator.
             */
            inline ValueType & Base()
            {
                if (Valid())
                {
                    ErrorReporter("Invalid call to Base() on an invalid GenericIterator.", true);
                }

                return m_Structure->Get(m_Index);
            }

            /**
             * @brief Checks if the iterator points to a valid element.
             *
             * @return \p true if the iterator points to a valid element and \p false if it is
             * invalid.
             */
            inline operator bool()
            {
                return m_Valid;
            }

            /**
             * @brief Returns an l-value reference to the data pointed by this iterator.
             *
             * @return An l-value reference to \p ValueType which is pointed by this iterator.
             */
            inline ValueType & operator*()
            {
                return m_Structure->Get(m_Index);
            }

            /**
             * @brief Increments the element index of this iterator and returns an unchanged copy.
             * 
             * @return The unchanged copy of this iterator.
             */
            inline GenericIterator<ValueType, StructureType> operator++(int)
            {
                auto copy = *this;
                m_Index++;
                m_Valid = m_Structure->IsIndexValid(m_Index);
                return copy;
            }

            /**
             * @brief Decrements the element index of this iterator and returns an unchanged copy.
             * 
             * @return The unchanged copy of this iterator.
             */
            inline GenericIterator<ValueType, StructureType> operator--(int)
            {
                auto copy = *this;
                m_Index--;
                m_Valid = m_Structure->IsIndexValid(m_Index);
                return copy;
            }

            /**
             * @brief Increments the element index of this iterator.
             */
            inline void operator++()
            {
                m_Index++;
            }

            /**
             * @brief Decrements the element index of this iterator.
             */
            inline void operator--()
            {
                m_Index--;
            }

            /**
             * @brief Returns an iterator whose element index is this iterator's element index
             * increased by \p num.
             *
             * @param num The amount by which the element index should be increased.
             *
             * @return The iterator with updated element index.
             */
            inline GenericIterator<ValueType, StructureType> operator+(size_t num)
            {
                auto copy = *this;
                copy.m_Index += num;
                copy.m_Valid = m_Structure->IsIndexValid(m_Index);
                return copy;
            }

            /**
             * @brief Returns an iterator whose element index is this iterator's element index
             * decreased by \p num.
             *
             * @param num The amount by which the element index should be decreased.
             *
             * @return The iterator with updated element index.
             */
            inline GenericIterator<ValueType, StructureType> operator-(size_t num)
            {
                auto copy = *this;
                copy.m_Index -= num;
                copy.m_Valid = m_Structure->IsIndexValid(m_Index);
                return copy;
            }

            /**
             * @brief Increases the element index of this iterator by \p num and returns this
             * iterator.
             *
             * @param num The amount by which the element index should be increased.
             *
             * @return This iterator with it's element index updated.
             */
            inline GenericIterator<ValueType, StructureType> operator+=(size_t num)
            {
                m_Index += num;
                m_Valid = m_Structure->IsIndexValid(m_Index);
                return *this;
            }

            /**
             * @brief Decreases the element index of this iterator by \p num and returns this
             * iterator.
             *
             * @param num The amount by which the element index should be decreased.
             *
             * @return This iterator with it's element index updated.
             */
            inline GenericIterator<ValueType, StructureType> operator-=(size_t num)
            {
                m_Index -= num;
                m_Valid = m_Structure->IsIndexValid(m_Index);
                return *this;
            }

        private:
            StructureType * m_Structure;
            size_t m_Index;
            bool m_Valid;

            GenericIterator(StructureType * structure, size_t index)
                : m_Structure(structure), m_Index(index)
            {
                m_Valid = m_Structure->IsIndexValid(m_Index);
            }

            friend StructureType;
        };

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
 * @brief Outputs the content of \p iter to \p out.
 *
 * @tparam ValueType The type of an element inside \p StructureType. Used for template parameters of
 * \p GenericIterator.
 * @tparam StructureType The type of the structure the iterator will be iterating. Used for template
 * parameters of \p GenericIterator.
 *
 * @param out The <tt>std::ostream &</tt> to output the content of \p iter to.
 * @param iter The \p GenericIterator<ValueType, StructureType> whose content needs to be written
 * to \p out.
 *
 * @return \p out after outputting content to it.
 */
template <typename ValueType, typename StructureType>
std::ostream & operator<<(std::ostream & out, const Shroon::Structure::GenericIterator<ValueType, StructureType> & iter)
{
    return out<<iter.Base();
}

/**
 * @}
 */
