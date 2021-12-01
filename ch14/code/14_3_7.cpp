#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

#include <vector>

#include "augment_red_black_tree/interval_tree/interval_tree.hpp"

typedef IntervalTree<int, int> Tree;
typedef Tree::Interval Interval;
typedef Tree::Iterator TreeIterator;

// rectilinearly oriented
struct Rectangle
{
    Interval x_int;
    Interval y_int;
};

struct RectangleWithXCoordIndex
{
    int x_coord;
    enum { LEFT, RIGHT } coord_pos;// CoordPos
    union
    {
        RectangleWithXCoordIndex* right;// for LEFT
        TreeIterator left;// for RIGHT
    } relate;
    const Rectangle* rectangle;
};

inline size_t Parent(size_t i) { return (i - 1) >> 1; }

inline size_t Left(size_t i) { return (i << 1) + 1; }
	
inline size_t Right(size_t i) { return (i << 1) + 2; }

inline void Swap(RectangleWithXCoordIndex*& a, RectangleWithXCoordIndex*& b)
{
	RectangleWithXCoordIndex* temp;
	temp = a;
	a = b;
	b = temp;
}

void MinHeapify(std::vector<RectangleWithXCoordIndex*>& heap, size_t i)
{
    size_t size, left, right, smallest;
    size = heap.size();
    left = Left(i);
    right = Right(i);
    if (left < size && heap[left]->x_coord < heap[i]->x_coord) smallest = left;
    else smallest = i;
    if (right < size && heap[right]->x_coord < heap[smallest]->x_coord) smallest = right;
    if (smallest != i)
    {
        Swap(heap[i], heap[smallest]);
        MinHeapify(heap, smallest);
    }
}

void MinHeapInsert(std::vector<RectangleWithXCoordIndex*>& heap, RectangleWithXCoordIndex* x)
{
    size_t i, parent;
    i = heap.size();
    heap.push_back(x);
    // i = heap.size() - 1;
    parent = Parent(i);
    while (i > 0 && heap[parent]->x_coord > heap[i]->x_coord)
    {
        Swap(heap[i], heap[parent]);
        i = parent;
        parent = Parent(i);
    }
}

RectangleWithXCoordIndex* HeapExtractMin(std::vector<RectangleWithXCoordIndex*>& heap)
{
    size_t size;
    RectangleWithXCoordIndex* min;
    size = heap.size();
    min = heap[0];
    heap[0] = heap[size - 1];
    heap.pop_back();
    MinHeapify(heap, 0);
    return min;
}

// caller is responsible to deallocate the return value
RectangleWithXCoordIndex* ConstructHeap(const std::vector<Rectangle>& rectangles, 
    std::vector<RectangleWithXCoordIndex*>& heap)// heap is out-para; heap need to be empty
{
    size_t rectangles_size, heap_size;
    RectangleWithXCoordIndex *alloc_ptr, *it;
    std::allocator<RectangleWithXCoordIndex> alloc;
    using traits_t = std::allocator_traits<decltype(alloc)>;
    rectangles_size = rectangles.size();
    heap_size = rectangles_size << 1;
    alloc_ptr = traits_t::allocate(alloc, heap_size);
    it = alloc_ptr;
    heap.reserve(heap_size);
    for (const Rectangle& rectangle : rectangles)
    {
        it->x_coord = rectangle.x_int.low;
        it->coord_pos = RectangleWithXCoordIndex::LEFT;
        it->rectangle = &rectangle;
        it->relate.right = it + 1;
        MinHeapInsert(heap, it);
        ++it;
        it->x_coord = rectangle.x_int.high;
        it->coord_pos = RectangleWithXCoordIndex::RIGHT;
        it->rectangle = &rectangle;
        MinHeapInsert(heap, it);
        ++it;
    }
    return alloc_ptr;
}

void DestructHeap(RectangleWithXCoordIndex* alloc_ptr, size_t rectangles_size)
{
    std::allocator<RectangleWithXCoordIndex> alloc;
    using traits_t = std::allocator_traits<decltype(alloc)>;
    traits_t::deallocate(alloc, alloc_ptr, rectangles_size << 1);
}

bool DetermineOverlapRectangles(const std::vector<Rectangle>& rectangles)
{
    bool found_overlap;
    RectangleWithXCoordIndex *alloc_ptr, *now;
    std::allocator<RectangleWithXCoordIndex> alloc;
    std::vector<RectangleWithXCoordIndex*> heap;
    Tree tree;// interval tree
    found_overlap = false;
    // sort the x-coordinates with the minimum heap
    alloc_ptr = ConstructHeap(rectangles, heap);
    while (heap.size() > 0)
    {
        now = HeapExtractMin(heap);
        if (now->coord_pos == RectangleWithXCoordIndex::LEFT)
        {
            if (tree.Find(now->rectangle->y_int) != tree.End())
            {
                found_overlap = true;
                break;
            }
            now->relate.right->relate.left = 
                tree.Insert({now->rectangle->y_int, 0}).first;
        }
        else
        {
            tree.Delete(now->relate.left);
        }
    }
    DestructHeap(alloc_ptr, rectangles.size());
    return found_overlap;
}

TEST_CASE()
{
    std::vector<Rectangle> rectangles;
    SECTION("overlap 1")
    {
        rectangles = {
            { { 1, 13 }, { 5, 10 } },
            { { 4, 15 }, { 15, 20 } },
            { { 6, 11 }, { 0, 12 } }
        };
        REQUIRE(DetermineOverlapRectangles(rectangles));
    }
    SECTION("not overlap 1")
    {
        rectangles = {
            { { 2, 5 }, { 6, 10 } },
            { { 1, 10 }, { 1, 5 } },
            { { 7, 9 }, { 7, 9 } }
        };
        REQUIRE_FALSE(DetermineOverlapRectangles(rectangles));
    }
    SECTION("overlap 2")
    {
        rectangles = {
            { { 8, 9 }, { 1, 7 } },
            { { 3, 5 }, { 6, 8 } },
            { { 1, 6 }, { 1, 3 } },
            { { 4, 6 }, { 2, 5 } },
            { { 1, 2 }, { 4, 9 } }
        };
        REQUIRE(DetermineOverlapRectangles(rectangles));
    }
}
