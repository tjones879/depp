#include "inc/acting.hpp"
#include <stack>
#include <utility>
#include <chrono>

namespace acting {

vector::behavior_type vectorActor(vector::pointer self) {
    return {
        [=](StartMsg b) -> ResultMsg {
            aout(self) << "TEST" << std::endl;
            return ResultMsg{10};
        }
    };
}

template <class Handle, class Task>
void tester(scoped_actor &self, const Handle& hdl, Task t) {
    auto handle_err = [&](const error &err) {
        aout(self) << "FAILURE" << std::endl;
    };
    self->request(hdl, infinite, t).receive(
        [&](ResultMsg ans) {
            aout(self) << "SUCCESS" << std::endl;
        }, handle_err);
}

void startActing() {
    actor_system_config cfg;
    actor_system system{cfg};
    scoped_actor main{system};
    StartMsg t{1};
    auto actor = system.spawn(vectorActor);
    tester(main, actor, t);
}
}
