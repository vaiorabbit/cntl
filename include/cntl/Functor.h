// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef CNTL_FUNCTOR_H_INCLUDED
#define CNTL_FUNCTOR_H_INCLUDED

#if defined(__APPLE__)
#  include <tr1/memory> // std::tr1::shared_ptr
#else
#  include <memory> // std::tr1::shared_ptr
#endif

namespace cntl
{
    namespace Detail
    {
        template <typename TArg, typename TRet>
        class IUnFunctorImpl
        {
        public:
            virtual ~IUnFunctorImpl() {}
            virtual IUnFunctorImpl* Clone() const =0;
            virtual TRet operator()( TArg arg ) =0;
        };

        template <typename TArg, typename TRet, typename TUnFuncObj>
        class UFIFuncObj : public IUnFunctorImpl<TArg, TRet>
        {
            TUnFuncObj func_obj;

        public:
            UFIFuncObj( const TUnFuncObj& func_obj_ ) : func_obj( func_obj_ ) {}

            virtual UFIFuncObj* Clone() const { return new UFIFuncObj( *this ); }

            virtual TRet operator ()( TArg arg ) { return func_obj( arg ); }
        };

        template <typename TArg, typename TRet, typename TObjPtr, typename TUnMemFunc>
        class UFIMemFunc : public IUnFunctorImpl<TArg, TRet>
        {
            TObjPtr pObjPtr;
            TUnMemFunc pMemFunc;

        public:
            UFIMemFunc( const TObjPtr& pObjPtr_, TUnMemFunc pMemFunc_ )
                : pObjPtr( pObjPtr_ ) , pMemFunc( pMemFunc_ ) {}

            virtual UFIMemFunc* Clone() const { return new UFIMemFunc( *this ); }

            virtual TRet operator ()( TArg arg ) { return ((*pObjPtr).*pMemFunc)( arg ); }
        };
    } // End : namespace Detail

    template <typename TArg, typename TRet>
    class UnFunctor
    {
        std::tr1::shared_ptr< Detail::IUnFunctorImpl<TArg, TRet> > pImpl;

    public:
        UnFunctor() : pImpl( NULL ) {}
        UnFunctor( const UnFunctor& other ) : pImpl( other.pImpl ? other.pImpl->Clone() : NULL ) {}

        template <typename TUnFuncObj>
        UnFunctor( TUnFuncObj func_obj )
            : pImpl( new Detail::UFIFuncObj<TArg, TRet, TUnFuncObj>( func_obj ) ) {}

        template <typename TObjPtr, typename TUnMemFunc>
        UnFunctor( TObjPtr pObject, TUnMemFunc pMemFunc )
            : pImpl( new Detail::UFIMemFunc<TArg, TRet, TObjPtr, TUnMemFunc>(pObject, pMemFunc) ) {}

        UnFunctor& operator =( const UnFunctor& other )
            {
                UnFunctor temp( other );
                pImpl.swap( temp.pImpl );
                return *this;
            }

        TRet operator ()( TArg arg ) { return (*pImpl)( arg ); }
    }; // End : class UnFunctor
}

#endif
