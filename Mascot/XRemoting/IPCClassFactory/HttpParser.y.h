// Created by Alex Bourakov 05/18/11
%{
struct yyhttp_token_type { 
	substring_locator _substr; 
	void clear() { 
		_substr.clear(); 
	} 
}; 
#define YYSTYPE yyhttp_token_type
%}

%name-prefix = "yyhttp"

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
int yyhttplex(yyhttp_token_type *yylval, std::string::const_iterator& _begin, std::string::const_iterator& _end); 
%} 


%token Tab NewLine Http Word Text 


%% /* The grammar follows. */ 

HttpObject 
	: MethodLine Headers 
	{ 
		_context.reduce_stack(); 
	} 
	| MethodLine Headers NewLine
	{ 
		_context.reduce_stack(); 
	} 
	| MethodLine Headers Text 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = "Payload"; 

		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Text"; 
		obj._vecCaptures[s - 1].second = $3._substr; 

		_context.process_flat_object(_context.get_current()); 
		_context.reduce_stack(); 
	} 
; 

MethodLine 
	: Method ' ' Url ' ' Http Version NewLine 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = "HttpMessage"; 

		size_t s = obj._vecCaptures.size(); 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Method"; 
		obj._vecCaptures[s - 1].second = $1._substr; 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Url"; 
		obj._vecCaptures[s - 1].second = $3._substr; 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Version"; 
		obj._vecCaptures[s - 1].second = $6._substr; 

		_context.place_composite_object_on_stack(_context.get_current()); 
	} 
	| Http Version Status ' ' Description NewLine 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = "HttpMessage"; 

		size_t s = obj._vecCaptures.size(); 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Version"; 
		obj._vecCaptures[s - 1].second = $2._substr; 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Status"; 
		obj._vecCaptures[s - 1].second = $3._substr; 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Description"; 
		obj._vecCaptures[s - 1].second = $5._substr; 

		_context.place_composite_object_on_stack(_context.get_current()); 
	} 
	| NewLine
	{
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj));

		obj._header.first = "HttpFormdata";

		_context.place_composite_object_on_stack(_context.get_current());
	}
	| error
	{ 
//		Debug: 
//		Error contains the last token before which the error was found. 
//		The trace will reset to this token. 
		$$ = $1;
	} 
; 

Headers 
	: Header 
	| Headers Header 
; 


Header 
	: HeaderName ':' HeaderValues NewLine 
	{
		_context.reduce_stack(); 
	}
; 

HeaderName 
	: Word 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = "Header"; 

		size_t s = obj._vecCaptures.size(); 
		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "HeaderName"; 
		obj._vecCaptures[s - 1].second = $1._substr; 

		_context.place_composite_object_on_stack(_context.get_current()); 
	} 
;

HeaderValues 
	: HeaderValue 
	| HeaderValues ';' HeaderValue 
; 


HeaderValue
	: HttpPairSet 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 
		obj._header.first = "HeaderValue"; 

		_context.process_flat_object(_context.get_current()); 
	} 
; 


HttpPairSet 
	: HttpPair 
	| HttpPairSet ',' HttpPair 
; 


HttpPair 
	: Name '=' Value 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 

		size_t s = obj._vecCaptures.size(); 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Value"; 
		obj._vecCaptures[s - 1].second = $3._substr; 
		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Name"; 
		obj._vecCaptures[s - 1].second = $1._substr; 
	} 
	| Value 
	{ 
		wsi_object& obj = *(reinterpret_cast<wsi_object*>(_context.get_current()._obj)); 

		size_t s = obj._vecCaptures.size(); 

		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Value"; 
		obj._vecCaptures[s - 1].second = $1._substr; 
		obj._vecCaptures.resize(++s); 
		obj._vecCaptures[s - 1].first = "Name"; 
		obj._vecCaptures[s - 1].second = ""; 
	} 
; 

Value 
	: Word 
	| Space Word 
	{ 
		$$ = $2;
	} 
	| Value ' ' Word 
	{ 
		$$ = $1; 
		$$._substr._end = $3._substr._end; 
	} 
	| Value ':' Word 
	{ 
		$$ = $1; 
		$$._substr._end = $3._substr._end; 
	} 
; 

Name 
	: Word 
	| Space Word
	{ 
		$$ = $2;
	} 
; 

Method 
	: Word 
; 

Url 
	: Words 
; 

Version 
	: Word 
; 

Status 
	: Word 
; 

Description 
	: Words 
	| Description ' ' Words 
	{ 
		$$ = $1; 
		$$._substr._end = $3._substr._end; 
	} 
; 

Words 
	: Word 
	| Words WordSeparators Word 
	{ 
		$$ = $1; 
		$$._substr._end = $3._substr._end; 
	} 
; 

WordSeparators 
	: WordSeparator 
	| WordSeparators WordSeparator
; 

WordSeparator 
	: ',' | '=' | ':' | ';'
; 

Space 
	: ' ' 
	| NewLine Tab 
	| ' ' NewLine Tab 
; 

