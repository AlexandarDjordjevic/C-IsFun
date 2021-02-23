#include <map>
#include <functional>
#include <iostream>
#include <stdarg.h>

namespace example1
{
    
class Invoked{
public:
bool Function(uint32_t argn, const char* message){
      std::cout << "Argument: " << argn << std::endl;
      std::cout << "Message: " << message << std::endl;
      return true;
}
};

class InvokerBase{

public:
    InvokerBase(Invoked* m_Invoked){
        m_Invoked = m_Invoked;
    }

    bool Invoke(const char* func, uint32_t argn, const char * message){
        return (m_Invoked->*m_FunctionMap[func])(argn, message);
    }

    void Insert(bool (Invoked::* a_Fkn)(uint32_t, const char*),
                const char* a_Key)
    {
        m_FunctionMap.insert(std::pair<const char*, bool(Invoked::*)(uint32_t, const char*)>(a_Key, a_Fkn));
    }

private:
    std::map<const char*, bool(Invoked::*)(uint32_t, const char*) > m_FunctionMap;
    Invoked* m_Invoked;
};

} // namespace example1

namespace example2
{

class Invoked{
public:
void Function(size_t argn, ...){
    char* val;
    va_list vl;
    va_start(vl, argn);
    for (size_t i=0; i < argn; i++)
    {
        val = va_arg(vl, char*);
        std::cout << "Argument " << val << std::endl;
    }
    va_end(vl);
}
};

class Invoker{
public:
    Invoker(Invoked* invoker)
    {
        m_Invoker = invoker;
    }
    
    void Insert(const char* f_name, void(Invoked::* f_ptr)(size_t,...)){
        funcMap.insert(std::pair<const char*, void(Invoked::*)(size_t, ...)>(f_name, f_ptr));
    }

    void Invoke(const char* f_name, size_t argn, ...){
            char* args[argn];
            va_list vl;
            std::cout << "Arg num: " << argn << std::endl;
            va_start(vl, argn);
            for (size_t i=0; i < argn; i++)
            {
                args[i] = va_arg(vl, char*);
                std::cout << "Argument " << args[i] << std::endl;
            }
            va_end(vl);
            (m_Invoker->*funcMap[f_name])(argn, args);
    }
private:
    Invoked* m_Invoker;
    
    std::map<const char*, void(Invoked::*)(size_t,...)> funcMap;
};

}// namespace example2

int main(){
    std::cout << "Example 1" << std::endl;
    example1::Invoked invoked1;
    example1::InvokerBase invoker1(&invoked1);
    invoker1.Insert(&example1::Invoked::Function, "Function");
    invoker1.Invoke("Function", 1234, "Invoked message");

    std::cout << "\nExample 2" << std::endl;
    example2::Invoked invoked;
    example2::Invoker invoker(&invoked);
    invoker.Insert("Function", &example2::Invoked::Function);
    invoker.Invoke("Function", 1, "Hello World");
    return 0;
}