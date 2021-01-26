#ifndef __OG_AST_NULLPTR_NODE_H__
#define __OG_AST_NULLPTR_NODE_H__

#include <cdk/ast/expression_node.h>

namespace og {

  /**
   * Class for describing nullptr nodes.
   */
  class nullptr_node: public cdk::expression_node {
  public:
    inline nullptr_node(int lineno) :
        expression_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_nullptr_node(this, level);
    }

  };

} // og

#endif
