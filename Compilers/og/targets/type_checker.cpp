#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void og::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_reference_type(4, cdk::make_primitive_type(0, cdk::TYPE_VOID)));
}

void og::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
}
void og::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
}

//---------------------------------------------------------------------------

void og::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  node->type(node->argument()->type());
}

void og::type_checker::processLogicalUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void og::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl, bool PMP) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)) {
    // Int + Pointer
    node->type(node->right()->type()); // keep pointer type
  }
  else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    // Pointer + Int
    node->type(node->left()->type()); // keep pointer type
  }
  else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT))
    // Int + Int
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));  
  else if ((node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) 
    || ( node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) 
    || ( node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)))
    // Int/Double + Int/Double (at least one double) or vs
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));  
  else if ( PMP && (( node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_POINTER))) )
    node->type(cdk::make_primitive_type(4, cdk::TYPE_POINTER));  
  else 
    throw std::string("wrong type of arguments in binary expression");
}

void og::type_checker::processIntOnlyExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::processIDArithmeticBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT) && !node->left()->is_typed(cdk::TYPE_DOUBLE)) 
    throw std::string("wrong type in left argument of arithmetic binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT) && !node->right()->is_typed(cdk::TYPE_DOUBLE)) 
    throw std::string("wrong type in right argument of arithmetic binary expression");

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE))
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  else
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::processILogicalBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of logical binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of logical binary expression");

  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::processIDLogicalBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT) && !node->left()->is_typed(cdk::TYPE_DOUBLE)) throw std::string("wrong type in left argument of logical binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT) && !node->right()->is_typed(cdk::TYPE_DOUBLE)) throw std::string("wrong type in right argument of logical binary expression");

  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::processPIDLogicalBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (!node->left()->is_typed(cdk::TYPE_INT) 
    && !node->left()->is_typed(cdk::TYPE_DOUBLE) 
    && !node->left()->is_typed(cdk::TYPE_POINTER)) 
      throw std::string("wrong type in left argument of logical binary expression");

  if (!node->right()->is_typed(cdk::TYPE_INT) 
    && !node->right()->is_typed(cdk::TYPE_DOUBLE) 
    && !node->right()->is_typed(cdk::TYPE_POINTER)) 
      throw std::string("wrong type in right argument of logical binary expression");

  if (node->left()->type()->name() != node->right()->type()->name() 
      && !(node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
      && !(node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)) 
      throw std::string("wrong type argument of logical binary expression");

  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void og::type_checker::do_identity_node(og::identity_node *const node, int lvl) {   
  ASSERT_UNSPEC;
  processUnaryExpression(node, lvl);
}
void og::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processUnaryExpression(node, lvl);
}
void og::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processLogicalUnaryExpression(node, lvl);
}
void og::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processILogicalBinaryExpression(node, lvl);
}
void og::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processILogicalBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processBinaryExpression(node, lvl, false); //CASO 1 (PID)
}
void og::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processBinaryExpression(node, lvl, true); //CASO 2 (PID + PP)
}
void og::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processIDArithmeticBinaryExpression(node, lvl); //CASO 3 (ID)
}
void og::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processIDArithmeticBinaryExpression(node, lvl); //CASO 3 (ID)
}
void og::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processIntOnlyExpression(node, lvl); //CASO 4 (I)
}
//---------------------------------------------------------------------------
void og::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processIDLogicalBinaryExpression(node, lvl);
}
void og::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processIDLogicalBinaryExpression(node, lvl);
}
void og::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processIDLogicalBinaryExpression(node, lvl);
}
void og::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processIDLogicalBinaryExpression(node, lvl);
}
void og::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processPIDLogicalBinaryExpression(node, lvl);
}
void og::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  ASSERT_UNSPEC;
  processPIDLogicalBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void og::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
    _parent->set_new_symbol(symbol);
  } else {
    throw id;
  }
}

void og::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void og::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 2);
  _readType = node->lvalue()->type();
  _allocType = node->lvalue()->type();
  node->rvalue()->accept(this, lvl + 2);

  if (node->lvalue()->type()->name() != node->rvalue()->type()->name() && 
      !(node->lvalue()->type()->name() == cdk::TYPE_DOUBLE && node->rvalue()->type()->name() == cdk::TYPE_INT)) 
        throw std::string("Assignment does not have matching types.");

  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------
