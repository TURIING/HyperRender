//
// Created by turiing on 2025/11/26.
//

#ifndef IANIMATIONTOOL_H
#define IANIMATIONTOOL_H

namespace HyperRender {
class IAnimationTool {
public:
    virtual ~IAnimationTool() = default;

    virtual void RunAction();
};
}
#endif //IANIMATIONTOOL_H
