#ifndef VALUES_HPP
#define VALUES_HPP

#include <string>
#include <map>
#include <functional>
#include <cstdint>
#include <limits>

#include "aliases.hpp"
#include "util.hpp"

namespace eris
{
    /**
     * @brief 
     * Generic base class for all values in Eris.
     */
    class Value
    {
    public:
        virtual ~Value() = default;
        virtual std::string str() const = 0;
        virtual bool truthy()
        {
            return true;
        }
    };

    /**
     * @brief 
     * Represents the null value in Eris.
     */
    class Null : public Value
    {
    public:
        std::string str() const
        {
            return "null";
        }

        bool truthy()
        {
            return false;
        }
    };

    /**
     * @brief 
     * Generic base class for all numeric values in Eris.
     */
    class Number : public Value
    {
    public:
        virtual std::int32_t intVal() const = 0;

        virtual double doubleVal() const = 0;
        
        virtual bool isInt() const = 0;
    };


    /**
     * @brief 
     * Represents a signed 32-bit integer in Eris.
     */
    class Int: public Number
    {
    public:
        std::int32_t value;
        
        Int(std::int32_t value)
            : value(value)
        {
        }

        bool isInt() const
        {
            return true;
        }

        int intVal() const
        {
            return value;
        }

        double doubleVal() const 
        {
            return (double)value;
        }

        std::string str() const
        {
            return std::to_string(value);
        }
    };

    /**
     * @brief 
     * Represents a double-precision floating-point number in Eris.
     */
    class Double: public Number
    {
    public:
        double value;
        
        Double(double value)
            : value(value)
        {
        }

        bool isInt() const
        {
            return false;
        }

        int intVal() const
        {
            return (std::int32_t)value;
        }
        
        double doubleVal() const 
        {
            return value;
        }

        std::string str() const
        {
            return std::to_string(value);
        }

        static const sh_ptr<Double> inf;
    };

    const sh_ptr<Double> Double::inf { new Double(std::numeric_limits<double>::infinity()) };

    /**
     * @brief 
     * Represents a boolean in Eris.
     */
    class Boolean : public Value
    {
    public:
        bool value;

        Boolean(bool value) : value(value) {}

        bool truthy()
        {
            return value;
        }

        std::string str() const
        {
            return value ? "true" : "false";
        }
    };

    /**
     * @brief 
     * Represents a repository of variables and functions defined in a scope.
     */
    class Environment : public std::enable_shared_from_this<Environment>
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
         * Updates an existing variable.
         */
        void assign(const std::string &name, sh_ptr<Value> value)
        {
            this->resolve(name)->record[name] = value;
        }

        /**
         * Returns the value of a defined variable, or null pointer
         * if the variable is not defined.
         */
        sh_ptr<Value> lookup(const std::string &name)
        {
            return this->resolve(name)->record.at(name);
        }

        /**
         * Returns the value of a defined variable, or null pointer
         * if the variable is not defined.
         */
        sh_ptr<Environment> resolve(const std::string &name)
        {
            if (this->record.count(name) == 1)
            {
                return shared_from_this();
            }

            if (!this->parent)
            {
                throw std::string("variable \"" + name + "\" is not defined");
                return sh_ptr<Environment>();
            }

            return this->parent->resolve(name);
        }

        /**
         * Returns whether or not a variable is defined.
         */
        bool contains(const std::string &name)
        {
            if (this->record.count(name) == 1)
            {
                return true;
            }

            if (!this->parent)
            {
                return false;
            }

            return this->parent->contains(name);
        }
    };

    /**
     * @brief 
     * Represents an object in Eris.
     */
    class Object : public Value
    {
    public:
        sh_ptr<Environment> members;

        Object() : members(sh_ptr<Environment>(new Environment())) {}
        Object(sh_ptr<Environment> members) : members(members) {}

        std::string str() const
        {
            return "(object : " + addressString((const void *)this)+")";
        }
    };

    /**
     * @brief 
     * Represents a class in Eris.
     */
    class Class : public Value
    {
    public:
        sh_ptr<Environment> members;

        Class() : members(sh_ptr<Environment>(new Environment())) {}
        Class(sh_ptr<Environment> members) : members(members) {}

        std::string str() const
        {
            return "(class : " + addressString((const void *)this)+")";
        }
    };

    /**
     * @brief 
     * Represents a string in Eris.
     */
    class String : public Object
    {
    public:
        std::string string;

        String(const std::string &string, sh_ptr<Environment> members) : string(string)
        {
            this->members = members;
        }

        std::string str() const
        {
            return string;
        }
    };

    /**
     * @brief 
     * Represents a user-defined function in Eris.
     */
    class UserDefinedFunction : public Object
    {
    public:
        std::vector<sh_ptr<AST> > params;
        sh_ptr<AST> body;
        sh_ptr<Environment> env;

        UserDefinedFunction(const std::vector<sh_ptr<AST> > &params, sh_ptr<AST> body, sh_ptr<Environment> env)
            : params(params), body(body), env(env)
        {
        }

        std::string str() 
        {
            return "(user-defined function : " + addressString((const void *)this)+")";
        }
    };  

    /**
     * @brief 
     *  Represents a native (built-in) function included in Eris's standard-library.
     */

    class NativeFunction : public Object
    {
    public:
        std::function<sh_ptr<Value> (std::vector<sh_ptr<Value> >)> fn;

        NativeFunction(const std::function<sh_ptr<Value> (std::vector<sh_ptr<Value> >)> &fn)
            : fn(fn)
        {
        }

        std::string str() 
        {
            return "(native function : " + addressString((const void *)this)+")";
        }
    };  
}   

#endif