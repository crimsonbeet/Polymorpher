// Created by Alex Nikol Bourakov (ANB) around April/07
%{
struct yywsi_token_type { 
	int _index; 
	substring_locator _substr; 
	yywsi_token_type(): 
		_index(0) { 
	} 
	void clear() { 
		_substr.clear(); 
		_index = 0; 
	} 
}; 
#define YYSTYPE yywsi_token_type
%}

%name-prefix = "yywsi"

%pure-parser

%parse-param {std::string::const_iterator& _begin}
%lex-param {std::string::const_iterator& _begin}
%parse-param {std::string::const_iterator& _end}
%lex-param {std::string::const_iterator& _end}

%parse-param {IWSIMaterializer& _context}

%initial-action
{
	(reinterpret_cast<wsi_object*>(_context.get_current()._obj))->clear(); 
};

%{
int yywsilex(yywsi_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end); 
%}


%token Preambula

%token Number SignedNumber FloatNumber Name GeneralString


%% /* The grammar follows. */

WsiString 
	: Objects
	{ 
		_context.flush(); 
	} 
; 

Objects
	: Object 
	| Objects Object 
;

Object
	: Preambula ObjectContent
	{ 
		_context.process_flat_object(_context.get_current()); 
	} 
	| Preambula CObjectContent Objects CObjectEnd
	{ 
		_context.reduce_stack(); 
	} 
	| Preambula CObjectContent CObjectEnd
	{ 
		_context.reduce_stack(); 
	} 
	| ObjArrayHeader '[' Objects ']'
	{
		_context.reduce_stack();
	}
	| ObjArrayHeader '[' Attributes Objects ']'
	{
		_context.reduce_stack();
	}
	| ObjArrayHeader '[' Values ']'
	{
		_context.reduce_stack();
	}
	| ObjArrayHeader '[' ']'
	{
		_context.reduce_stack();
	}
;

ObjArrayHeader
	: '=' Header
	{
		$$ = $2;
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj));
		obj._vecCaptures.resize(0);
		obj._vecCaptures.resize(1);
		obj._vecCaptures[0].first = $2._substr; // so values will have a name

		_context.place_composite_object_on_stack(_context.get_current());
	}
;

CObjectContent 
	: '$' ObjectContent
	{ 
		_context.place_composite_object_on_stack(_context.get_current()); 
	}
;

CObjectEnd
	: '$' 
	| '$' Number
; 

ObjectContent
	: Header
	| Header Attributes
;

Attributes
	: Attribute
	{
//		Debug: 
//		Trace the entire image. 
		$$ = $1;
		$$._index = 0;
	} 
	| Attributes Attribute
	{ 
//		Debug: 
//		Trace the entire image. 
		$$ = $1; 
		$$._substr._end = $2._substr._end; 
	} 
;

Header
	: NUid
	{ 
		$$ = $1;
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.index = $1._index; 
		obj._header.first = $1._substr; 
	}
	| NUid ':' Number
	{ 
		$$ = $1;
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.index = $1._index; 
		obj._header.first = $1._substr; 

		obj._header.second = $3._substr; 
	}
;

NUid
	: Name
	| Name '@' Number
	{
		$$ = $1;
		$$._index = atoi($3._substr.str().c_str());
	}
;

Attribute
	: Uid '=' Value
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_named_current()._obj));
		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures[s - 1].second = $3._substr; 

//		Debug: 
//		Trace the entire image. 
		$$ = $1;
		$$._substr._end = $3._substr._end; 
	} 
	| Uid '=' '{' Values '}'
	{ 
//		Debug: 
//		Trace the entire image. 
		$$ = $1;
		$$._substr._end = $5._substr._end;
	} 
;

Uid
	: Name 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj));
		size_t s = obj._vecCaptures.size(); 
		if(s > 0 && obj._vecCaptures[s - 1].second.size() == 0) { // it can be one created by ObjArrayHeader
			--s;
		}
		else {
			obj._vecCaptures.resize(s + 1);
		}
		obj._vecCaptures[s].first = $1._substr; 
	} 
	| Name '@' Number
	{ 
		$$ = $1; 
		$$._index = atoi($3._substr.str().c_str()); 

		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		size_t s = obj._vecCaptures.size(); 
		if(s > 0 && obj._vecCaptures[s - 1].second.size() == 0) {
			--s;
		}
		else {
			obj._vecCaptures.resize(s + 1);
		}
		obj._vecCaptures[s].first = $1._substr; 
		obj._vecCaptures[s].index = $$._index; 
	}
; 


Values 
	: Value 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_named_current()._obj));
		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures[s - 1].second = $1._substr; 
	} 
	| Values Value 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_named_current()._obj));
		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures.resize(s + 1); 
		obj._vecCaptures[s].second = $2._substr; 
		obj._vecCaptures[s].first = obj._vecCaptures[s - 1].first; 
	} 
;


Value 
	: Number 
	| SignedNumber 
	{ 
		$$ = $1; 
		if($$._substr._begin[0] == '+') { 
			$$._substr._begin++; 
		} 
	}
	| FloatNumber
	{ 
		$$ = $1; 
		if($$._substr._begin[0] == '+') { 
			$$._substr._begin++; 
		} 
	} 
	| Name
	| GeneralString
;



