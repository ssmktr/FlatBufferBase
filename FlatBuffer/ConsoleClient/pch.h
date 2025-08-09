#pragma once

//#include <sdkddkver.h>

#include <iostream>
#include <deque>
#include <cstdlib>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Message.h"
#include "flatbuffers/flatbuffers.h"
#include "protocol_generated.h"

using namespace std;
using boost::asio::ip::tcp;

