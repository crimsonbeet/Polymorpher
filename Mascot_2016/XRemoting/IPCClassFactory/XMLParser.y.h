// Created by Alex Bourakov 03/05/12
%{
struct yyxml_token_type { 
	int _index; 
	substring_locator _substr; 
	substring_locator _namespace; 
	yyxml_token_type(): _index(0) { 
	} 
	void clear() { 
		_substr.clear(); 
		_namespace.clear(); 
		_index = 0; 
	} 
}; 
#define YYSTYPE yyxml_token_type
%}

%name-prefix = "yyxml"

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
int yyxmllex(yyxml_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end); 
%}


%token Word String Comment 


%% /* The grammar follows. */

XmlString 
	: ProcessingInstruction Elements
	{ 
		_context.flush(); 
	} 
	| Elements
	{ 
		_context.flush(); 
	} 
; 

ProcessingInstruction 
	: '<' '?' Word Attributes '?' '>' 
;

Elements
	: Element 
	| Elements Element 
	| error
;

Element
	: Comment 
	| ElementFlat
	| ElementComposit Elements ClosingTag 
	{ 
		_context.reduce_stack(); 
	} 
	| ElementComposit FlatContent ClosingTag 
	{ 
		_context.reduce_stack(); 
	} 
	| ElementComposit ClosingTag 
	{ 
		_context.reduce_stack(); 
	} 
; 

ElementFlat
	: '<' ElementName Attributes '/' '>'
	{ 
		_context.process_flat_object(_context.get_current()); 
	} 
	| '<' ElementName '/' '>'
	{ 
		_context.process_flat_object(_context.get_current()); 
	} 
; 

FlatContent 
	: Words 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_context_current()._obj)); 
		switch(*($1._substr._begin - 1)) { 
			case (int)'\"': case (int)'\'': $1._substr._begin--; break; 
		} 
		switch(*($1._substr._end)) { 
			case (int)'\"': case (int)'\'': $1._substr._end++; break; 
		} 
		obj._vecCaptures.resize(1); 
		obj._vecCaptures[0].first = obj._header.first; 
		obj._vecCaptures[0].second = $1._substr; 

//		Debug: 
//		Trace the entire image. 
		$$ = $1; 
	} 
; 

ElementComposit
	: '<' ElementName Attributes '>' 
	{ 
		$$ = $2; 
		_context.place_composite_object_on_stack(_context.get_current()); 
	} 
	| '<' ElementName '>' 
	{ 
		$$ = $2; 
		_context.place_composite_object_on_stack(_context.get_current()); 
	} 
; 

ClosingTag 
	: '<' '/' Name'>' 
	{ 
		$$ = $3
	}
; 

ElementName
	: Name 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = $1._substr; 
		obj._header.anamespace = $1._namespace; 
	} 
; 

Attributes 
	: Attribute
	| Attributes Attribute
	| error
	{ 
//		Debug: 
//		Error contains the last token before which the error was found. 
//		The trace will reset to this token. 
		$$ = $1;
		$$._index = 0;
	} 
; 

Attribute 
	: Name '=' String 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures.resize(s + 1); 
		obj._vecCaptures[s].first = $1._substr; 
		obj._vecCaptures[s].anamespace = $1._namespace; 
		obj._vecCaptures[s].second = $3._substr; 

//		Debug: 
//		Trace the entire image. 
		$$ = $1;
		$$._substr._end = $3._substr._end; 
	} 
; 

Name 
	: Word ':' Word  
	{ 
		$$ = $3; 
		$$._namespace = $1._substr 
	} 
	| Word
; 

Words 
	: WordSeparator 
	| Word 
	| String  
	| Words String 
	{ 
		$$ = $1; 
		$$._substr._end = $2._substr._end; 
	} 
	| Words Word 
	{ 
		$$ = $1; 
		$$._substr._end = $2._substr._end; 
	} 
	| Words WordSeparator 
	{ 
		$$ = $1; 
		$$._substr._end = $2._substr._end; 
	} 
; 

WordSeparator 
	: ':' | '?' | '!' | '=' | ',' | ';' | '{' | '}' | '/'
; 

