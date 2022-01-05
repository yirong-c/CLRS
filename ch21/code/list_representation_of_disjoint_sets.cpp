#include <list>

template <typename T>
class DisjointSets
{
public:
    struct Element;
    using Representative = typename std::list< std::list<Element> >::iterator;
    using Iterator = typename std::list<Element>::iterator;
    struct Element
    {
        T key;
        Representative set;

        Element(const T& key, Representative set) : key(key), set(set) {}
    };

    Iterator MakeSet(const T& x);
    void Union(Iterator x, Iterator y);
    Representative FindSet(Iterator x);

    std::list< std::list<Element> > collection_;
};

template <typename T>
typename DisjointSets<T>::Iterator DisjointSets<T>::MakeSet(const T& x)
{
    collection_.emplace_front();
    collection_.front().emplace_front(x, collection_.begin());
    return collection_.front().begin();
}

template <typename T>
void DisjointSets<T>::Union(Iterator x, Iterator y)
{
    Representative x_rep = FindSet(x);
    Representative y_rep = FindSet(y);
    if (x_rep->size() < y_rep->size())
    {
        Union(y, x);
        return;
    }
    for (Element& element : *y_rep)
    {
        element.set = x_rep;
    }
    x_rep->splice(x_rep->end(), *y_rep);
    collection_.erase(y_rep);
}

template <typename T>
typename DisjointSets<T>::Representative DisjointSets<T>::FindSet(Iterator x)
{
    return x->set;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

#include <unordered_map>
#include <vector>

using Iterator = DisjointSets<char>::Iterator;

void Check(DisjointSets<char>& disjoint_sets, 
    const std::list< std::list<char> >& require_collection)
{
    int i;
    std::unordered_map<char, int> map;
    i = 0;
    for (const std::list<char>& set : require_collection)
    {
        for (char key : set)
        {
            map[key] = i;
        }
        ++i;
    }
    std::vector<bool> set_repeat_counter(i, false);
    for (std::list<DisjointSets<char>::Element>& set : disjoint_sets.collection_)
    {
        assert(set.empty() == false);
        assert(map.count(set.front().key) != 0);
        i = map[set.front().key];
        assert(set_repeat_counter[i] == false);
        set_repeat_counter[i] = true;
        for (DisjointSets<char>::Element& element : set)
        {
            assert(&(*(element.set)) == &set);
            assert(map.count(element.key) != 0);
            assert(map[element.key] == i);
            map.erase(element.key);
        }
    }
    assert(map.empty() == true);
}

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    DisjointSets<char> disjoint_sets;
    Iterator a = disjoint_sets.MakeSet('a');
    Iterator b = disjoint_sets.MakeSet('b');
    Iterator c = disjoint_sets.MakeSet('c');
    Iterator d = disjoint_sets.MakeSet('d');
    Iterator e = disjoint_sets.MakeSet('e');
    Iterator f = disjoint_sets.MakeSet('f');
    Iterator g = disjoint_sets.MakeSet('g');
    Iterator h = disjoint_sets.MakeSet('h');
    Iterator i = disjoint_sets.MakeSet('i');
    Iterator j = disjoint_sets.MakeSet('j');
    Check(disjoint_sets, { { 'a' }, { 'b' }, { 'c' }, { 'd' }, { 'e' }, 
        { 'f' }, { 'g' }, { 'h' }, { 'i' }, { 'j' },  });
    disjoint_sets.Union(b, d);
    Check(disjoint_sets, { { 'a' }, { 'b', 'd' }, { 'c' }, { 'e' }, 
        { 'f' }, { 'g' }, { 'h' }, { 'i' }, { 'j' },  });
    disjoint_sets.Union(e, g);
    Check(disjoint_sets, { { 'a' }, { 'b', 'd' }, { 'c' }, { 'e', 'g' }, 
        { 'f' }, { 'h' }, { 'i' }, { 'j' },  });
    disjoint_sets.Union(a, c);
    Check(disjoint_sets, { { 'a', 'c' }, { 'b', 'd' }, { 'e', 'g' }, 
        { 'f' }, { 'h' }, { 'i' }, { 'j' },  });
    disjoint_sets.Union(h, i);
    Check(disjoint_sets, { { 'a', 'c' }, { 'b', 'd' }, { 'e', 'g' }, 
        { 'f' }, { 'h', 'i' }, { 'j' },  });
    disjoint_sets.Union(a, b);
    Check(disjoint_sets, { { 'a', 'b', 'c', 'd' }, { 'e', 'g' }, 
        { 'f' }, { 'h', 'i' }, { 'j' },  });
    disjoint_sets.Union(e, f);
    Check(disjoint_sets, { { 'a', 'b', 'c', 'd' }, { 'e', 'f', 'g' }, 
        { 'h', 'i' }, { 'j' },  });
    REQUIRE(disjoint_sets.FindSet(b) == disjoint_sets.FindSet(c));
}


