#include "inc/acting.hpp"

namespace acting {

cell::behavior_type cellActor(cell::pointer self) {
    auto handle_err = [=](const error &err) {
        aout(self) << "FAILURE" << std::endl;
    };
    return {
        [=](StartMsg b) -> ResultMsg {
            
        }
    };
}
}
