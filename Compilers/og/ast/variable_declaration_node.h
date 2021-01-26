#ifndef __OG_AST_VARIABLE_DECLARATION_NODE_H__
#define __OG_AST_VARIABLE_DECLARATION_NODE_H__

#include <string>
#include <vector>
#include <cdk/ast/typed_node.h>
#include <cdk/types/basic_type.h>

namespace og {

  /**
   * Class for describing variable declaration nodes.
   */
  class variable_declaration_node: public cdk::typed_node {
    int _qualifier;
    std::vector<std::string> *_identifiers;
    cdk::expression_node *_expression;

  public:
    inline variable_declaration_node(int lineno, int qualifier, cdk::basic_type *type, std::vector<std::string> *identifiers) :
        typed_node(lineno), _qualifier(qualifier), _identifiers(identifiers), _expression(nullptr) {
          this->type(std::shared_ptr<cdk::basic_type>(type));
    }

    inline variable_declaration_node(int lineno, int qualifier, cdk::basic_type *type, std::vector<std::string> *identifiers,
                  cdk::expression_node *expression) :
        typed_node(lineno), _qualifier(qualifier), _identifiers(identifiers), _expression(expression) {
          this->type(std::shared_ptr<cdk::basic_type>(type));
    }

  public:
    inline int qualifier() {
      return _qualifier;
    }

    inline std::vector<std::string> *identifiers() {
      return _identifiers;
    }

    inline cdk::expression_node *expression() {
      return _expression;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // og

#endif
