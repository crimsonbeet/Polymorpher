// Created by Alex Nikol Bourakov (ANB) around April/07

#ifndef exportstlmacroH
#define exportstlmacroH


#if defined(EXP_STL)

#define EXPIMP_TEMPLATE_CLASS template class __declspec(dllexport) 
#define EXPIMP_TEMPLATE_STRUCT template struct __declspec(dllexport) 

#if !defined(__BORLANDC__)

//#pragma warning( disable:4231 )



//class EXP std::ios_base; 
EXPIMP_TEMPLATE_CLASS std::allocator<char>; 
//EXPIMP_TEMPLATE_CLASS std::basic_ios<char, std::char_traits<char> >; 
EXPIMP_TEMPLATE_CLASS std::basic_stringstream<char>; 
//EXPIMP_TEMPLATE_CLASS std::basic_ostringstream<char>; 
EXPIMP_TEMPLATE_CLASS std::basic_string<char>;

#define EXPORT_TEMPLATE_STRUCT(S) EXPIMP_TEMPLATE_STRUCT S; 
#define EXPORT_TEMPLATE_CLASS(C) EXPIMP_TEMPLATE_CLASS C; 
 


#define EXPORT_STL_VECTOR( vectype ) \
EXPIMP_TEMPLATE_CLASS std::allocator< vectype >; \
EXPIMP_TEMPLATE_CLASS std::vector< vectype >;



#define TSET_TRAITS(K, MULTI) \
std::_Tset_traits<K, std::less<K>, std::allocator<K>, MULTI> 

#define EXPORT_STL_SET_EX( settype, M ) \
EXPIMP_TEMPLATE_STRUCT std::less< settype >; \
EXPIMP_TEMPLATE_CLASS std::allocator< settype >; \
EXPIMP_TEMPLATE_CLASS std::allocator< std::_Tree_nod< TSET_TRAITS(settype, M) >::_Node >; \
EXPIMP_TEMPLATE_CLASS std::allocator< std::_Tree_ptr< TSET_TRAITS(settype, M) >::_Nodeptr >; 

#define EXPORT_STL_SET( settype ) \
EXPORT_STL_SET_EX( settype, false ) \
EXPIMP_TEMPLATE_CLASS std::set< settype >; 

#define EXPORT_STL_MULTISET( settype ) \
EXPORT_STL_SET_EX( settype, true ) \
EXPIMP_TEMPLATE_CLASS std::multiset< settype >; 



#define TMAP_TRAITS_1(K, V, MULTI) \
std::_Tmap_traits<K, V, std::less<K>, std::allocator< std::pair<const K, V> >, MULTI> 

#define TMAP_TRAITS_2(K, V, MULTI) \
std::_Tmap_traits<K, V, std::less<K>, std::allocator< TMAP_TRAITS_1(K, V, MULTI)::value_type >, MULTI> 

#define EXPORT_STL_MAP_EX( mapkey, mapvalue, M ) \
EXPIMP_TEMPLATE_STRUCT std::pair< mapkey, mapvalue >; \
EXPIMP_TEMPLATE_STRUCT std::less< mapkey >; \
EXPIMP_TEMPLATE_CLASS std::allocator< std::pair<const mapkey, mapvalue> >; \
EXPIMP_TEMPLATE_CLASS std::allocator< TMAP_TRAITS_1(mapkey, mapvalue, M)::value_type >; \
EXPIMP_TEMPLATE_CLASS std::allocator< std::_Tree_nod< TMAP_TRAITS_1(mapkey, mapvalue, M) >::_Node >; \
EXPIMP_TEMPLATE_CLASS std::allocator< std::_Tree_ptr< TMAP_TRAITS_1(mapkey, mapvalue, M) >::_Nodeptr >; \
EXPIMP_TEMPLATE_CLASS std::allocator< std::_Tree_nod< TMAP_TRAITS_2(mapkey, mapvalue, M) >::_Node >; \
EXPIMP_TEMPLATE_CLASS std::allocator< std::_Tree_ptr< TMAP_TRAITS_2(mapkey, mapvalue, M) >::_Nodeptr >; 


#define EXPORT_STL_MAP( mapkey, mapvalue ) \
EXPORT_STL_MAP_EX( mapkey, mapvalue, false ) \
EXPIMP_TEMPLATE_CLASS std::map<mapkey, mapvalue>; 


#define EXPORT_STL_MULTIMAP( mapkey, mapvalue ) \
EXPORT_STL_MAP_EX( mapkey, mapvalue, true ) \
EXPIMP_TEMPLATE_CLASS std::multimap<mapkey, mapvalue>; 


#define EXPORT_STL_STACK( value ) \
EXPIMP_TEMPLATE_CLASS std::allocator< std::_Deque_map<value, std::allocator<value> >::_Tptr >; \
EXPIMP_TEMPLATE_CLASS std::allocator< value >; \
EXPIMP_TEMPLATE_CLASS std::deque< value >; \
EXPIMP_TEMPLATE_CLASS std::stack< value >; 

#endif // !defined(__BORLANDC__)


#else // defined(EXP_STL)

#ifndef EXPIMP_TEMPLATE_CLASS
#define EXPIMP_TEMPLATE_CLASS
#endif
#ifndef EXPIMP_TEMPLATE_STRUCT
#define EXPIMP_TEMPLATE_STRUCT
#endif

#define EXPORT_TEMPLATE_STRUCT(S) 
#define EXPORT_TEMPLATE_CLASS(C) 

#define EXPORT_STL_VECTOR( vectype )
#define EXPORT_STL_SET( settype )
#define EXPORT_STL_MULTISET( settype )
#define EXPORT_STL_MAP( mapkey, mapvalue )
#define EXPORT_STL_MULTIMAP( mapkey, mapvalue )
#define EXPORT_STL_STACK( value )


#endif


#endif //exportstlmacroH

