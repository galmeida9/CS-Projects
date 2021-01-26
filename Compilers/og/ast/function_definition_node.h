#ifndef __OG_AST_FUNCTION_DEFINITION_NODE_H__
#define __OG_AST_FUNCTION_DEFINITION_NODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/types/basic_type.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing function_definition nodes.
   */
  class function_definition_node: public cdk::typed_node {
    int _qualifier;
    std::string _identifier;
    cdk::sequence_node *_arguments;
    og::block_node *_body;

  public:
    inline function_definition_node(int lineno, cdk::basic_type *type, const std::string &identifier, og::block_node *body, 
                              int qualifier = 0) :
        typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(new cdk::sequence_node(lineno)),
        _body(body) {
          this->type(std::shared_ptr<cdk::basic_type>(type));
    }

    inline function_definition_node(int lineno, cdk::basic_type *type, const std::string &identifier,
                              cdk::sequence_node *arguments, og::block_node *body, int qualifier = 0) :
        typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments), _body(body) {
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

    inline cdk::basic_node *body() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // og

#endif
