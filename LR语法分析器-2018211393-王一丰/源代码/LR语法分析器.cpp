#include"dfa.h"
#include"item.h"
#include"grammar.h"
#include"parser.h"
#include<iostream>

int main()
{
	parser p;
	p.dfa.ini();
	p.table_derivation();
	p.output_parser_table();
	p.input_token();
	p.predict();
}