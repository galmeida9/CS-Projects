#ifndef __OG_AST_IDENTITY_NODE_H__
#define __OG_AST_IDENTITY_NODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/unary_operation_node.h>

namespace og {

  /**
   * Class for describing identity nodes.
   */
  class identity_node: public cdk::unary_operation_node {

  public:
    inline identity_node(int lineno, cdk::expression_node *expr) :
        unary_operation_node(lineno, expr) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // og

#endif
