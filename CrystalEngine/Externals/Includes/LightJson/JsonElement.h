#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <stdexcept>

#include "JsonUtils.h"

namespace LightJson
{
    class JsonElement
    {
    protected:
        JsonUtils::JsonType type;
    
    public:
        JsonElement() : type(JsonUtils::JsonType::Null) {}
        virtual ~JsonElement() = default;
    
        virtual std::string          getAsString()  const { throw std::runtime_error("Element is not a string");   }
        virtual bool                 getAsBoolean() const { throw std::runtime_error("Element is not a boolean");  }
        virtual JsonUtils::JsonArray getAsArray()   const { throw std::runtime_error("Element is not an array");   }
        virtual JsonUtils::JsonMap   getAsObject()  const { throw std::runtime_error("Element is not an object");  }
        virtual float                getAsFloat()   const { throw std::runtime_error("Element is not a float");    }
        virtual int                  getAsInteger() const { throw std::runtime_error("Element is not an integer"); }
    
        bool isOfType(const JsonUtils::JsonType& _type) const { return type == _type; }

        JsonUtils::JsonType getType() const { return type; }
    };


    class StringElement final : public JsonElement
    {
    private:
        std::string value;
    
    public:
        explicit StringElement(std::string value);
        std::string getAsString() const override { return value; }
    };
    inline StringElement::StringElement(std::string value) : value(std::move(value))
    {
        type = JsonUtils::JsonType::String;
    }

    
    template <typename T> class NumberElement final : public JsonElement
    {
    private:
        T value;
        
    public:
        explicit NumberElement(const T& value);
        
        float getAsFloat() const override { return (float)value; }
        int getAsInteger() const override { return (int)value; }
    };
    template <typename T> inline NumberElement<T>::NumberElement(const T& value) : value(value)
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int> || std::is_same_v<T, size_t>) {
            type = JsonUtils::JsonType::Integer;
        } else {
            type = JsonUtils::JsonType::Float;
        }
    }
    
    
    class BooleanElement final : public JsonElement
    {
    private:
        bool value;
    
    public:
        explicit BooleanElement(const bool& value);
        bool getAsBoolean() const override { return value; }
    };
    inline BooleanElement::BooleanElement(const bool& value) : value(value)
    {
        type = JsonUtils::JsonType::Boolean;
    }


    class ArrayElement final : public JsonElement
    {
    private:
        JsonUtils::JsonArray array;
    
    public:
        ArrayElement(JsonUtils::JsonArray array);
        ~ArrayElement() override {
            for(const JsonElement* element : array) {
                delete element;
            }
            array.clear();
        }
    
        JsonUtils::JsonArray getAsArray() const override { return array; }
        void addElement(JsonElement* element) { array.push_back(element); }
    };
    inline ArrayElement::ArrayElement(JsonUtils::JsonArray array) : array(std::move(array))
    {
        type = JsonUtils::JsonType::Array;
    }


    class ObjectElement final : public JsonElement
    {
    private:
        JsonUtils::JsonMap map;
    
    public:
        explicit ObjectElement(JsonUtils::JsonMap map);
        ~ObjectElement() override {
            for(const auto& it : map) {
                delete it.second;
            }
            map.clear();
        }
    
        JsonUtils::JsonMap getAsObject() const override { return map; }
        void addElement(std::string key, JsonElement* element) { map[key] = element; }
    };
    inline ObjectElement::ObjectElement(JsonUtils::JsonMap map) : map(std::move(map))
    {
        type = JsonUtils::JsonType::Object;
    }
}