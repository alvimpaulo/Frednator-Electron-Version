#ifndef MOTION_HPP
#define MOTION_HPP

#include <basemodule.hpp>
#include <motion/MotionAdapter.hpp>
#include <boost/shared_ptr.hpp>

class motion : public baseModule
{
public:
    void tick();
    motion(void* _ub);
private:
    int a;
    boost::shared_ptr<MotionAdapter> runswiftMotion;
};

#endif // MOTION_HPP
