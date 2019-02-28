#include <motion.hpp>
#include <utils/options.hpp>

#include <boost/program_options.hpp>


namespace po = boost::program_options;

motion::motion(void* _ub)
{
    this->unBoard = static_cast<UnBoard*>(_ub);

    po::variables_map vm;

    po::options_description cmdline_options = store_and_notify(std::vector<string>(0), vm, NULL);

    runswiftMotion = boost::shared_ptr<MotionAdapter>(new MotionAdapter(vm, unBoard));
}

void motion::tick()
{
    runswiftMotion->tick();
}
