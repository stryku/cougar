%thread-safe
%namespace Cougar_generated
%class-name Parser_generated
%target-directory generated
%implementation-header parser_adaptor.hh
%no-lines
%print-tokens
%scanner-matched-text-function Cougar::Parser::LexerAdaptor::currentText()
%stype std::string_view
%include generated/tokens.y

%%

module:
  module_decls
;

module_decls:
  module_decl
|
  module_decls
;

module_decl:
  module_name_defitnion
|
  external_function_declaration
;

module_name_defitnion:
  KwModule Identifier Semicolon
  {
    fmt::print("Module defitnition: 1={} 2={}\n", $1, $2);
  }
;

external_function_declaration:
  KwExternal KwFunction Identifier Identifier ParentOpen param_declations ParentClose Semicolon
;

param_declations:
  param_declaration
|
  param_declations Comma param_declaration
;

param_declaration:
  Identifier
|
  Identifier Identifier
;