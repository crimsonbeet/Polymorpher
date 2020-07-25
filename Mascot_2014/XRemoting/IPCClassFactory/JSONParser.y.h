// Created by Aliaksandr Burakou 
%{ 
struct yyjson_token_type { 
	substring_locator _substr; 
	yyjson_token_type() { 
	} 
	void clear() { 
		_substr.clear(); 
	} 
}; 
#define YYSTYPE yyjson_token_type
void yyparser_inherit_context(IWSIMaterializer& _context) { 
	wsi_object& obj_prev = *(reinterpret_cast<wsi_object*>(_context.get_context_current()._obj)); 
	wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
	obj._header.first = obj_prev._header.first; 
	obj._header.second = 0; 
} 
%} 

%name-prefix = "yyjson"

%pure-parser

%parse-param {std::string::const_iterator& _begin}
%lex-param {std::string::const_iterator& _begin}
%parse-param {std::string::const_iterator& _end}
%lex-param {std::string::const_iterator& _end}

%parse-param {IWSIMaterializer& _context}

%initial-action
{
	(reinterpret_cast<wsi_object*>(_context.get_current()._obj))->clear(); 
	//yydebug = 1;
};

%{
int yyjsonlex(yyjson_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end); 
%}


%token Number SignedNumber FloatNumber NumberExp FloatNumberExp true_token false_token null_token Word GeneralString


%% /* The grammar follows. */

JsonString 
	: '{' '}'
	{ 
		_context.flush(); 
	} 
	| '{' Elements '}'
	{ 
		_context.flush(); 
	} 
; 

Elements
	: Element 
	| Elements ',' Element 
;

Element
	: ElementName '}' 
	{ 
		_context.reduce_stack(); 
	} 
	| ElementName Attributes '}' 
	{ 
		_context.reduce_stack(); 
	} 
; 

ElementName 
	: Name ':' '{' 
	{
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = $1._substr; 
		_context.place_composite_object_on_stack(_context.get_current()); 
	} 
; 

ObjectContent
	: ObjectContentBegin '}'
	{ 
		yyparser_inherit_context(_context); 
		_context.reduce_stack(); 
	} 
	| ObjectContentBegin Attributes '}'
	{ 
		yyparser_inherit_context(_context); 
		_context.reduce_stack(); 
	} 
; 

ObjectContentBegin
	: '{'
	{ 
		_context.place_composite_object_on_stack(_context.get_current()); 
		yyparser_inherit_context(_context); 
	}
;

ObjectArrayContent
	: ObjectContent
	| ObjectArrayContent ',' ObjectContent
; 

ValueArrayContent
	: Value
	{
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_context_current()._obj)); 
		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures.resize(s + 1); 
		obj._vecCaptures[s].first = (*(reinterpret_cast<wsi_object*>(_context.get_current()._obj)))._header.first; 
		obj._vecCaptures[s].second = $1._substr; 
	} 
	| ValueArrayContent ',' Value
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_context_current()._obj)); 
		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures.resize(s + 1); 
		obj._vecCaptures[s].first = obj._vecCaptures[s - 1].first; 
		obj._vecCaptures[s].second = $3._substr; 
	} 
	| error
	{ 
//		Debug: Error contains the last token before which the error was found. 
//		The trace will reset to this token. 
		$$ = $1;
	} 
;

Attribute
	: Name ':' Value 
	{
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_context_current()._obj)); 
		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures.resize(s + 1); 
		obj._vecCaptures[s].first = $1._substr; 
		obj._vecCaptures[s].second = $3._substr; 
	} 
	| AttributeArrayName ']'
	{ 
	} 
	| AttributeArrayName ValueArrayContent ']'
	{ 
	} 
	| AttributeArrayName ObjectArrayContent ']'
	{ 
	} 
; 

AttributeArrayName
	: Name ':' '['
	{
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = $1._substr; 
		obj._header.second = 0; 
	}
;

Attributes
	: Attribute
	{
		$$ = $1;
	} 
	| Element 
	{
	}
	| Attributes ',' Attribute
	{ 
		$$ = $1; 
		$$._substr._end = $3._substr._end; 
	} 
	| Attributes ',' Element
	{
	}
	| error
	{ 
//		Debug: Error contains the last token before which the error was found. 
//		The trace will reset to this token. 
		$$ = $1;
	} 
;

Value 
	: GeneralString 
	{
		$$ = $1;
	} 
	| Word 
	{
		$$ = $1;
	} 
	| Number 
	{
		$$ = $1;
	} 
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
	| NumberExp Number
	{
		$$ = $1; 
		$$._substr._end = $2._substr._end; 
	}
	| FloatNumberExp Number
	{
		$$ = $1; 
		$$._substr._end = $2._substr._end; 
	}
	| true_token 
	| false_token  
	| null_token 
; 

Name
	: GeneralString
	| Word 
	| Number 
	| Number Word 
	{
		$$ = $1; 
		$$._substr._end = $2._substr._end; 
	}
; 

