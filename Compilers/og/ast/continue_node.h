#ifndef __OG_AST_CONTINUE_NODE_H__
#define __OG_AST_CONTINUE_NODE_H__

namespace og {

  /**
   * Class for describing continue nodes.
   */
  class continue_node: public cdk::basic_node {

  public:
    inline continue_node(int lineno) :
        cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_continue_node(this, level);
    }

  };

} // og

#endif
