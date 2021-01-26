#ifndef __OG_AST_FUNCTION_DECLARATION_NODE_H__
#define __OG_AST_FUNCTION_DECLARATION_NODE_H__

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/types/basic_type.h>

namespace og {

  /**
   * Class for describing function_declaration nodes.
   */
  class function_declaration_node: public cdk::typed_node {
    int _qualifier;
    std::string _identifier;
    cdk::sequence_node *_arguments;

  public:
    inline function_declaration_node(int lineno, cdk::basic_type *type, const std::string &identifier, int qualifier = 0) :
        typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(new cdk::sequence_node(lineno)) {
          this->type(std::shared_ptr<cdk::basic_type>(type));
    }

    inline function_declaration_node(int lineno,  cdk::basic_type *type, const std::string &identifier,
                              cdk::sequence_node *arguments, int qualifier = 0) :
        typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments) {
          this->type(std::shared_ptr<cdk::basic_type>(type));
    }

  public:
    inline int qualifier() {
      return _qualifier;
    }

    inline std::string identifier() {
      return _identifier;
    }

    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // og

#endif
