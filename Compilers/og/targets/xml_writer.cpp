#include <string>
#include <iostream>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include <targets/symbol.h>
#include "ast/all.h"  // automatically generated
#include "og_parser.tab.h"

//---------------------------------------------------------------------------

static std::string type_name(std::shared_ptr<cdk::basic_type> type) {
  if (type == nullptr) return "";

  if (type->name() == cdk::typename_type::TYPE_INT) return "integer";
  else if (type->name() == cdk::typename_type::TYPE_DOUBLE) return "real";
  else if (type->name() == cdk::typename_type::TYPE_STRING) return "string";
  else if (type->name() == cdk::typename_type::TYPE_UNSPEC) return "auto";
  else if (type->name() == cdk::typename_type::TYPE_STRUCT) return "tuple";
  else if (type->name() == cdk::typename_type::TYPE_POINTER) {
    std::string s = "pointer";
    std::shared_ptr<cdk::basic_type> p = cdk::reference_type_cast(type)->referenced();
    if (p != nullptr){
      s += " to " + type_name(p);
    }
    return s;
  }
  else {
    return "unknown type";
  }
}

static std::string access_name(int access) {
  if (access == tPRIVATE) return "private";
  else if (access == tPUBLIC) return "public";
  else if (access == tREQUIRE) return "require";
  else return "unknown access";
}

//---------------------------------------------------------------------------

void og::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void og::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}
void og::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void og::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

void og::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_identity_node(og::identity_node *const node, int lvl) {   
  do_unary_operation(node, lvl);
}

void og::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void og::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() 
    << " names=[";

  auto names = node->identifiers();
  for (unsigned int i = 0; i < names->size(); i++) {
    os() << "'" << names->at(i) << "'";
    if ( (i+1) != names->size())
      os ()<< ", ";
  }

  os() << "] qualifier='" << access_name(node->qualifier()) 
    << "' type='" << type_name(node->type()) << "'>" << std::endl;

  if (node->expression()) {
    openTag("initializer", lvl + 2);  
    node->expression()->accept(this, lvl + 4);
    closeTag("initializer", lvl + 2);  
  }

  closeTag(node, lvl);
}

void og::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void og::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl + 2);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_print_node(og::print_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_read_node(og::read_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_block_node(og::block_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("declarations", lvl + 2);
  if (node->declarations()) node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl + 2);
  openTag("instructions", lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------


void og::xml_writer::do_for_node(og::for_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("inits", lvl + 2);
  node->inits()->accept(this, lvl + 4);
  closeTag("inits", lvl + 2);
  openTag("conditions", lvl + 2);
  node->conditions()->accept(this, lvl + 4);
  closeTag("conditions", lvl + 2);
  openTag("incs", lvl + 2);
  node->incs()->accept(this, lvl + 4);
  closeTag("incs", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_break_node(og::break_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void og::xml_writer::do_continue_node(og::continue_node *const node, int lvl) {   
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_return_node(og::return_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->expressions()->accept(this, lvl + 2); 
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_function_call_node(og::function_call_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "'>" << std::endl;
  
  openTag("arguments", lvl + 2);
  if (node->arguments()) {
    node->arguments()->accept(this, lvl + 4);
  }
  closeTag("arguments", lvl + 2);
  
  closeTag(node, lvl);
}

void og::xml_writer::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() 
    << "' qualifier='" << access_name(node->qualifier()); 
  if (node->type())
    os() << "' type='" << type_name(node->type());
  os() << "'>" << std::endl;;
  
  openTag("arguments", lvl + 2);
  if (node->arguments()) {
    node->arguments()->accept(this, lvl + 4);
  }
  closeTag("arguments", lvl + 2);

  closeTag(node, lvl);
}

void og::xml_writer::do_function_definition_node(og::function_definition_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;

  os() << std::string(lvl, ' ') << "<" << node->label() 
    << " name='" << node->identifier() << "' qualifier='" << access_name(node->qualifier());
  if (node->type())
    os () << "' type='" << type_name(node->type());
  os() << "'>" << std::endl;

  openTag("arguments", lvl + 2);
  if (node->arguments()) {
    node->arguments()->accept(this, lvl + 4);
  }
  closeTag("arguments", lvl + 2);

  node->body()->accept(this, lvl + 2);

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_stack_alloc_node(og::stack_alloc_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_address_of_node(og::address_of_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_sizeof_node(og::sizeof_node *const node, int lvl) {       
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->expressions()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_index_node(og::index_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->base()->accept(this, lvl + 2);
  node->index()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_tuple_node(og::tuple_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << " qualifier='" << access_name(node->qualifier()) << "'>" << std::endl;
  node->values()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->base()->accept(this, lvl + 2);
  os() << std::string(lvl + 2, ' ') << "<index>" << node->index() << "</index>" << std::endl;
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void og::xml_writer::do_if_node(og::if_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void og::xml_writer::do_if_else_node(og::if_else_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}
