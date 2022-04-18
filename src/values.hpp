#ifndef VALUES_HPP
#define VALUES_HPP

#include "aliases.hpp"

namespace eris
{
    class Value
    {
    public:
        virtual ~Value() = default;
        virtual std::string str() const = 0;
    };

    class None: public Value
    {
    public:
        std::string str() const 
        {
            return "none";
        }
    };

    class Number : public Value
    {
    public:
        double value;

        Number(double value) : value(value) {}

        std::string str() const
        {
            return std::to_string(value);
        }
    };

    class Environment
    {
    public:
        std::map<std::string, sh_ptr<Value> > record;
        sh_ptr<Environment> parent;

        Environment(std::map<std::string, sh_ptr<Value> > record = {},
                    sh_ptr<Environment> parent = sh_ptr<Environment>())
            : record(record), parent(parent)
        {
        }

        /**
         * Creates a variable with the given name and value.
         */
        void define(const std::string &name, sh_ptr<Value> value)
        {
            this->record[name] = value;
        }

        /**
         * Returns the value of a defined variable, or null pointer
         * if the variable is not defined.
         */
        sh_ptr<Value> lookup(const std::string &name)
        {
            if(this->record.count(name) == 0)
            {
                if(this->parent)
                {
                    return this->parent->lookup(name);
                }
                
                throw std::string("variable \""+name+"\" is not defined");
                return sh_ptr<Value>();
            }

            return this->record.at(name);
        }
    };
    
    class Object : public Value
    {
    public:
        sh_ptr<Environment> environment;

        std::string str() const
        {
            return "<object>";
        }
    };

    class String : public Value
    {
    public:
        std::string string;

        String(const std::string &string) : string(string) {}

        std::string str() const
        {
            return string;
        }
    };
}

#endif