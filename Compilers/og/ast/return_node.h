#ifndef __OG_AST_RETURN_NODE_H__
#define __OG_AST_RETURN_NODE_H__

#include <vector>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing return nodes.
   */
  class return_node: public cdk::basic_node {
    og::tuple_node *_expressions;

  public:
    inline return_node(int lineno) :
        basic_node(lineno), _expressions(nullptr) {
    }
    inline return_node(int lineno, og::tuple_node *expressions) :
        basic_node(lineno), _expressions(expressions) {
    }

  public:
    inline og::tuple_node *expressions() {
      return _expressions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // og

#endif
