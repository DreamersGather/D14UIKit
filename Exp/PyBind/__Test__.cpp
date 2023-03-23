#include "Common/Precompile.h"

#ifdef _D14_UIKIT_PYBIND_DEBUG

#include "__Test__.h"

// This file demostrates the basic boilerplate to use pybind11.
// Also see the documentation: https://pybind11.readthedocs.io.

namespace d14uikit
{
    class Animal
    {
    public:
        // MUST define this to notify pybind11 to recognize polymorphism!
        virtual ~Animal() = default;

        virtual void move()
        {
            printf("move, move, move~\n");
        }
    };

    template<typename AnimalBase = Animal>
    class PyAnimal : public AnimalBase
    {
    public:
        using AnimalBase::AnimalBase;

        void move() override
        {
            PYBIND11_OVERRIDE(void, AnimalBase, move, );
        }
    };

    class FlyAnimal : virtual public Animal
    {
    public:
        virtual void fly()
        {
            printf("fly, fly, fly~\n");
        }
    };

    template<typename FlyAnimalBase = FlyAnimal>
    // pybind11 does NOT support using virtual inheritance here!
    class PyFlyAnimal : public PyAnimal<FlyAnimalBase>
    {
    public:
        using PyAnimal::PyAnimal;

        void fly() override
        {
            PYBIND11_OVERRIDE(void, FlyAnimalBase, fly, );
        }
    };

    class WalkAnimal : virtual public Animal
    {
    public:
        virtual void walk()
        {
            printf("walk, walk, walk~\n");
        }
    };

    template<typename WalkAnimalBase = WalkAnimal>
    // pybind11 does NOT support using virtual inheritance here!
    class PyWalkAnimal : public PyAnimal<WalkAnimalBase>
    {
    public:
        using PyAnimal::PyAnimal;

        void walk() override
        {
            PYBIND11_OVERRIDE(void, WalkAnimalBase, walk, );
        }
    };

    class Bird : public FlyAnimal, public WalkAnimal
    {
    public:
        virtual void sing()
        {
            printf("bird, bird, bird~\n");
        }
        void move() override
        {
            printf("both fly and walk~\n");
        }
    };

    template<typename BirdBase = Bird>
    class PyBird : public PyAnimal<BirdBase>
    {
    public:
        using PyAnimal::PyAnimal;

        // As commented above, pybind11 does not support using virtual
        // inheritance on trampoline classes, which causes multiple
        // inheritance invalid here. Have to repeat boilerplates here.
        void fly() override
        {
            PYBIND11_OVERRIDE(void, BirdBase, fly, );
        }
        void walk() override
        {
            PYBIND11_OVERRIDE(void, BirdBase, walk, );
        }
        void sing() override
        {
            PYBIND11_OVERRIDE(void, BirdBase, sing, );
        }
    };

    class Eagle : public Bird { /* a dummy child */ };

    void Py_InitTest(py::module_& m)
    {
        py::class_<Animal, PyAnimal<>> animal(m, "Animal");

        animal.def(py::init<>());
        animal.def("move", &Animal::move);

        py::class_<FlyAnimal, Animal, PyFlyAnimal<>> flyanim(
            m, "FlyAnimal",
            // MUST define this to notify pybind11 to recognize polymorphism!
            py::multiple_inheritance());

        flyanim.def(py::init<>());
        flyanim.def("fly", &FlyAnimal::fly);

        py::class_<WalkAnimal, Animal, PyWalkAnimal<>> walkanim(
            m, "WalkAnimal",
            // MUST define this to notify pybind11 to recognize polymorphism!
            py::multiple_inheritance());

        walkanim.def(py::init<>());
        walkanim.def("walk", &WalkAnimal::walk);

        py::class_<Bird, FlyAnimal, WalkAnimal, PyBird<>> bird(
            m, "Bird", py::multiple_inheritance());

        bird.def(py::init<>());
        bird.def("sing", &Bird::sing);

        py::class_<Eagle, Bird, PyBird<Eagle>> eagle(m, "Eagle");

        eagle.def(py::init<>());
    }
}

#endif
