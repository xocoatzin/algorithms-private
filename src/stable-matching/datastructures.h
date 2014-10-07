#ifndef STABLE_MATCHING_DATASTRUCTURES_H
#define STABLE_MATCHING_DATASTRUCTURES_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <functional>


namespace stable_matching
{
    template<typename T>
    class TableContainer
    {
    public:
        typedef T Table;
        typedef typename T::key_type Key;
        typedef typename T::mapped_type Type;
        typedef typename T::iterator iterator;
        typedef typename T::const_iterator const_iterator;

        virtual iterator begin() { return table.begin(); }
        virtual iterator end() { return table.end(); }

        virtual void add(Key key, Type name)
        {
            table.insert(typename Table::value_type(key, name));
        };

        virtual Type get(Key id)
        {
        	iterator it = table.find(id);
            if (it != table.end())
                return it->second;
            return Type();
        }

        virtual void clear()
        {
            table.clear();
        };

        virtual void print()
        {};

        virtual std::size_t size() { return table.size(); }

        template<typename K = TableContainer>
        K copy_filter(std::function<bool(iterator)> filter)
        {
            K t;
            for (iterator it = table.begin(); it != table.end(); it++)
            {
                if (filter(it))
                    t.add(it->first, it->second);
            }

            return (t);
        };

    protected:
        Table table;

        virtual TableContainer& self() { return *this; }
    };

    class Members
        : public TableContainer<std::map<std::string, std::string> >
    {
    public:
        void print()
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                std::cout << it->first << ": " << it->second << std::endl;
            }
        };

        template<typename T>
        void print(T t)
        {
            t.print(self());
        };

    protected:
        virtual Members& self() { return *this; }
    };

    class Relationships
        : public TableContainer<std::map<std::string, std::string> >
    {
    public:
        bool exist(std::string key1, std::string key2)
        {
            iterator f1 = table.find(key1);
            bool ret = (f1 != table.end() && f1->second.compare(key2) == 0);

            std::cout << key1 << " -?- " << key2 << " >> " << (ret?"yes":"no") << std::endl;

            return ret;
        };

        bool are_single(std::string key1, std::string key2)
        {
            iterator f1 = table.find(key1);
            iterator f2 = table.find(key2);

            return !(f1 != table.end() || f2 != table.end());
        };

        bool is_single(std::string key1, bool print = false)
        {
            bool ret = (table.find(key1) == table.end());
            if(print) std::cout << "Is {" << key1 << "} single? " << (!ret ? "yes" : "no") << std::endl;
            return ret;
        };

        void free(std::string key)
        {
            iterator
                f1 = table.find(key),
                f2;

            if (f1 != table.end())
            {
                f2 = table.find(f1->second);

                std::cout << key << " -x- " << f1->second << std::endl;

                table.erase(f1);
                table.erase(f2);
            }
        };

        void add(std::string key1, std::string key2)
        {
            if (are_single(key1, key2))
            {
                table.insert(Table::value_type(key1, key2));
                table.insert(Table::value_type(key2, key1));

                std::cout << key1 << " <-> " << key2 << std::endl;
            }
            else
            {
                std::cout << key1 << " -x- " << key2 << " Error creating relationship, are they both single?" << std::endl;
            }
        }

        void print()
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                if (it->first < it->second)
                    std::cout << it->first << " <-> " << it->second << std::endl;
            }
        };

        void print(Members &m)
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                if (it->first < it->second)
                {
                    std::string id = m.get(it->first);
                    std::string name = m.get(it->second);

                    std::cout << id << " <-> " << name << std::endl;
                }
            }
        };
    };

    class Preferences
        : public TableContainer<std::map<std::string, std::vector<std::string>> >
    {
    public:
        void print()
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                std::cout << it->first << " -> ";
                for (Type::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << "[" << *it2 << "] ";
                }
                std::cout << std::endl;
            }
        };

        void print(Members &m)
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                std::cout << m.get(it->first) << " -> ";
                for (Type::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << "[" << m.get(*it2) << "] ";
                }
                std::cout << std::endl;
            }
        };
    };
}

#endif
