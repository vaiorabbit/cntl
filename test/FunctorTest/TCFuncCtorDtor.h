// -*- mode: C++; coding: utf-8 -*-
#ifndef TCFUNCCTORDTOR_H_INCLUDED
#define TCFUNCCTORDTOR_H_INCLUDED

#include <queue>
#include <cntl/Functor.h>
#include <TestFramework.h>

typedef cntl::UnFunctor<void*, void> Handler;
typedef std::queue< std::tr1::shared_ptr<Handler> > HandlerQueue;

void Handler0_Func( void* arg )
{
    (void)arg;
    std::puts( "Handler0_Func" );
}

struct Handler1_FuncObj
{
    void operator ()( void* arg )
        {
            (void)arg;
            std::puts( "Handler1_FuncObj" );
        }
};

class CHandler
{
public:
    void Handle2_Method( void* arg )
        {
            (void)arg;
            std::puts( "Handle2_Method" );
        }
};


class TCFuncCtorDtor : public Test::Case
{
public:
    TCFuncCtorDtor( const char* name )
        : Test::Case( name )
        {}

    virtual void Run()
        {
            HandlerQueue q;

            Handler1_FuncObj handler1;
            CHandler handler2;

            std::tr1::shared_ptr<Handler> cmd0( new Handler(Handler0_Func) );
            q.push( cmd0 );
            std::tr1::shared_ptr<Handler> cmd1( new Handler(handler1) );
            q.push( cmd1 );
            std::tr1::shared_ptr<Handler> cmd2( new Handler(&handler2, &CHandler::Handle2_Method) );
            q.push( cmd2 );

            while ( !q.empty() )
            {
                std::tr1::shared_ptr<Handler> pfnCmd = q.front();
                (*pfnCmd)( NULL );
                q.pop();
            }

            return;
        }
};

#endif
