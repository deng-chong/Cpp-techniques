#include <iostream>
#include <memory>
using namespace std;

namespace NS {
template <typename T>
class Counted {
   public:
    class TooManyObjects {};
    static std::size_t objectCount() { return numObjects; }

   protected:
    Counted() { init(); }
    Counted(const Counted&) { init(); }
    ~Counted() { --numObjects; }

   private:
    static std::size_t numObjects;
    static const std::size_t maxObjects;

    void init() {
        if (numObjects >= maxObjects) throw TooManyObjects();
        ++numObjects;
    }
};
template <typename T>
std::size_t Counted<T>::numObjects = 0;

class A : private Counted<A> {
   public:
    using Counted::objectCount;
    using Counted::TooManyObjects;
    static std::shared_ptr<A> make_A() {
        // struct make_shared_enabler : public A {};
        // return std::make_shared<make_shared_enabler>();
        return std::shared_ptr<A>(new A());
    }
    static std::shared_ptr<A> make_A(const A& rhs) {
        return std::shared_ptr<A>(new A(rhs));
    }
    ~A() = default;

   private:
    A() = default;
    A(const A&) = default;
};
}  // namespace NS

template <>
const std::size_t NS::Counted<NS::A>::maxObjects = 3;

int main() {
    auto pa = NS::A::make_A();
    auto x = NS::A::make_A(*pa);
    std::cout << NS::A::objectCount();
}
