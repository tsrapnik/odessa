#pragma once

//including this header enables bitwise logic operations for all enum types.

template< typename enum_type >   
inline enum_type operator &( enum_type left_argument, enum_type right_argument )  
{
    using underlying_type =  __underlying_type( enum_type );
    return static_cast< enum_type >( static_cast< underlying_type >( left_argument) & static_cast< underlying_type >( right_argument ) );
}

template< typename enum_type >   
inline enum_type operator |( enum_type left_argument, enum_type right_argument )  
{
    using underlying_type =  __underlying_type( enum_type );
    return static_cast< enum_type >( static_cast< underlying_type >( left_argument ) | static_cast< underlying_type >( right_argument ) );
}

template< typename enum_type >   
inline enum_type operator ^( enum_type left_argument, enum_type right_argument )  
{
    using underlying_type =  __underlying_type( enum_type );
    return static_cast< enum_type >( static_cast< underlying_type >( left_argument) ^ static_cast< underlying_type >( right_argument ) );
}

template< typename enum_type >   
inline enum_type operator ~( enum_type right_argument )  
{
    using underlying_type =  __underlying_type( enum_type );
    return static_cast< enum_type >( ~static_cast< underlying_type >( right_argument ) );
}

template< typename enum_type >   
inline enum_type& operator &=( volatile enum_type& left_argument, enum_type right_argument )  
{
    left_argument = left_argument & right_argument;
    return const_cast< enum_type& >( left_argument );
}

template< typename enum_type >   
inline enum_type& operator |=( volatile enum_type& left_argument, enum_type right_argument )  
{
    left_argument = left_argument | right_argument;
    return const_cast< enum_type& >( left_argument );
}

template< typename enum_type >   
inline enum_type& operator ^=( volatile enum_type& left_argument, enum_type right_argument )  
{
    left_argument = left_argument ^ right_argument;
    return const_cast< enum_type& >( left_argument );
}