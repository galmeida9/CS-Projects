#include <string>
#include <sstream>
#include "targets/frame_size_calculator.h"
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated
#include "og_parser.tab.h"

//---------------------------------------------------------------------------

void og::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_nullptr_node(og::nullptr_node *const node, int lvl) {      
  ASSERT_SAFE_EXPRESSIONS;

  if (_inFunctionBody) {
    _pf.INT(0);
  }
  else {
    _pf.SINT(0);
  }
}

void og::postfix_writer::do_tuple_node(og::tuple_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto type = std::dynamic_pointer_cast<cdk::structured_type>(node->type());
  if (type->length() == 1) {
    node->values()->node(0)->accept(this, lvl+2);
  } else if (!_inFunctionBody) {
    node->values()->accept(this, lvl);
  }
}


void og::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value()); // push an integer
  }
  else {
    _pf.SINT(node->value()); // store an integer
  }
}

void og::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value()); // push a double
  }
  else {
    _pf.SDOUBLE(node->value()); // store a double
  }
}

void og::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int strLb;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(strLb = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if (_inFunctionBody) {
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(strLb));
  }
  else {
    // global variable initializer
    _pf.DATA(); // return to the DATA segment
    _pf.SADDR(mklbl(strLb)); // the string to be printed
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->type()->name() == cdk::TYPE_DOUBLE) _pf.DNEG();
  else _pf.NEG(); // 2-complement
}

void og::postfix_writer::do_identity_node(og::identity_node *const node, int lvl) {   
  node->argument()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
}

void og::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int endAnd = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(endAnd));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(endAnd));
}

void og::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int endOr = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(endOr));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(endOr));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  else if (node->type()->name() == cdk::TYPE_POINTER && node->left()->type()->name() == cdk::TYPE_INT) {
    int size = std::dynamic_pointer_cast<cdk::reference_type>(node->type())->referenced()->size();
    _pf.INT(size);
    _pf.MUL();
  }
  
  node->right()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  else if (node->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT) {
    int size = std::dynamic_pointer_cast<cdk::reference_type>(node->type())->referenced()->size();
    _pf.INT(size);
    _pf.MUL();
  }
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DADD();
  else _pf.ADD();
}
void og::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DSUB();
  else _pf.SUB();
}
void og::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DMUL();
  else _pf.MUL();
}
void og::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DDIV();
  else _pf.DIV();
}

void og::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void og::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LT();
}
void og::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.LE();
}
void og::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GE();
}
void og::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.GT();
}
void og::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.NE();
}
void og::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    _pf.I2D();
  
  node->right()->accept(this, lvl);
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    _pf.I2D();
  
  if (node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  _pf.EQ();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->name();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);
  if (symbol->global()) {
    _pf.ADDR(symbol->name());
  }
  else {
    _pf.LOCAL(symbol->offset());
  }
}

void og::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::shared_ptr<og::symbol> varToAttr = new_symbol();
  int varBaseOffset = 0;
  if (varToAttr) varBaseOffset = varToAttr->offset();

  node->lvalue()->accept(this, lvl);

  if (node->type()->name() == cdk::TYPE_INT || node->type()->name() == cdk::TYPE_POINTER || node->type()->name() == cdk::TYPE_STRING) {
    _pf.LDINT();
  }
  else if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  }
  else if (node->type()->name() == cdk::TYPE_STRUCT && varToAttr) {
    auto structuredType = std::dynamic_pointer_cast<cdk::structured_type>(node->type());
    int offset = 0;
    for (size_t i = 0; i < structuredType->length(); i++) {
      auto type = structuredType->component(i);

      _pf.DUP32();
      _pf.INT(offset);
      _pf.ADD(); // tuple to copy offset

      if (type->name() == cdk::TYPE_INT || type->name() == cdk::TYPE_POINTER || type->name() == cdk::TYPE_STRING) {
        _pf.LDINT();
      }
      else if (type->name() == cdk::TYPE_DOUBLE) {
        _pf.LDDOUBLE();
      }

      _pf.LOCAL(varBaseOffset);
      _pf.INT(offset);
      _pf.ADD(); // Local var offset

      if (type->name() == cdk::TYPE_INT || type->name() == cdk::TYPE_POINTER || type->name() == cdk::TYPE_STRING) {
        _pf.STINT();
      }
      else if (type->name() == cdk::TYPE_DOUBLE) {
        _pf.STDOUBLE();
      }

      offset += type->size();
    }
  }

}

