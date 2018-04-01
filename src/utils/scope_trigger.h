/******************************************************************************
 * @file: scope_trigger.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-03-31
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_SCOPE_TRIGGER_H
#define SRC_UTILS_SCOPE_TRIGGER_H

#include "utils/macros.h"

namespace picasso {
namespace utils {


// We need combine because we need __LINE__ evaluated
#define SCOPED_TRIGGER(begin_trigger, end_trigger)           \
  auto COMBINE(trigger, __LINE__) = ::picasso::utils::CreateScopeTrigger(        \
      [&]() { (begin_trigger); },                              \
      [&]() { (end_trigger); }                                 \
  );


/**
 * Helper class used for running factors on constructor & destructor.
 * The idea is to be able to run work that normally run in pairs,
 * at the beginning of the scope and at the end.
 * @tparam CtrFunctor Object has an operator()(void) to be called.
 *    Will be called at ScopeTrigger constructor.
 *    Needs to be save to construct/copy/destroy (no side-effects)
 * @tparam DtrFunctor Object has an operator()(void) to be called.
 *    Will be stored and called at ScopeTrigger destructor.
 *    Needs to be save to construct/copy/destroy (no side-effects)
 *
 * eg:
 *
 *  {
 *    ScopeTrigger<Start, End>();   <-- Runs Start()()
 *
 *
 *
 *  }   <--- Runs End()()
 *
 */
template <typename CtrFunctor, typename DtrFunctor>
class ScopeTrigger {
 public:
  ScopeTrigger() {
    ctr_functor_();
  }

  /**
   * Can override with specific instances.
   * This is useful for when we need functors that have state.
   */
  ScopeTrigger(const CtrFunctor& ctr, const DtrFunctor& dtr)
    : ctr_functor_(ctr), dtr_functor_(dtr) {
    ctr_functor_();
  }

  ~ScopeTrigger() {
    if (valid_dtr_) {
      dtr_functor_();
    }
  }

 public:
  DISABLE_COPY(ScopeTrigger);

 public:
  ScopeTrigger(ScopeTrigger&& other) 
    : ctr_functor_(std::move(other.ctr_functor_)),
      dtr_functor_(std::move(other.dtr_functor_)),
      valid_dtr_(other.valid_dtr_) {
    other.valid_dtr_ = false;
  }
  ScopeTrigger& operator=(ScopeTrigger&& other) {
    if (this != &other) {
      ctr_functor_ = std::move(other.ctr_functor_);
      dtr_functor_ = std::move(other.dtr_functor_);
      valid_dtr_ = other.valid_dtr_;
      other.valid_dtr_ = false;
    }
    return *this;
  }

 private:
  CtrFunctor ctr_functor_;
  DtrFunctor dtr_functor_;
  bool valid_dtr_ = true;
};  // class ScopeTrigger

template<typename CtrFunctor, typename DtrFunctor>
ScopeTrigger<CtrFunctor, DtrFunctor> 
CreateScopeTrigger(const CtrFunctor& ctr, const DtrFunctor& dtr) {
  return { ctr, dtr };
}

}   // namespace utils
}   // namespace picasso

#endif  // SRC_UTILS_SCOPE_TRIGGER_H
