#pragma once

#include <boost/shared_ptr.hpp>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/alproxy.h>

struct ProxyData
{
	boost::shared_ptr<AL::ALBroker> broker;
	boost::shared_ptr<AL::ALProxy> Motion;
	boost::shared_ptr<AL::ALProxy> Communication;
	boost::shared_ptr<AL::ALProxy> Vision;
};
