#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/TypeTraits.h"

namespace d14engine
{
    // Define ISortable as template to enable distinguishing the priorities
    // of different attributes.
    // 
    // For example, suppose struct Student is inherited from
    // ISortable<Height> and ISortable<Weight>, in which case a student knows
    // their height and weight respectively and can be sorted by any of them.

    template<typename T, typename PT = int>
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
        struct ShrdAscending
        {
            bool operator()(ShrdPtrParam<ISortable<T>> lhs, ShrdPtrParam<ISortable<T>> rhs) const
            {
                return RawAscending()(*lhs.get(), *rhs.get());
            }
        };
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
                //    Ensure the total order relation (compatible with STL).

                if (rhs.expired()) return false;
                if (lhs.expired()) return true;

                return ShrdAscending()(lhs.lock(), rhs.lock());
            }
        };

        using RawPrioritySet = std::set<T, RawAscending>;
        template<typename ValueType>
        using RawPriorityMap = std::map<T, ValueType, RawAscending>;

        using ShrdPrioritySet = std::set<SharedPtr<T>, ShrdAscending>;
        template<typename ValueType>
        using ShrdPriorityMap = std::map<SharedPtr<T>, ValueType, ShrdAscending>;

        using WeakPrioritySet = std::set<WeakPtr<T>, WeakAscending>;
        template<typename ValueType>
        using WeakPriorityMap = std::map<WeakPtr<T>, ValueType, WeakAscending>;

        // func's return boolean means whether to handle the remaining alive objects.
        static void foreach(WeakPrioritySet& cont, FuncParam<bool(ShrdPtrParam<T>)> func)
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
#ifdef _DEBUG // We are determined to disable the naming mechanism in release-mode.
    protected:
        Wstring m_name = L"Unnamed";

    public:
        const Wstring& name() const { return m_name; }
        void setName(WstrParam name) { m_name = name; }
#else
        template<typename T = int>
        const Wstring& name() const
        {
            static_assert(false, "This method is not available in release-mode.");
        }
        template<typename T = int>
        void setName(WstrParam name)
        {
            static_assert(false, "This method is not available in release-mode.");
        }
#endif
    };
}