void og::type_checker::do_evaluation_node(og::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void og::type_checker::do_print_node(og::print_node *const node, int lvl) {
  _readType = cdk::make_primitive_type(4, cdk::TYPE_INT);
  node->argument()->accept(this, lvl + 2);
  
  for (size_t i = 0; i < node->argument()->size(); i++) {
    auto type = dynamic_cast<cdk::expression_node*>(node->argument()->node(i))->type();
    if (type->name() == cdk::TYPE_POINTER || type->name() == cdk::TYPE_STRUCT)
      throw std::string("cannot print a value with the type " + type->name());
  }
}

//---------------------------------------------------------------------------

void og::type_checker::do_read_node(og::read_node *const node, int lvl) {
  ASSERT_UNSPEC;

  if (_readType) {
    if (_readType->name() == cdk::TYPE_POINTER) {
      throw std::string("Can only input integers or real values");
    }
    node->type(_readType);
  } else node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void og::type_checker::do_block_node(og::block_node *const node, int lvl) {
  if (node->declarations())
    node->declarations()->accept(this, lvl + 4);
  if (node->instructions())
    node->instructions()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void og::type_checker::do_for_node(og::for_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_break_node(og::break_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_continue_node(og::continue_node *const node, int lvl) {   
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_return_node(og::return_node *const node, int lvl) {
  _readType = _function->type();
  if (node->expressions()) {
    node->expressions()->accept(this, lvl);
    if (_function->type()->name() == cdk::TYPE_VOID)
      throw std::string("Function is void, return has expression");

    std::shared_ptr<cdk::basic_type> type;
    if (node->expressions()->values()->size() > 1) {
      std::vector<std::shared_ptr<cdk::basic_type>> types;
      for (size_t i = 0; i < node->expressions()->values()->size(); i++) {
        auto exprType = dynamic_cast<cdk::expression_node*>(node->expressions()->values()->node(i))->type();
        types.push_back(exprType);
      }

      type = cdk::make_structured_type(types);
    }
    else type = dynamic_cast<cdk::expression_node*>(node->expressions()->values()->node(0))->type();

    if (_function->type()->name() == cdk::TYPE_UNSPEC) {
        _function->type(type);
    }

    if (_function->type()->name() != type->name() && !(_function->type()->name() == cdk::TYPE_DOUBLE && type->name() == cdk::TYPE_INT))
        throw std::string("Return expression does not have the same type as its function.");
    else if (_function->type()->name() == cdk::TYPE_STRUCT) {
      auto subType = std::dynamic_pointer_cast<cdk::structured_type>(_function->type());
      auto nodeType = std::dynamic_pointer_cast<cdk::structured_type>(type);

      if (subType->length() != nodeType->length())
        throw std::string("Return expression does not have the same type as its function.");

      for (size_t i = 0; i < nodeType->length(); i++) {
        if (subType->component(i)->name() == cdk::TYPE_INT && nodeType->component(i)->name() == cdk::TYPE_DOUBLE)
          _function->type(type);

        else if (subType->component(i)->name() != nodeType->component(i)->name() 
            && !(subType->component(i)->name() == cdk::TYPE_DOUBLE && nodeType->component(i)->name() == cdk::TYPE_INT))
          throw std::string("Return expression does not have the same type as its function.");
      }
    }
  }
}

//---------------------------------------------------------------------------

void og::type_checker::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
  std::string id;

  //Check if is main
  if (node->identifier() == "og")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  std::shared_ptr<og::symbol> function = std::make_shared<og::symbol>(node->type(), id);
  function->function(true);
  function->qualifier(node->qualifier());

  std::shared_ptr<og::symbol> previous = _symtab.find(function->name());

  if (previous)
    throw std::string("conflicting declaration for '" + function->name() + "'");
  else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

void og::type_checker::do_function_definition_node(og::function_definition_node *const node, int lvl) {
  std::string id;

  //Check if is main
  if (node->identifier() == "og") {
    id = "_main";
  }
  else if (node->identifier() == "_main") {
    id = "._main";
  }
  else
    id = node->identifier();

  std::shared_ptr<og::symbol> function = std::make_shared<og::symbol>(node->type(), id);
  function->offset(-node->type()->size());
  function->function(true);
  function->qualifier(node->qualifier());

  std::shared_ptr<og::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->qualifier() == node->qualifier()) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    }
    else
      throw std::string("conflicting definition for '" + function->name() + "'");
  }
  else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }

  _function = function;

  // Pass typechecker in body for calculating size to enter and args
  _symtab.push(); // args scope

  std::vector<std::shared_ptr<cdk::basic_type>> args;
  if (node->arguments()) {
    for (size_t i = 0; i < node->arguments()->size(); i++) {
      cdk::basic_node *arg = node->arguments()->node(i);
      if (arg == nullptr) break;
      arg->accept(this, 0);
      args.push_back(dynamic_cast<og::variable_declaration_node*>(node->arguments()->node(i))->type());
    }
  }
  // Update args to allow recursion in body
  function->args(args);
  _symtab.replace(function->name(), function);

  node->body()->accept(this, lvl);
  _symtab.pop();
    

  if (node->type()->name() == cdk::TYPE_STRUCT)
    function->offset(-4);

  _symtab.replace(function->name(), function);
  _function = function;
  _parent->reset_new_symbol();
  _parent->set_new_symbol(_function);
}

//---------------------------------------------------------------------------

void og::type_checker::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {
  
  auto names = node->identifiers();
  int nVars = names->size();
  int nExprs = 0;

  _readType = node->type();
  _allocType = node->type();

  if (node->expression() != nullptr) {
    node->expression()->accept(this, lvl);
    if (node->type()->name() == cdk::TYPE_UNSPEC) node->type(node->expression()->type());
    if (node->type()->name() == cdk::TYPE_STRUCT) nExprs = std::dynamic_pointer_cast<cdk::structured_type>(node->expression()->type())->length();
  }

  for (unsigned int i = 0; i < names->size(); i++) {
    auto id = node->identifiers()->at(i);

    std::shared_ptr<cdk::basic_type> type = node->type();
    bool isTuple = false;
    if (node->type()->name() == cdk::TYPE_STRUCT) isTuple = true;

    if ( node->expression() && isTuple) {
      og::tuple_node* tuple = dynamic_cast<og::tuple_node*>(node->expression());

      if ( nExprs > 0 && nVars == nExprs ) {
        type = std::dynamic_pointer_cast<cdk::structured_type>(tuple->type())->component(i);
        isTuple = false;
      }
    }

    std::shared_ptr<og::symbol> symbol = std::make_shared<og::symbol>(type, id);
    symbol->isTuple(isTuple);

    if (_symtab.insert(id, symbol)) {
      _parent->set_new_symbol(symbol);
    }
    else
      throw std::string("variable '" + id + "' redeclared");
  }

  if (node->expression() != nullptr) {
    node->expression()->accept(this, lvl);
    if (node->type()->name() == cdk::TYPE_UNSPEC) node->type(node->expression()->type());
  }
}

//---------------------------------------------------------------------------

void og::type_checker::do_stack_alloc_node(og::stack_alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  auto type = _allocType;
  if (!type || type->name() != cdk::TYPE_POINTER)
    throw std::string( "Stack alloc has to be stored in a pointer");

  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != cdk::TYPE_INT) throw std::string( "integer expression expected in allocation expression");

  node->type(type);
}

//---------------------------------------------------------------------------

void og::type_checker::do_address_of_node(og::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl);
  node->type(cdk::make_reference_type(4, node->lvalue()->type()));
}

