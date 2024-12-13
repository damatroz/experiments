#include <functional>
#include <iostream>
#include <list>
#include <memory>

class A
{
public:
    A(A* parent) : m_parent(parent) {}

    virtual void Test1() const
    {
        std::cout << "A::Test1()" << std::endl;
    }

    virtual int Test2(int param)
    {
        std::cout << "A::Test2(" << param << ")" << std::endl;
        return param;
    }

    void Test1Impl()
    {
        Test1();
    }

    void Test2Impl()
    {
        Test2(1);
    }

private:
    A* m_parent = nullptr;
};

class B : public A
{
public:
    using A::A;

    void Test1() const override
    {
        std::cout << "B::Test1()" << std::endl;
        A::Test1();
    }
};

template <typename Base>
class Decorator : public Base
{
public:
    using Base::Base;

    int Test2(int param) override
    {
        std::cout << "Decorator::Test2(" << param << ")" << std::endl;
        return Base::Test2(param);
    }
};

//==============================================================

template <typename Base>
class ObjectDecorators : public Base
{
public:
    class InnerDecorator;

    using List = std::list<std::unique_ptr<InnerDecorator>>;
    using Iterator = typename List::iterator;

    class InnerDecorator
    {
    public:
        InnerDecorator(ObjectDecorators* list, ObjectDecorators::Iterator iterator)
            : m_list(list), m_iterator(std::move(iterator)) {}

        virtual void Test1() const { m_list->BaseTest1(m_iterator); }
        virtual int Test2(int param) { return m_list->BaseTest2(m_iterator, param); }

        virtual void BaseTest1() const = 0;
        virtual int BaseTest2(int) = 0;

        ObjectDecorators* m_list;
        const ObjectDecorators::Iterator m_iterator;
    };

private:
    template <typename Base2>
    class OuterDecorator : public Base2
    {
    public:
        using Base2::Base2;

        void Test1() const override { InnerDecorator::m_list->Test1(); }
        int Test2(int param) override { return InnerDecorator::m_list->Test2(param); }

        void BaseTest1() const override { Base2::Test1(); }
        int BaseTest2(int param) override { return Base2::Test2(param); }
    };

public:
    using Base::Base;

    template <template<typename> typename Decorator>
    void AddDecorator()
    {
        m_decorators.emplace_front();
        auto iterator = --m_decorators.end();
        iterator->reset(new OuterDecorator<Decorator<InnerDecorator>>(this, iterator));
    }

    void Test1() const override { return InternalImplementation(this, m_decorators.begin(), &InnerDecorator::BaseTest1, &ObjectDecorators::ObjectBaseTest1); }
    int Test2(int a) override { return InternalImplementation(this, m_decorators.begin(), &InnerDecorator::BaseTest2, &ObjectDecorators::ObjectBaseTest2, a); }

private:
    List m_decorators;

    void BaseTest1(Iterator iterator) const { return InternalImplementation(this, ++iterator, &InnerDecorator::BaseTest1, &ObjectDecorators::ObjectBaseTest1); }
    int BaseTest2(Iterator iterator, int a) { return InternalImplementation(this, ++iterator, &InnerDecorator::BaseTest2, &ObjectDecorators::ObjectBaseTest2, a); }

    void ObjectBaseTest1() const { Base::Test1(); }
    int ObjectBaseTest2(int param) { return Base::Test2(param); }

    template <typename Object, typename Iterator, typename IterFunc, typename BaseFunc, typename ...Args>
    static auto InternalImplementation(Object* object, Iterator iterator, IterFunc iterFunc, BaseFunc baseFunc, Args... args)
    {
        if (iterator != object->m_decorators.end())
        {
            return (iterator->get()->*iterFunc)(args...);
        }
        else
        {
            return (object->*baseFunc)(args...);
        }
    }
};

//==============================================================

int main(int argv, char* argc[])
{
    ObjectDecorators<B> b(nullptr);
    b.AddDecorator<Decorator>();
    b.Test1Impl();
    std::cout << "---------------------------" << std::endl;
    b.Test2Impl();

    return 0;
}
