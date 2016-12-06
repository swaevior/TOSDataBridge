/* 
Copyright (C) 2014 Jonathon Ogden   < jeog.dev@gmail.com >

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses.
*/

#ifndef JO_TOSDB_SHELL
#define JO_TOSDB_SHELL

#include <iostream>
#include <map>

#include "tos_databridge.h"

#define MAX_DISPLAY_WIDTH 80
#define LEFT_INDENT_SIZE 10
#define CMD_OUT_PER_PAGE 10

/* forward decl */
class stream_prompt;

class stream_prompt_base {   
protected:
    std::string _prmpt;  

    explicit stream_prompt_base(std::string prompt)
        :      
            _prmpt(prompt)        
        {
        }

    virtual
    ~stream_prompt_base()       
        {
        }

public:
    /* return the base */
    template<typename T>
    stream_prompt_base& 
    operator<<(const T& val)
    {
        std::cout<< val; 
        return *this;
    }

    /* return the base */
    template< typename T >
    stream_prompt_base& 
    operator>>(T& val)
    {        
        std::cout<< _prmpt << ' ';        
        std::cin>> val;      
        return *this;
    }

    /* return the derived by casting from base
    this is safe if done from outside since we protect constructor */
    static stream_prompt& 
    endl(stream_prompt_base& p)
    {
        std::cout<< std::endl;
        return (stream_prompt&)(p);
    }

    inline stream_prompt& 
    operator<<(decltype(stream_prompt_base::endl) val)
    {
        return (*val)(*this);                    
    }   

    inline stream_prompt& 
    operator>>(decltype(stream_prompt_base::endl) val)
    {
        return (*val)(*this);                    
    } 
};
 
class stream_prompt
        : public stream_prompt_base {
public:
    explicit stream_prompt(std::string prompt)
        :      
            stream_prompt_base(prompt)
        {
        }

    /* RETURN THE BASE */
    template<typename T>
    stream_prompt_base& 
    operator<<(const T& val)
    {
        std::cout<< _prmpt << ' ' << val << ' '; 
        return *this;
    }  
};

class stream_prompt_basic
        : public stream_prompt_base {
public:
    explicit stream_prompt_basic(std::string prompt)
        :      
            stream_prompt_base(prompt)
        {
        }

    /* DONT RETURN THE BASE */
    template<typename T>
    stream_prompt_basic& 
    operator<<(const T& val)
    {
        std::cout<< _prmpt << ' ' << val << ' '; 
        return *this;
    }  
};

extern stream_prompt prompt;
extern stream_prompt_basic prompt_b;

/* fwrd decl */
struct CommandCtx;

typedef void(*commands_func_ty)(CommandCtx*);

struct CommandCtx{
    std::string name;
    std::string doc;
    commands_func_ty func;
    bool exit;    
};

typedef std::map<std::string, CommandCtx> commands_map_ty;

commands_map_ty::value_type
build_commands_map_elem(std::string name, commands_func_ty, std::string doc="");

extern commands_map_ty commands_admin; 
extern commands_map_ty commands_get;
extern commands_map_ty commands_stream;
extern commands_map_ty commands_frame;
extern commands_map_ty commands_local;

typedef std::pair<std::string,commands_map_ty> command_display_pair;
typedef std::unordered_map<std::string, command_display_pair> commands_map_of_maps_ty;

/* should be const but we can't use init lists so manually instantiate in util.cpp */
extern commands_map_of_maps_ty commands;

enum class language 
    : int {
    none = 0,
    c = 1,
    cpp = 2    
};

extern std::unordered_map<language, std::pair<std::string,std::string>> language_strings;

language
set_default_language(language l);

language
get_default_language();

size_type 
get_cstr_entries(std::string label, char ***pstrs, CommandCtx *ctx);

inline void 
prompt_for(std::string sout, std::string* sin, CommandCtx *ctx)
{
    prompt_b<< ctx->name << sout >> *sin;
}

bool 
prompt_for_cpp(CommandCtx *ctx, int recurse=2);

bool 
prompt_for_datetime(std::string block, CommandCtx *ctx);

void
prompt_for_block_item_topic(std::string *pblock, 
                            std::string *pitem, 
                            std::string *ptopic,
                            CommandCtx *ctx);

void
prompt_for_block_item_topic_index(std::string *pblock, 
                                  std::string *pitem, 
                                  std::string *ptopic, 
                                  std::string *pindex, 
                                  CommandCtx *ctx);


#endif /* JO_TOSDB_SHELL */