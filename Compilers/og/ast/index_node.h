#ifndef __OG_AST_INDEX_NODE_H__
#define __OG_AST_INDEX_NODE_H__

#include <cdk/ast/lvalue_node.h>

namespace og {

  /**
   * Class for describing left index nodes.
   */
  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_base, *_index;


  public:
    inline index_node(int lineno, cdk::expression_node *base, cdk::expression_node *index) :
        lvalue_node(lineno), _base(base), _index(index) {
    }

  public:
    inline cdk::expression_node *base(){
      return _base;
    }

    inline cdk::expression_node *index(){
      return _index;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // og

#endif