//---------------------------------------------------------------------------

void og::type_checker::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->expressions()->accept(this, lvl);
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}
//---------------------------------------------------------------------------

void og::type_checker::do_index_node(og::index_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->base()->accept(this, lvl + 2);
  if (node->base()->type()->name() != cdk::TYPE_POINTER)
    throw std::string("Pointer expression expected in index left-value");

  node->index()->accept(this, lvl + 2);
  if (node->index()->type()->name() != cdk::TYPE_INT)
    throw std::string("integer expression expected in left-value index");

  node->type( std::dynamic_pointer_cast<cdk::reference_type>(node->base()->type())->referenced() ); 
}

//---------------------------------------------------------------------------

void og::type_checker::do_tuple_node(og::tuple_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->values()->accept(this, lvl);

  if (node->values()->size() == 1 && dynamic_cast<cdk::expression_node*>(node->values()->node(0))->type()->name() == cdk::TYPE_STRUCT) {
    node->type(dynamic_cast<cdk::expression_node*>(node->values()->node(0))->type());
  }
  else {
    std::vector<std::shared_ptr<cdk::basic_type>> types;
    for (size_t i = 0; i < node->values()->size(); i++) {
      auto type = dynamic_cast<cdk::expression_node*>(node->values()->node(i))->type();
      types.push_back(type);
    }

    node->type(cdk::make_structured_type(types));
  }
}
//---------------------------------------------------------------------------

void og::type_checker::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->base()->accept(this, lvl);
  node->type(std::dynamic_pointer_cast<cdk::structured_type>(node->base()->type())->component(node->index()-1));
}

//---------------------------------------------------------------------------

void og::type_checker::do_function_call_node(og::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;

  auto id = node->identifier();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);

  if (symbol == nullptr) throw std::string("symbol '" + id + "' not declared.");

  if (!symbol->function()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());

  if (node->arguments()) {
    unsigned int argsSize = symbol->args()->size();
    if (node->arguments()->size() != argsSize)
      throw std::string("Function '" + id + "' requires " + std::to_string(argsSize) + " arguments.");
    
    for (size_t i = 0; i < argsSize; i++) {
      _readType = symbol->args()->at(i);
      auto currNode = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
      auto argType = symbol->args()->at(i);
      currNode->accept(this, lvl + 4);      
      if (argType->name() != currNode->type()->name() 
          && !(argType->name() == cdk::TYPE_DOUBLE && currNode->type()->name() == cdk::TYPE_INT)) {
        throw std::string("Arguments do not match called function's arguments type.");
      }
    }
  }
}

void og::type_checker::do_if_node(og::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->block()->accept(this, lvl + 4);
}

void og::type_checker::do_if_else_node(og::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->thenblock()->accept(this, lvl + 4);
  node->elseblock()->accept(this, lvl + 4);
}
