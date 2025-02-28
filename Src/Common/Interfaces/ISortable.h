#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/TypeTraits.h"

namespace d14engine
{
    // Define ISortable as a template class, so that an object can implement
    // sorting functionality for different properties by inheriting
    // multiple ISortable<T> (where T represents different properties).
    //
    // For example, suppose struct Student is inherited from
    // ISortable<Height> and ISortable<Weight>, in which case a student knows
    // their height and weight respectively and can be sorted by each of them.

    template<typename T, typename PT = int> // PT: Priority Type
    struct ISortable
    {
        static_assert(cpp_lang_utils::has_operator::lessThan<PT> && cpp_lang_utils::has_operator::equalTo<PT>,
            "Both less-than (lhs < rhs) operator and equal-to (lhs == rhs) operator must be implemented for PT");

    protected:
        PT m_priority = {};

    public:
        const ISortable<T>* id() const { return this; }

        PT priority() const { return m_priority; }
        void setPriority(PT value) { m_priority = value; }

        ///////////////////////
        // Raw Ptr Ascending //
        ///////////////////////
        
        struct RawAscending
        {
            bool operator()(const ISortable<T>& lhs, const ISortable<T>& rhs) const
            {
                if (lhs.id() == rhs.id())
                {
                    return false;
                }
                else // Strict unique according to ID.
                {
                    if (lhs.priority() == rhs.priority())
                    {
                        return lhs.id() < rhs.id();
                    }
                    else // Strict ascending according to priority.
                    {
                        return lhs.priority() < rhs.priority();
                    }
                }
            }
        };
        using RawPrioritySet = std::set<T, RawAscending>;
        template<typename ValueType>
        using RawPriorityMap = std::map<T, ValueType, RawAscending>;

        //////////////////////////
        // Shared Ptr Ascending //
        //////////////////////////

        struct ShrdAscending
        {
            bool operator()(ShrdPtrParam<ISortable<T>> lhs, ShrdPtrParam<ISortable<T>> rhs) const
            {
                return RawAscending()(*lhs.get(), *rhs.get());
            }
        };
        using ShrdPrioritySet = std::set<SharedPtr<T>, ShrdAscending>;
        template<typename ValueType>
        using ShrdPriorityMap = std::map<SharedPtr<T>, ValueType, ShrdAscending>;

        ////////////////////////
        // Weak Ptr Ascending //
        ////////////////////////

        struct WeakAscending
        {
            bool operator()(WeakPtrParam<ISortable<T>> lhs, WeakPtrParam<ISortable<T>> rhs) const
            {
                // The order of comparison here is significant:
                //
                // 1. Compare "rhs" in advance of "lhs":
                //
                //    Place the expired elements in front of the set.
                //
                // 2. Return "false" in advance of "true":
                //
                //    Ensure the TOR (compatible with STL).
                //
                //-----------------------------------------------
                // TOR: Total Order Relation
                //-----------------------------------------------
                // 1. Reflexive:
                //    assert(a <= a)
                //-----------------------------------------------
                // 2. Transitive:
                //    If (a <= b && b <= c) then (a <= c)
                //-----------------------------------------------
                // 3. Antisymmetric:
                //    If (a <= b && b <= a) then (a == b)
                //-----------------------------------------------
                // 4. Strongly Connected (Total):
                //    assert(a <= b || b <= a)
                //-----------------------------------------------

                if (rhs.expired()) return false;
                if (lhs.expired()) return true;

                return ShrdAscending()(lhs.lock(), rhs.lock());
            }
        };
        using WeakPrioritySet = std::set<WeakPtr<T>, WeakAscending>;
        template<typename ValueType>
        using WeakPriorityMap = std::map<WeakPtr<T>, ValueType, WeakAscending>;

        ///////////////////
        // Miscellaneous //
        ///////////////////

        // The boolean value returned by func
        // indicates whether to handle the remainings.
        static void foreach(
            WeakPrioritySet& cont,
            FuncParam<bool(ShrdPtrParam<T>)> func)
        {
            bool continueDeliver = true;

            for ( auto itor = cont.begin() ;; )
            {
                while (itor != cont.end() && itor->expired())
                {
                    itor = cont.erase(itor);
                }
                if (itor != cont.end())
                {
                    if (continueDeliver)
                    {
                        continueDeliver = func(itor->lock());
                    }
                    ++itor;
                }
                else break;
            }
        }
    };
}