void og::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl); // determine the new value

  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    if ( node->rvalue()->type()->name() == cdk::TYPE_INT ) _pf.I2D();
    _pf.DUP64();
    node->lvalue()->accept(this, lvl);
    _pf.STDOUBLE();
  }
  else {
    _pf.DUP32();
    node->lvalue()->accept(this, lvl);
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  auto type = node->argument()->type();
  if (type->name() != cdk::TYPE_VOID) _pf.TRASH(type->size());
}

void og::postfix_writer::do_print_node(og::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t i = 0; i < node->argument()->size() ; i++) {
    auto currNode = (cdk::expression_node*) node->argument()->node(i);
    currNode->accept(this, lvl); // determine the value to print

    if (currNode->is_typed(cdk::TYPE_INT)) {
      _functions_undeclared.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4); // delete the int value
    } else if (currNode->is_typed(cdk::TYPE_DOUBLE)) {
      _functions_undeclared.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8); // delete the double value
    } else if (currNode->is_typed(cdk::TYPE_STRING)) {
      _functions_undeclared.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4); // delete the printed value's address
    } else {
      std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
      exit(1);
    }
  }
  if (node->newline()) {
    _functions_undeclared.insert("println");
    _pf.CALL("println"); // print a newline
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_read_node(og::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == cdk::TYPE_INT) {
    _functions_undeclared.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  }
  else if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _functions_undeclared.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_block_node(og::block_node *const node, int lvl) {
  _symtab.push();
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_for_node(og::for_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lblCond = ++_lbl, lblEnd = ++_lbl, lblInc = ++_lbl;

  _forInc.push(lblInc); 
  _forEnd.push(lblEnd);

  if (node->inits())
    node->inits()->accept(this, lvl);

  _pf.LABEL(mklbl(lblCond));
  if (node->conditions())
    node->conditions()->accept(this, lvl);
  _pf.JZ(mklbl(lblEnd));

  node->block()->accept(this, lvl + 2);

  _pf.LABEL(mklbl(lblInc));
  if (node->incs())
    node->incs()->accept(this, lvl);
  _pf.JMP(mklbl(lblCond));

  _pf.LABEL(mklbl(lblEnd));

  _forInc.pop();
  _forEnd.pop();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_break_node(og::break_node *const node, int lvl) {
  if (_forEnd.size() != 0) {
    _pf.JMP(mklbl(_forEnd.top()));
  }
  else {
    throw new std::string("Cannot perform a break outside a 'for' loop.");
  }
}

void og::postfix_writer::do_continue_node(og::continue_node *const node, int lvl) {   
  if (_forInc.size() != 0) {
    _pf.JMP(mklbl(_forInc.top()));
  } else {
    throw new std::string("Cannot perform a continue outside a 'for' loop.");
  }

}

//---------------------------------------------------------------------------

void og::postfix_writer::do_return_node(og::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_inBranch) _hasCurrBranchReturn = true;
  else _hasFixedReturn = true;

  auto nametype = _function->type()->name();

  if (nametype != cdk::TYPE_VOID) {
    
    if (nametype == cdk::TYPE_INT || nametype == cdk::TYPE_POINTER || nametype == cdk::TYPE_STRING) {
      node->expressions()->accept(this, lvl + 2);
      _pf.STFVAL32();
    }
    else if (nametype == cdk::TYPE_DOUBLE) {
      node->expressions()->accept(this, lvl + 2);
      if (node->expressions()->values()->size() == 1 && dynamic_cast<cdk::expression_node*>(node->expressions()->values()->node(0))->type()->name() == cdk::TYPE_INT)
        _pf.I2D();
      _pf.STFVAL64();
    }
    else if (nametype == cdk::TYPE_STRUCT) {
      int valueOffset = 0;
      for (size_t i = 0; i < node->expressions()->values()->size(); i++) {
        cdk::expression_node* value = dynamic_cast<cdk::expression_node*>(node->expressions()->values()->node(i));
        auto currType = std::dynamic_pointer_cast<cdk::structured_type>(_function->type())->component(i);

        value->accept(this, lvl); 

        if (value->type()->name() == cdk::TYPE_INT || value->type()->name() == cdk::TYPE_STRING
            || value->type()->name() == cdk::TYPE_POINTER) {
          bool isDouble = currType->name() == cdk::TYPE_DOUBLE;
          if (isDouble) _pf.I2D();
          _pf.LOCAL(8); // &tuplo
          _pf.LDINT();
          _pf.INT(valueOffset);
          _pf.ADD();
          if (isDouble) _pf.STDOUBLE();
          else _pf.STINT();
        }
        else if (value->type()->name() == cdk::TYPE_DOUBLE) {
          _pf.LOCAL(8);
          _pf.LDINT();
          _pf.INT(valueOffset);
          _pf.ADD();
          _pf.STDOUBLE();
        }

        valueOffset += currType->size();
      }
    }
  }

  if (_tupleSizeToTrash > 0) {
    _pf.TRASH(_tupleSizeToTrash); // trash tuple if allocated to it
    _tupleSizeToTrash = 0;
  }
  _pf.LEAVE();
  _pf.RET();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_function_call_node(og::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<og::symbol> symbol = _symtab.find(node->identifier());

  size_t sizeArgs = 0;
  auto type = symbol->type();

  // Alloc memory for tuple
  std::string labelSP = mklbl(++_lbl);
  if (type->name() == cdk::TYPE_STRUCT) {
    _pf.INT(type->size());
    _pf.ALLOC();
    _pf.SP(); // &tuplo
    _pf.BSS(); // guardar para label
    _pf.ALIGN();
    _pf.LABEL(labelSP);
    _pf.SALLOC(4);
    _pf.TEXT();
    _pf.ALIGN();
    _pf.ADDR(labelSP);
    _pf.STINT();

    sizeArgs += 4 + type->size();
  }

  if (node->arguments()) {
    for (int i = (node->arguments()->size())-1; i >= 0; i--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
      arg->accept(this, lvl + 2);

      if (symbol->args()->at(i)->name() == cdk::TYPE_DOUBLE && arg->type()->name() == cdk::TYPE_INT)
        _pf.I2D();

      sizeArgs += arg->type()->size();
    }
  }

  if (type->name() == cdk::TYPE_STRUCT) {
    // carregar label
    _pf.ADDR(labelSP);
    _pf.LDINT();
  }
  
  _pf.CALL(node->identifier());

  if (type->name() == cdk::TYPE_STRUCT && _varsToAttr.size() > 0) {
    auto structuredType = std::dynamic_pointer_cast<cdk::structured_type>(node->type());
    int offset = 0;
    for (size_t i = 0; i < structuredType->length(); i++) {
      auto type = structuredType->component(i);

      _pf.DUP32();
      _pf.INT(offset);
      _pf.ADD(); // SP offset

      if (type->name() == cdk::TYPE_INT || type->name() == cdk::TYPE_POINTER || type->name() == cdk::TYPE_STRING) {
        _pf.LDINT();
      }
      else if (type->name() == cdk::TYPE_DOUBLE) {
        _pf.LDDOUBLE();
      }

      int index = 0;
      if (_varsToAttr.size() == structuredType->length()) index = i;
      auto varToAttr = _varsToAttr.at(index);
      _pf.LOCAL(varToAttr->offset());
      if (_varsToAttr.size() != structuredType->length()) {
        _pf.INT(offset);
        _pf.ADD(); // Local var offset
      }

      if (type->name() == cdk::TYPE_INT || type->name() == cdk::TYPE_POINTER || type->name() == cdk::TYPE_STRING) {
        _pf.STINT();
      }
      else if (type->name() == cdk::TYPE_DOUBLE) {
        _pf.STDOUBLE();
      }

      offset += type->size();
    }
  }

  if ( _varsToAttr.size() == 0) {
    sizeArgs -= type->size();
    _tupleSizeToTrash += type->size();
  }
  if (sizeArgs != 0) _pf.TRASH(sizeArgs);

  if (type->name() == cdk::TYPE_INT || type->name() == cdk::TYPE_POINTER || type->name() == cdk::TYPE_STRING) {
    _pf.LDFVAL32();
  }
  else if (type->name() == cdk::TYPE_DOUBLE) {
    _pf.LDFVAL64();
  }
  _varsToAttr.clear();

  if (type->name() == cdk::TYPE_STRUCT && _varsToAttr.size() == 0) {
    _pf.ADDR(labelSP);
    _pf.LDINT();
  }
}

void og::postfix_writer::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
  if (_inFunctionBody || _inFunctionArgs)
    throw new std::string("Cannot define function in function body or args");

  ASSERT_SAFE_EXPRESSIONS;

  if (!new_symbol()) return;

  std::shared_ptr<og::symbol> function = new_symbol();

  std::vector<std::shared_ptr<cdk::basic_type>> args;
  if (node->arguments()) {
    for (size_t i = 0; i < node->arguments()->size(); i++) {
      args.push_back(dynamic_cast<og::variable_declaration_node*>(node->arguments()->node(i))->type());
    }
  }

  function->args(args);

  _functions_undeclared.insert(function->name());
  reset_new_symbol();
}

void og::postfix_writer::do_function_definition_node(og::function_definition_node *const node, int lvl) {
  if (_inFunctionBody || _inFunctionArgs)
    throw new std::string("Cannot define function in function body");

  ASSERT_SAFE_EXPRESSIONS;

  // define symbol
  _function = new_symbol();
  _functions_undeclared.erase(_function->name());
  reset_new_symbol();
  _tupleSizeToTrash = 0;

  if (node->type()->name() == cdk::TYPE_UNSPEC) node->type(_function->type());

  _offset = 8;
  if (node->type()->name() == cdk::TYPE_STRUCT) _offset += 4;

  _symtab.push(); // args scope
  std::vector<std::shared_ptr<cdk::basic_type>> args;
  if (node->arguments()) {
    _inFunctionArgs = true;
    for (size_t i = 0; i < node->arguments()->size(); i++) {
      cdk::basic_node *arg = node->arguments()->node(i);
      if (arg == nullptr) break;
      arg->accept(this, 0);
      args.push_back(dynamic_cast<og::variable_declaration_node*>(node->arguments()->node(i))->type());
    }
    _inFunctionArgs = false;
  }

  _function->args(args);

  _pf.TEXT();
  _pf.ALIGN();

  if (node->qualifier() == tPUBLIC) 
    _pf.GLOBAL(_function->name(), _pf.FUNC());
  _pf.LABEL(_function->name());

  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables

  _offset = -_function->type()->size();

  os() << ";; before body" << std::endl;
  _inFunctionBody = true;
  _hasFixedReturn = false;
  _inBranch = false;
  node->body()->accept(this, lvl + 4);
  _inFunctionBody = false;
  os() << ";; after body" << std::endl;

  if (!_hasFixedReturn && _function->type()->name() != cdk::TYPE_VOID) {
    std::cerr << "Function doesn't have a return" << std::endl;
    exit(1);
  }

  _symtab.pop(); //Get out of args scope

  if (_function->type()->name() == cdk::TYPE_VOID) {
    _pf.LEAVE();
    _pf.RET();
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto names = node->identifiers();
  for (unsigned int i = 0; i < names->size(); i++) {
      auto id = node->identifiers()->at(i);

      std::shared_ptr<og::symbol> symbol = _symtab.find(id);
      if (!symbol) std::cerr << " symbol not found, cannot happen" << std::endl;

      int offset = 0, size = symbol->type()->size();

      if (_inFunctionBody) {
        _offset -= size;
        offset = _offset;
      }
      else if (_inFunctionArgs) {
        offset = _offset;
        _offset += size;
      }

      symbol->offset(offset);
      reset_new_symbol();

      if (_inFunctionBody) {
        // Local, only writes if initialized
        symbol->global(false);
        if (node->expression()) {
          // Value comes from tuple node
          if ( node->expression()->type()->name() == cdk::TYPE_STRUCT && !symbol->isTuple()) {
            og::tuple_node* tuple = dynamic_cast<og::tuple_node*>(node->expression());

            if (names->size() > tuple->values()->size()) {
              // auto a,b,c = f(); bigger number of ids than expressions
              if (i < (names->size()-1)) {
                _varsToAttr.push_back(symbol);
                continue;
              }
              _varsToAttr.push_back(symbol);
              tuple->values()->accept(this, lvl);
            } 
            else if (names->size() == tuple->values()->size()) {
              // auto a,b,c = 1,2,3; same number of ids and expressions
              cdk::basic_node* value = tuple->values()->node(i);
              value->accept(this, lvl);
            }
            
          } else if (node->expression()->type()->name() == cdk::TYPE_STRUCT && symbol->isTuple()) {
            int valueOffset = 0;
            og::tuple_node* tuple = dynamic_cast<og::tuple_node*>(node->expression());
            
            if (tuple->values()->size() == 1) { // This happens if the expression is a function call or a tuple
              _varsToAttr.push_back(symbol);
              set_new_symbol(symbol);
              tuple->values()->accept(this, lvl);
              reset_new_symbol();
            }
            else {
              for (size_t i = 0; i < tuple->values()->size(); i++) {
                cdk::expression_node* value = dynamic_cast<cdk::expression_node*>(tuple->values()->node(i));
                value->accept(this, lvl);

                if (value->type()->name() == cdk::TYPE_INT || value->type()->name() == cdk::TYPE_STRING
                    || value->type()->name() == cdk::TYPE_POINTER) {
                  _pf.LOCAL(symbol->offset());
                  _pf.INT(valueOffset);
                  _pf.ADD();
                  _pf.STINT();
                }
                else if (value->type()->name() == cdk::TYPE_DOUBLE) {
                  _pf.LOCAL(symbol->offset());
                  _pf.INT(valueOffset);
                  _pf.ADD();
                  _pf.STDOUBLE();
                }

                valueOffset += value->type()->size();
              }
            }
          }
          else node->expression()->accept(this, lvl);

          if (node->type()->name() == cdk::TYPE_INT || node->type()->name() == cdk::TYPE_STRING
              || node->type()->name() == cdk::TYPE_POINTER) {
            _pf.LOCAL(symbol->offset());
            _pf.STINT();
          } else if (node->type()->name() == cdk::TYPE_DOUBLE) {
            if (node->expression()->type()->name() == cdk::TYPE_INT) _pf.I2D();
            _pf.LOCAL(symbol->offset());
            _pf.STDOUBLE();
          } else if (node->type()->name() == cdk::TYPE_STRUCT && !symbol->isTuple() 
              && names->size() == dynamic_cast<og::tuple_node*>(node->expression())->values()->size()) {
            _pf.LOCAL(symbol->offset());
            if ( symbol->type()->name() == cdk::TYPE_DOUBLE ) {
              _pf.STDOUBLE();
            }
            else _pf.STINT();

          } else if (node->type()->name() != cdk::TYPE_STRUCT){
            std::cerr << "cannot initialize" << std::endl;
          }
        }
      }
      else if (!_function) {
        // Global
        symbol->global(true);

        if (node->expression() == nullptr) {
          _pf.BSS();
          _pf.ALIGN();
          _pf.LABEL(id);
          _pf.GLOBAL(id, _pf.OBJ());
          _pf.SALLOC(size);
        }
        else {
          if (node->type()->name() == cdk::TYPE_INT 
            || node->type()->name() == cdk::TYPE_DOUBLE 
            || node->type()->name() == cdk::TYPE_POINTER) {
            _pf.DATA();
            _pf.ALIGN();
            _pf.GLOBAL(id, _pf.OBJ());
            _pf.LABEL(id);

            if (node->type()->name() == cdk::TYPE_INT || node->type()->name() == cdk::TYPE_POINTER) {
              node->expression()->accept(this, lvl);
            }
            else if (node->type()->name() == cdk::TYPE_DOUBLE) {
              if (node->expression()->type()->name() == cdk::TYPE_DOUBLE) {
                node->expression()->accept(this, lvl);
              }
              else if (node->expression()->type()->name() == cdk::TYPE_INT) {
                cdk::integer_node *ini = dynamic_cast<cdk::integer_node*>(node->expression());
                cdk::double_node curr(ini->lineno(), ini->value());
                curr.accept(this, lvl);
              }
              else {
                std::cerr << node->lineno() << ": '" << id << "' has bad initializer for real value\n";
              }
            }
          }
          else if (node->type()->name() == cdk::TYPE_STRING) {
            _pf.DATA();
            _pf.ALIGN();
            _pf.GLOBAL(id, _pf.OBJ());
            _pf.LABEL(id);
        
            node->expression()->accept(this, lvl);
          }
          else if (node->type()->name() == cdk::TYPE_STRUCT) {
            _pf.DATA();
            _pf.ALIGN();
            _pf.GLOBAL(id, _pf.OBJ());
            _pf.LABEL(id);
            if ( node->expression()->type()->name() == cdk::TYPE_STRUCT && !symbol->isTuple()) {
              // Value comes from tuple node
              og::tuple_node* tuple = dynamic_cast<og::tuple_node*>(node->expression());
              cdk::basic_node* value = tuple->values()->node(i);
              value->accept(this, lvl);
            }
            else {
              node->expression()->accept(this, lvl);
            }
          }
        }
      }
  }
  _varsToAttr.clear();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_stack_alloc_node(og::stack_alloc_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int size = std::dynamic_pointer_cast<cdk::reference_type>(node->type())->referenced()->size();
  node->argument()->accept(this, lvl);  

  _pf.INT(size);
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();

}

//---------------------------------------------------------------------------

void og::postfix_writer::do_address_of_node(og::address_of_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl + 2);
}

void og::postfix_writer::do_sizeof_node(og::sizeof_node *const node, int lvl) {       
  ASSERT_SAFE_EXPRESSIONS;

  int value = 0;
  for (size_t i = 0; i < node->expressions()->size(); i++) {
    cdk::expression_node *currNode = dynamic_cast<cdk::expression_node*>(node->expressions()->node(i));
    value += currNode->type()->size();
  }

  _pf.INT(value);
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_index_node(og::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->base()->accept(this, lvl);
  node->index()->accept(this, lvl);

  auto size = node->type()->size();
  _pf.INT(size);
  _pf.MUL();
  _pf.ADD();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->base()->accept(this, lvl);

  int size = 0;
  for (int i = 0; i < (node->index() - 1); i++) {
    size += std::dynamic_pointer_cast<cdk::structured_type>(node->base()->type())->component(i)->size();
  }
  _pf.INT(size);
  _pf.ADD();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_node(og::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_else_node(og::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));

  // store return booleans
  bool beforeBranchReturn = _hasCurrBranchReturn;
  bool beforeInBranch = _inBranch;
  _hasCurrBranchReturn = false;
  _inBranch = true;

  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));

  bool ifReturn = _hasCurrBranchReturn;
  _hasCurrBranchReturn = false;
  node->elseblock()->accept(this, lvl + 2);
  if (ifReturn && _hasCurrBranchReturn) _hasFixedReturn = true;
  
  _pf.LABEL(mklbl(lbl1 = lbl2));

  // return to before return booleans
  _hasCurrBranchReturn = beforeBranchReturn;
  _inBranch = beforeInBranch;
}
