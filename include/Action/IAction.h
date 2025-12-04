//
// Created by turiing on 2025/11/26.
//

#ifndef IACTION_H
#define IACTION_H

namespace HyperRender {
class IAction {
public:
    virtual ~IAction() = default;

    /**
     * 暂停动画
     */
    virtual void Pause() = 0;

    /**
     * 恢复动画
     */
    virtual void Resume() = 0;
};
}
#endif //IACTION_H
