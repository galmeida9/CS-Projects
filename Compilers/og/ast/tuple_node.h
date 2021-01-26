#ifndef __OG_AST_TUPLE_NODE_H__
#define __OG_AST_TUPLE_NODE_H__

#include <string>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing tuple nodes.
   */
  class tuple_node: public cdk::expression_node {
    cdk::sequence_node *_values;
    int _qualifier;


  public:
    inline tuple_node(int lineno, cdk::sequence_node *values, int qualifier = 0) :
        expression_node(lineno), _values(values), _qualifier(qualifier) {
    }

  public:
    inline int qualifier() {
      return _qualifier;
    }

    inline cdk::sequence_node *values(){
      return _values;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_tuple_node(this, level);
    }

  };

} // og

#endif
