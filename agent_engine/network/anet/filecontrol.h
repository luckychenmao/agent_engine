#ifndef ANET_FILECONTROL_H_
#define ANET_FILECONTROL_H_
namespace anet {
class FileControl {
public:
    static bool setCloseOnExec(int fd);
    static bool clearCloseOnExec(int fd);
    static bool isCloseOnExec(int fd);
};
} // namespace anet

#endif /*FILECONTROL_H_*/
