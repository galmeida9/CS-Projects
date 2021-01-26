#ifndef __OG_TARGETS_SYMBOL_H__
#define __OG_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <vector>
#include <cdk/types/basic_type.h>

namespace og {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    long _value; // hack!

    bool _constant;
    int _qualifier;
    bool _initialized;
    bool _tuple = false;
    int _offset = 0;
    bool _function = false;
    bool _global = false;

    std::vector<std::shared_ptr<cdk::basic_type>> _args;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name) :
        _type(type), _name(name), _value(0) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    void type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    bool constant() {
      return _constant;
    }
    void constant(bool c) {
      _constant = c;
    }
    int qualifier() {
      return _qualifier;
    }
    void qualifier(int q) {
      _qualifier = q;
    }
    bool isTuple() {
      return _tuple;
    }
    void isTuple(bool i) {
      _tuple = i;
    }
    bool initialized() {
      return _initialized;
    }
    void initialized(bool i) {
      _initialized = i;
    }
    int offset() {
      return _offset;
    }
    void offset(int o) {
      _offset = o;
    }
    bool function() {
      return _function;
    }
    void function(bool f) {
      _function = f;
    }
    bool global() {
      return _global;
    }
    void global(bool g) {
      _global = g;
    }
    std::vector<std::shared_ptr<cdk::basic_type>>* args() {
      return &_args;
    }
    void args(std::vector<std::shared_ptr<cdk::basic_type>> a) {
      _args = a;
    }
    
  };

} // og

#endif
