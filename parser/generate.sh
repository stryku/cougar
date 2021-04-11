rm -f generated/*
grep TOKEN ../lexer/token_type.def | sed -e 's~TOKEN(\([A-Za-z]\+\)),\?~%token \1~g' > generated/tokens.y
bisonc++ grammar.y 
cp parser_adaptor.hh.in generated/parser_adaptor.hh